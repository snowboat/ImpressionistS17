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
#include <math.h>
using namespace std;


#define LEFT_MOUSE_DOWN		1
#define LEFT_MOUSE_DRAG		2
#define LEFT_MOUSE_UP		3
#define RIGHT_MOUSE_DOWN	4
#define RIGHT_MOUSE_DRAG	5
#define RIGHT_MOUSE_UP		6


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
	std::cout << "paintview is " << w << "and" << h << std::endl;
	
	
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
