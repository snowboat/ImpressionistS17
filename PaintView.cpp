//
// paintview.cpp
//
// The code maintaining the painting view of the input images
//

#include "impressionist.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "paintview.h"
#include "ImpBrush.h"
#include <iostream>
#include <algorithm>
#include <math.h>
using namespace std;


#define LEFT_MOUSE_DOWN		1
#define LEFT_MOUSE_DRAG		2
#define LEFT_MOUSE_UP		3
#define RIGHT_MOUSE_DOWN	4
#define RIGHT_MOUSE_DRAG	5
#define RIGHT_MOUSE_UP		6
#define AUTO_DRAW			7
#define	PAINTERLY			8


#ifndef WIN32
#define min(a, b)	( ( (a)<(b) ) ? (a) : (b) )
#define max(a, b)	( ( (a)>(b) ) ? (a) : (b) )
#endif

static int		eventToDo;
static int		isAnEvent=0;
static Point	coord;

Point startPoint;

PaintView::PaintView(int			x, 
					 int			y, 
					 int			w, 
					 int			h, 
					 const char*	l)
						: Fl_Gl_Window(x,y,w,h,l)
{
	m_nWindowWidth	= w;
	m_nWindowHeight	= h;
	// cout << "paintview is " << w << " and " << h << endl;	
	painterlyRadius = 10;
	painterlyRefimage = NULL;
}


void PaintView::draw()
{
	#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_FRONT_AND_BACK);
	#endif // !MESA

	if(!valid())//initialize opengl
	{

		glClearColor(0.7f, 0.7f, 0.7f, 1.0);

		// We're only using 2-D, so turn off depth 
		glDisable( GL_DEPTH_TEST );

		ortho();

		glClear( GL_COLOR_BUFFER_BIT );//clear the buffer

		//enable alpha blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	}

	Point scrollpos;// = GetScrollPosition();
	scrollpos.x = 0;
	scrollpos.y	= 0;

	m_nWindowWidth	= w();
	m_nWindowHeight	= h();
	
	int drawWidth, drawHeight;
	drawWidth = min( m_nWindowWidth, m_pDoc->m_nPaintWidth );
	drawHeight = min( m_nWindowHeight, m_pDoc->m_nPaintHeight );

	int startrow = m_pDoc->m_nPaintHeight - (scrollpos.y + drawHeight);
	if ( startrow < 0 ) startrow = 0;

	m_pPaintBitstart = m_pDoc->m_ucPainting + 
		3 * ((m_pDoc->m_nPaintWidth * startrow) + scrollpos.x);

	m_nDrawWidth	= drawWidth;
	m_nDrawHeight	= drawHeight;

	m_nStartRow		= startrow;
	m_nEndRow		= startrow + drawHeight;
	m_nStartCol		= scrollpos.x;
	m_nEndCol		= m_nStartCol + drawWidth;

	//cout << "startrow endrow startcoloum endcol" << m_nStartRow << " " << m_nEndRow << " "
	//	<< m_nStartCol << " " << m_nEndCol << endl;

	if ( m_pDoc->m_ucPainting && !isAnEvent) 
	{
		RestoreContent();

	}

	if ( m_pDoc->m_ucPainting && isAnEvent) 
	{

		// Clear it after processing.
		isAnEvent	= 0;	

		Point source( coord.x + m_nStartCol, m_nEndRow - coord.y );
		Point target( coord.x, m_nWindowHeight - coord.y );

		//for auto painting
		int spacing = m_pDoc->m_pUI->getPaintSpacing();
		int sizeBeforeRandom = m_pDoc->m_pUI->getSize();
		int sizeCap = min(40, m_pDoc->m_pUI->getSize()*2);
		if (sizeCap <= 0)
			sizeCap = 1;

		//implement random ordering of brush locations
		std::vector<int> xorder, yorder;
		for (int i = 0; i < drawWidth ; i+= spacing) {
			xorder.push_back(i);
		}
		for (int j = 0; j < drawHeight; j+= spacing) {
			yorder.push_back(j);
		}
		std::random_shuffle(xorder.begin(), xorder.end());
		std::random_shuffle(yorder.begin(), yorder.end());
		std::vector<Point> randomDrawingOrder;

		// This is the event handler
		switch (eventToDo) 
		{
		case LEFT_MOUSE_DOWN:


			if (coord.x >= 0 && coord.x <= m_nDrawWidth && coord.y >= 0 && coord.y <= m_nDrawHeight) {
				//save current m_ucPainting to the undoImage
				delete [] m_pDoc->m_undoImage;
				m_pDoc->m_undoImage = new unsigned char[m_nDrawHeight*m_nDrawWidth * 3];
				memcpy(m_pDoc->m_undoImage, m_pDoc->m_ucPainting, m_nDrawHeight*m_nDrawWidth * 3);
				
				//Actually start painting
				m_pDoc->m_pCurrentBrush->BrushBegin(source, target);
			}
			break;
		case LEFT_MOUSE_DRAG:
			if (coord.x >= 0 && coord.x <= m_nDrawWidth && coord.y >= 0 && coord.y <= m_nDrawHeight) {
				m_pDoc->m_pCurrentBrush->BrushMove(source, target);
			}
			break;
		case LEFT_MOUSE_UP:
			m_pDoc->m_pCurrentBrush->BrushEnd( source, target );

			SaveCurrentContent();
			RestoreContent();
			break;
		case RIGHT_MOUSE_DOWN:
			// cout << "brushType is " << m_pDoc->brushType << endl;
			// cout << "strokeDirection is " << m_pDoc->strokeDirection << endl;
			if (m_pDoc->brushType == BRUSH_LINES || m_pDoc->brushType == BRUSH_SCATTERED_LINES) {
				SaveCurrentContent();
				startPoint = target;
				// cout << "right mouse movement" << endl;
			}
			break;
		case RIGHT_MOUSE_DRAG:
			if (m_pDoc->brushType == BRUSH_LINES || m_pDoc->brushType == BRUSH_SCATTERED_LINES) {
				RestoreContent();
				glLineWidth(1.0f);
				glBegin(GL_LINE_STRIP);
				
					glColor3ub(255, 0, 0);
					glVertex3d(startPoint.x, startPoint.y, 0.0);
					glVertex3d(target.x, target.y, 0.0);
				glEnd();
			}
			break;
		case RIGHT_MOUSE_UP:
			if (m_pDoc->brushType == BRUSH_LINES || m_pDoc->brushType == BRUSH_SCATTERED_LINES) {
				RestoreContent();
				int lineLength = sqrt((target.x - startPoint.x) * (target.x - startPoint.x) + (target.y - startPoint.y) * (target.y - startPoint.y));
				m_pDoc->m_pUI->setSize(lineLength);
				if (m_pDoc->strokeDirection == 1) {
					int lineAngle = (int)(atan2((target.y - startPoint.y), (target.x - startPoint.x)) / M_PI * 180);
					if (lineAngle < 0)
						lineAngle += 360;
					m_pDoc->m_pUI->setLineAngle(lineAngle);
				}	
			}
			break;

		case AUTO_DRAW:

			for (int i = 0; i < xorder.size(); i++) {
				for (int j = 0; j < yorder.size(); j++) {
					randomDrawingOrder.push_back(Point(xorder[i], yorder[j]));
					
					
					


				}
			}
			std::random_shuffle(randomDrawingOrder.begin(), randomDrawingOrder.end());
			
			for (int i = 0; i < randomDrawingOrder.size(); i++) {
				Point source(randomDrawingOrder[i].x + m_nStartCol, m_nEndRow - randomDrawingOrder[i].y);
				Point target(randomDrawingOrder[i].x, m_nWindowHeight - randomDrawingOrder[i].y);
				
				if (m_pDoc->m_pUI->getSizeRandom())
					m_pDoc->m_pUI->setSize(rand() % sizeCap);
				m_pDoc->m_pCurrentBrush->BrushBegin(source, target);

			}
			//m_pDoc->m_pCurrentBrush->BrushBegin(Point(xorder[i] + m_nStartCol, m_nEndRow - yorder[j]), Point(xorder[i], m_nWindowHeight - yorder[j]));


			m_pDoc->m_pUI->setSize(sizeBeforeRandom);
			SaveCurrentContent();
			RestoreContent();

			break;
		case PAINTERLY:
			m_pDoc->m_pUI->setSize(painterlyRadius * 2);
			if (true) {
				//"borrow" ucbitmap to draw
				unsigned char* tempUcbitmap = m_pDoc->m_ucBitmap;
				m_pDoc->m_ucBitmap = painterlyRefimage;


				for (int i = 0; i < strokes.size(); i++) {
					//cout << strokes[i].x << "x y " << strokes[i].y << endl;
					m_pDoc->m_pCurrentBrush->BrushBegin(Point(strokes[i].x + m_nStartCol, strokes[i].y), Point(strokes[i].x, m_nWindowHeight - (m_nDrawHeight - strokes[i].y)));

				}

				//restore 
				m_pDoc->m_pUI->setSize(sizeBeforeRandom);
				m_pDoc->m_ucBitmap = tempUcbitmap;
				SaveCurrentContent();
				RestoreContent();
			}

			
			
			break;

		default:
			printf("Unknown event!!\n");		
			break;
		}
	}

	glFlush();

	#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_BACK);
	#endif // !MESA

}


int PaintView::handle(int event)
{

	switch(event)
	{
	
	case FL_ENTER:
	    redraw();
		break;
	case FL_PUSH:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_DOWN;
		else
			eventToDo=LEFT_MOUSE_DOWN;
		isAnEvent=1;	
		

		redraw();
		break;
	case FL_DRAG:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_DRAG;
		else
			eventToDo=LEFT_MOUSE_DRAG;
		isAnEvent=1;
		redraw();
		m_pDoc->m_pUI->m_origView->setCursorPosition(Point(coord.x, m_nWindowHeight - coord.y));
		break;
	case FL_RELEASE:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_UP;
		else
			eventToDo=LEFT_MOUSE_UP;
		isAnEvent=1;
		redraw();
		break;
	case FL_MOVE:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		m_pDoc->m_pUI->m_origView->setCursorPosition(Point(coord.x, m_nWindowHeight - coord.y));
		break;
	case FL_LEAVE:	//NO cursor when the mouse is not in the PaintView window.
		m_pDoc->m_pUI->m_origView->setCursorPosition(Point(-1,-1));
	default:
		return 0;
		break;

	}



	return 1;
}

void PaintView::refresh()
{
	redraw();
}

void PaintView::resizeWindow(int width, int height)
{
	resize(x(), y(), width, height);
}

void PaintView::SaveCurrentContent()
{
	// Tell openGL to read from the front buffer when capturing
	// out paint strokes
	glReadBuffer(GL_FRONT);

	glPixelStorei( GL_PACK_ALIGNMENT, 1 );
	glPixelStorei( GL_PACK_ROW_LENGTH, m_pDoc->m_nPaintWidth );
	
	glReadPixels( 0, 
				  m_nWindowHeight - m_nDrawHeight, 
				  m_nDrawWidth, 
				  m_nDrawHeight, 
				  GL_RGB, 
				  GL_UNSIGNED_BYTE, 
				  m_pPaintBitstart );
}


void PaintView::RestoreContent()
{
	glDrawBuffer(GL_BACK);

	glClear( GL_COLOR_BUFFER_BIT );

	glRasterPos2i( 0, m_nWindowHeight - m_nDrawHeight );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glPixelStorei( GL_UNPACK_ROW_LENGTH, m_pDoc->m_nPaintWidth );
	glDrawPixels( m_nDrawWidth, 
				  m_nDrawHeight, 
				  GL_RGB, 
				  GL_UNSIGNED_BYTE, 
				  m_pPaintBitstart);

//	glDrawBuffer(GL_FRONT);
}

int PaintView::getDrawHeight()
{
	return this->m_nDrawHeight;
}

int PaintView::getWindowHeight()
{
	return this->m_nWindowHeight;
}




void PaintView::startAutoPaint()
{
	isAnEvent = 1;
	eventToDo = AUTO_DRAW;
	redraw();
}

void PaintView::doPainterly()
{
	int threshold = m_pDoc->m_pUI->getPainterlyThreshold();
	int numLayers = m_pDoc->m_pUI->getPainterlyLayers();
	int r0level = m_pDoc->m_pUI->getPainterlyR0level();
	double blurFactor = m_pDoc->m_pUI->getPainterlyBlur();
	double gridFactor = m_pDoc->m_pUI->getPainterlyGridsize();
	int numBrushes = min(numLayers, r0level + 1);
	
	//determine the set of brush sizes
	std::vector<int> brushSizes;
	int twoPower = r0level;
	for (int i = 0; i < numBrushes; i++) {
		brushSizes.push_back(pow(2, twoPower));
		twoPower--;
	}

	//for (int i = 0; i < numBrushes; i++) {
		//cout << brushSizes[i] << "brushsize" << endl;
	//}

	//initialize the canvas to be the same as current m_ucPainting
	if (m_pDoc && m_pDoc->m_ucPainting && m_pDoc->m_ucBitmap) {
		unsigned char* canvas = new unsigned char[m_pDoc->m_nWidth*m_pDoc->m_nHeight * 3];
		memset(canvas, 255, m_pDoc->m_nWidth*m_pDoc->m_nHeight * 3);
		//memcpy(canvas, m_pDoc->m_ucPainting, m_pDoc->m_nWidth*m_pDoc->m_nHeight * 3);

		//loop thru all brush sizes
		for (int i = 0; i < brushSizes.size(); i++) {
			//fl_message("start a new round");
			int currentRadius = brushSizes[i];
			double gaussianSd = blurFactor*currentRadius;


			unsigned char* refImage = new unsigned char[m_pDoc->m_nWidth*m_pDoc->m_nHeight * 3];
			memset(refImage, 0, m_pDoc->m_nWidth*m_pDoc->m_nHeight * 3);

			double sigma = m_pDoc->m_pUI->getPainterlyBlur() * currentRadius;
			cout << currentRadius << endl;
			double gaussianCorner = (1 / (2 * 3.1416*sigma*sigma))*exp(-(2 / sigma*sigma));
			double gaussianEdge = (1 / (2 * 3.1416*sigma*sigma))*exp(-(1 / sigma*sigma));
			double gaussianCenter = (1 / (2 * 3.1416*sigma*sigma));
			

			double tempCorner = gaussianCorner/(gaussianCorner * 4 + gaussianEdge * 4 + gaussianCenter);
			double tempEdge = gaussianEdge/(gaussianCorner * 4 + gaussianEdge * 4 + gaussianCenter);
			double tempCenter = gaussianCenter/(gaussianCorner * 4 + gaussianEdge * 4 + gaussianCenter);
			gaussianCorner = tempCorner;
			gaussianEdge = tempEdge;
			gaussianCenter = tempCenter;
			cout << gaussianCorner << " " << gaussianEdge << " --" << gaussianCenter << endl;
			//double gaussianCorner = 0.0625;
			//double gaussianEdge = 0.125;
			//double gaussianCenter = 0.25;

			//loop thru every point in m_ucbitmap
			for (int y = 0; y < m_pDoc->m_nHeight; y++) {
				for (int x= 0; x < m_pDoc->m_nWidth; x++) {
					int xpos = x;
					int ypos = y;
					//the following two for loops scans thru the ref grid, right to left, top to bottom
					int newRed = 0;
					int newGreen = 0;
					int newBlue = 0;

					for (int yref = ypos - 1; yref <= ypos + 1; yref++) {
						for (int xref = xpos - 1; xref <= xpos + 1; xref++) {

								
							int samplex = xref;
							if (samplex < 0)
								samplex = 0;
							if (samplex > m_pDoc->m_nPaintWidth)
								samplex = m_pDoc->m_nPaintWidth - 1;
							int sampley = yref;
							if (sampley < 0)
								sampley = 0;
							if (sampley > m_pDoc->m_nPaintWidth)
								sampley = m_pDoc->m_nPaintHeight - 1;
								
							float refRed = (float)m_pDoc->m_ucBitmap[redbytePosition(samplex, sampley)];
							float refGreen = (float)m_pDoc->m_ucBitmap[greenbytePosition(samplex, sampley)];
							float refBlue = (float)m_pDoc->m_ucBitmap[bluebytePosition(samplex, sampley)];

							//cout << "refrgb is" << refRed << " " << refGreen << " " << refBlue << endl;
 							float refpointWeight = 0.0;
							if (abs(yref - ypos) + abs(xref - xpos) == 2)
								refpointWeight = gaussianCorner;
							else if (abs(yref - ypos) + abs(xref - xpos) == 1)
								refpointWeight = gaussianEdge;
							else
								refpointWeight = gaussianCenter;

								newRed += (int)(refpointWeight *refRed);
								newGreen += (int)(refpointWeight * refGreen);
								newBlue += (int)(refpointWeight * refBlue);

								//cout << "newrgb is now" << newRed << " " << newBlue << " " << newGreen << endl;

							}
						}
					//cout << (int)refImage[redbytePosition(xpos, ypos)] << endl;
					refImage[redbytePosition(xpos, ypos)] = newRed;
					refImage[greenbytePosition(xpos, ypos)] = newGreen;
					refImage[bluebytePosition(xpos, ypos)] = newBlue;
					//cout << (int)refImage[redbytePosition(xpos, ypos)] << "afterassigning" << endl;
				}
			}
			//cout << "refimage generated" << endl;
			paintLayer(canvas, refImage, currentRadius);
			m_pDoc->m_pUI->m_paintView->flush();
		}

		//fl_message("painterly completed");
	}
	else {
		fl_message("please load an image first");
	}

}

void PaintView::paintLayer(unsigned char * canvas, unsigned char * refImage, int radius)
{
	strokes.clear();
	//cout << "current radius is " << radius << endl;
	std::vector<double> difference;
	int imageSize = m_pDoc->m_nPaintWidth * m_pDoc->m_nPaintHeight;
	/*
	for (int i = 0; i < imageSize; i++) {
	int rdiff = (int)canvas[3 * i] - (int)refImage[3 * i];
	int gdiff = (int)canvas[3 * i+1] - (int)refImage[3 * i+2];
	int bdiff = (int)canvas[3 * i+2] - (int)refImage[3 * i+2];
	double diff = sqrt(rdiff*rdiff + gdiff*gdiff + bdiff*bdiff);
	difference.push_back(diff);
	}
	*/

	//check whether it's first round
	bool firstRound = (radius == pow(2, m_pDoc->m_pUI->getPainterlyR0level()));


	int gridSize = (int)(m_pDoc->m_pUI->getPainterlyGridsize() * (double)radius);

	//scan thru all the sub-regions
	for (int x = 0; x < m_pDoc->m_nPaintWidth; x += gridSize) {
		for (int y = 0; y < m_pDoc->m_nPaintHeight; y += gridSize) {
			double areaError = 0;

			//loop thru the region M and record the point with largest error
			int regionxStart = max(x - gridSize / 2, 0);
			int regionxEnd = min(m_pDoc->m_nPaintWidth, x + gridSize / 2);
			int regionyStart = max(y - gridSize / 2, 0);
			int regionyEnd = min(m_pDoc->m_nPaintHeight, y + gridSize / 2);
			//cout << "analyzing region " << regionxStart << " " << regionxEnd << " - " << regionyStart << " " << regionyEnd << endl;
			double largestColorDiff = 0.0;
			Point pointwithLargestError(0, 0);
			for (int regionx = regionxStart; regionx < regionxEnd; regionx++) {
				for (int regiony = regionyStart; regiony < regionyEnd; regiony++) {
					int whichPixel = regiony*m_pDoc->m_nPaintWidth + regionx;
					int rdiff = (int)(canvas[3*whichPixel]) - (int)(refImage[3*whichPixel]);
					int gdiff = (int)(canvas[3*whichPixel+1]) - (int)(refImage[3*whichPixel+1]);
					int bdiff = (int)(canvas[3*whichPixel+2]) - (int)(refImage[3*whichPixel+2]);
					double pointColorDiff = sqrt(rdiff*rdiff + gdiff*gdiff + bdiff*bdiff);
					areaError += pointColorDiff;
					if (pointColorDiff > largestColorDiff) {
						pointwithLargestError.x = regionx;
						pointwithLargestError.y = regiony;
						largestColorDiff = pointColorDiff;
					}
				}
			}
			areaError /= (regionyEnd - regionyStart)*(regionxEnd - regionxStart);
			//cout << "areaError is " << areaError << endl;
			//cout << "threshold is" << m_pDoc->m_pUI->getPainterlyThreshold() << endl;
			//in the first round, any distance is considered above threshold.
			if (firstRound) {
				strokes.push_back(pointwithLargestError);
			}
			else if (areaError > m_pDoc->m_pUI->getPainterlyThreshold()) {
				strokes.push_back(pointwithLargestError);
			}

		}
	}

	std::random_shuffle (strokes.begin(), strokes.end());
	painterlyRadius = radius;
	painterlyRefimage = refImage;

	isAnEvent = 1;
	eventToDo = PAINTERLY;
	redraw();

	//update canvas
	delete[] canvas;
	canvas = new unsigned char[3 * imageSize];
	memcpy(canvas, m_pDoc->m_ucPainting, 3 * imageSize);
	//canvas = m_pDoc->m_ucPainting;



	//delete[]m_pDoc->m_ucPainting;
	//m_pDoc->m_ucPainting = canvas;
	//redraw();

		
}

int PaintView::redbytePosition(int x, int y)
{
	return  (y*m_pDoc->m_nWidth + x) * 3;
}

int PaintView::greenbytePosition(int x, int y)
{
	return  (y*m_pDoc->m_nWidth + x) * 3 +1;
}

int PaintView::bluebytePosition(int x, int y)
{
	return  (y*m_pDoc->m_nWidth + x) * 3+2;
}

