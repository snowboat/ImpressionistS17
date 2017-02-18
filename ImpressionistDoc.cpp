// 
// impressionistDoc.cpp
//
// It basically maintain the bitmap for answering the color query from the brush.
// It also acts as the bridge between brushes and UI (including views)
//

#include <FL/fl_ask.H>

#include "impressionistDoc.h"
#include "impressionistUI.h"

#include "ImpBrush.h"
#include <iostream>
using namespace std;

// Include individual brush headers here.
#include "PointBrush.h"
#include "LineBrush.h"
#include "CircleBrush.h"
#include "ScatteredCircleBrush.h"
#include "ScatteredLineBrush.h"
#include "ScatteredPointBrush.h"
#include "SaturationBrush.h"
#include "BlackandwhiteBrush.h"
#include "BlurringBrush.h"
#include "SharpeningBrush.h"

#define DESTROY(p)	{  if ((p)!=NULL) {delete [] p; p=NULL; } }

ImpressionistDoc::ImpressionistDoc()
{
	// Set NULL image name as init. 
	m_imageName[0] = '\0';

	m_nWidth = -1;
	m_ucBitmap = NULL;
	backupBitmap = NULL;
	m_ucPainting = NULL;
	m_undoImage = NULL;
	m_ucEdgeMap = NULL;
	m_ucAnotherBitmap = NULL;
	strokeDirection = 1;
	brushType = BRUSH_POINTS;


	// create one instance of each brush
	ImpBrush::c_nBrushCount = NUM_BRUSH_TYPE;
	ImpBrush::c_pBrushes = new ImpBrush*[ImpBrush::c_nBrushCount];

	ImpBrush::c_pBrushes[BRUSH_POINTS] = new PointBrush(this, "Points");

	//On startup, construct these brushes right now
	// Note: You should implement these 5 brushes.  They are set the same (PointBrush) for now
	ImpBrush::c_pBrushes[BRUSH_LINES]
		= new LineBrush(this, "Lines");
	ImpBrush::c_pBrushes[BRUSH_CIRCLES]
		= new CircleBrush(this, "Circles");
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_POINTS]
		= new ScatteredPointBrush(this, "Scattered Points");
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_LINES]
		= new ScatteredLineBrush(this, "Scattered Lines");
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_CIRCLES]
		= new ScatteredCircleBrush(this, "Scattered Circles");
	ImpBrush::c_pBrushes[BRUSH_SATURATION_BRUSH]
		= new SaturationBrush(this, "Saturation Brush");
	ImpBrush::c_pBrushes[BRUSH_BLACKANDWHITE_BRUSH]
		= new BlackandwhiteBrush(this, "Blackandwhite Brush");
	ImpBrush::c_pBrushes[BRUSH_BLURRING]
		= new BlurringBrush(this, "Bluring Brush");
	ImpBrush::c_pBrushes[BRUSH_SHARPENING]
		= new SharpeningBrush(this, "Sharpening Brush");

	// make one of the brushes current
	m_pCurrentBrush = ImpBrush::c_pBrushes[0];

	m_sobelFilter = new SobelFilter(this);

}


//---------------------------------------------------------
// Set the current UI 
//---------------------------------------------------------
void ImpressionistDoc::setUI(ImpressionistUI* ui)
{
	m_pUI = ui;
}

//---------------------------------------------------------
// Set the current Filter 
//---------------------------------------------------------
void ImpressionistDoc::setFilter(SobelFilter* filter)
{
	m_sobelFilter = filter;
}

//---------------------------------------------------------
// Returns the active picture/painting name
//---------------------------------------------------------
char* ImpressionistDoc::getImageName()
{
	return m_imageName;
}

//---------------------------------------------------------
// Called by the UI when the user changes the brush type.
// type: one of the defined brush types.
//---------------------------------------------------------
void ImpressionistDoc::setBrushType(int type)
{
	m_pCurrentBrush = ImpBrush::c_pBrushes[type];
	brushType = type;

	//upon change of brush type, activate or deactivate the line-related sliders
	if (type == BRUSH_LINES || type == BRUSH_SCATTERED_LINES) {
		m_pUI->m_strokeDirectionChoice->activate();
		m_pUI->m_LineAngleSlider->activate();
		m_pUI->m_LineWidthSlider->activate();
	}
	else {
		m_pUI->m_strokeDirectionChoice->deactivate();
		m_pUI->m_LineAngleSlider->deactivate();
		m_pUI->m_LineWidthSlider->deactivate();
	}
}

//change the stroke direction
void ImpressionistDoc::setStrokeDirection(int type) {
	strokeDirection = type;
}

//get the stroke direction
int ImpressionistDoc::getStrokeDirection() {
	return strokeDirection;
}

//apply the current color manipulation on original image
void ImpressionistDoc::applyManipulation()
{
	float redScale = m_pUI->getRed();
	float greenScale = m_pUI->getGreen();
	float blueScale = m_pUI->getBlue();

	if (backupBitmap) {
		memcpy(m_ucBitmap, backupBitmap, m_nPaintWidth*m_nPaintHeight * 3);
		for (int i = 0; i < m_nPaintWidth*m_nPaintHeight; i++) {
			m_ucBitmap[3 * i + 0] *= redScale;
			m_ucBitmap[3 * i + 1] *= greenScale;
			m_ucBitmap[3 * i + 2] *= blueScale;
		}
		m_pUI->m_origView->refresh();
	}

}

void ImpressionistDoc::undo()
{
	if (m_undoImage && m_undoImage != m_ucPainting) {
		delete[] m_ucPainting;
		m_ucPainting = new unsigned char[m_nPaintWidth*m_nPaintHeight * 3];
		memcpy(m_ucPainting, m_undoImage, m_nPaintWidth*m_nPaintHeight * 3);

		m_pUI->m_paintView->refresh();
	}
}

//---------------------------------------------------------
// Returns the size of the brush.
//---------------------------------------------------------
int ImpressionistDoc::getSize()
{
	return m_pUI->getSize();
}

//---------------------------------------------------------
// Returns the line width.
//---------------------------------------------------------
int ImpressionistDoc::getLineWidth()
{
	return m_pUI->getLineWidth();
}

//---------------------------------------------------------
// Returns the line angle.
//---------------------------------------------------------
int ImpressionistDoc::getLineAngle()
{
	return m_pUI->getLineAngle();
}

//---------------------------------------------------------
// Returns the alpha value.
//---------------------------------------------------------
double ImpressionistDoc::getAlpha()
{
	return  m_pUI->getAlpha();
}

//---------------------------------------------------------
// Load the specified image
// This is called by the UI when the load image button is 
// pressed.
//---------------------------------------------------------
int ImpressionistDoc::loadImage(char *iname)
{
	// try to open the image to read
	unsigned char*	data;
	int				width,height;

	if ((data = readBMP(iname, width, height)) == NULL)
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	// reflect the fact of loading the new image
	m_nWidth = width;
	m_nPaintWidth = width;
	m_nHeight = height;
	m_nPaintHeight = height;

	// release old storage
	if (m_ucBitmap) delete[] m_ucBitmap;
	if (m_ucPainting) delete[] m_ucPainting;
	if (backupBitmap) {
		delete[] backupBitmap;
		backupBitmap = NULL;
	}
	if (m_undoImage) {
		delete[] m_undoImage;
		m_undoImage = NULL;
	}


	m_ucBitmap = data;
	//do deep copy to initialize backupbitmap
	backupBitmap = new unsigned char[width*height * 3];
	memcpy(backupBitmap, m_ucBitmap, width*height * 3);

	// allocate space for draw view
	m_ucPainting = new unsigned char[width*height * 3];
	memset(m_ucPainting, 0, width*height * 3);

	m_pUI->m_mainWindow->resize(m_pUI->m_mainWindow->x(),
		m_pUI->m_mainWindow->y(),
		width * 2,
		height + 25);

	// display it on origView
	m_pUI->m_origView->resizeWindow(width, height);
	m_pUI->m_origView->refresh();

	// refresh paint view as well
	m_pUI->m_paintView->resizeWindow(width, height);
	m_pUI->m_paintView->refresh();


	return 1;
}


//----------------------------------------------------------------
// Save the specified image
// This is called by the UI when the save image menu button is 
// pressed.
//----------------------------------------------------------------
int ImpressionistDoc::saveImage(char *iname)
{

	writeBMP(iname, m_nPaintWidth, m_nPaintHeight, m_ucPainting);

	return 1;
}

int ImpressionistDoc::loadDissolveImage(char * iname)
{
	// try to open the image to read
	unsigned char*	data;	//data is the new loaded image
	int				width, height;	//width, height are the attributes of the newly loaded image

	if ((data = readBMP(iname, width, height)) == NULL)
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	// reflect the fact of loading the new image
	//m_nWidth = width;
	//m_nPaintWidth = width;
	//m_nHeight = height;
	//m_nPaintHeight = height;
	if (width != m_nPaintWidth || height != m_nPaintHeight) {
		fl_alert("Dimensions of dissolved image not correct!!");
		return 0;
	}

	// release old storage
	if (backupBitmap) {
		delete[] backupBitmap;
		backupBitmap = NULL;
	}
	if (m_undoImage) {
		delete[] m_undoImage;
		m_undoImage = NULL;
	}




	//update m_ucBitmap
	int imageSize = 3*m_nPaintWidth*m_nPaintHeight;
	for (int i = 0; i < imageSize; i++) {
		m_ucBitmap[i] = (unsigned char)((int)m_ucBitmap[i] * 0.5 + (int)data[i] * 0.5);
	}

	//do deep copy to initialize backupbitmap
	backupBitmap = new unsigned char[width*height * 3];
	memcpy(backupBitmap, m_ucBitmap, width*height * 3);

	// allocate space for draw view
	//m_ucPainting = new unsigned char[width*height * 3];
	//memset(m_ucPainting, 0, width*height * 3);

	m_pUI->m_mainWindow->resize(m_pUI->m_mainWindow->x(),
		m_pUI->m_mainWindow->y(),
		width * 2,
		height + 25);

	// display it on origView
	m_pUI->m_origView->resizeWindow(width, height);
	m_pUI->m_origView->refresh();

	// refresh paint view as well
	m_pUI->m_paintView->resizeWindow(width, height);
	m_pUI->m_paintView->refresh();


	return 1;
}

int ImpressionistDoc::loadMuralImage(char * iname)
{
	// try to open the image to read
	unsigned char*	data;	//data is the new loaded image
	int				width, height;	//width, height are the attributes of the newly loaded image

	if ((data = readBMP(iname, width, height)) == NULL)
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	// reflect the fact of loading the new image
	//m_nWidth = width;
	//m_nPaintWidth = width;
	//m_nHeight = height;
	//m_nPaintHeight = height;
	if (width != m_nPaintWidth || height != m_nPaintHeight) {
		fl_alert("Dimensions of mural image not correct!!");
		return 0;
	}

	// release old storage
	if (backupBitmap) {
		delete[] backupBitmap;
		backupBitmap = NULL;
	}
	if (m_undoImage) {
		delete[] m_undoImage;
		m_undoImage = NULL;
	}




	////update m_ucBitmap
	m_ucBitmap = data;

	//do deep copy to initialize backupbitmap
	backupBitmap = new unsigned char[width*height * 3];
	memcpy(backupBitmap, m_ucBitmap, width*height * 3);

	//m_nPainting is not changed

	m_pUI->m_mainWindow->resize(m_pUI->m_mainWindow->x(),
		m_pUI->m_mainWindow->y(),
		width * 2,
		height + 25);

	// display it on origView
	m_pUI->m_origView->resizeWindow(width, height);
	m_pUI->m_origView->refresh();

	// refresh paint view as well
	m_pUI->m_paintView->resizeWindow(width, height);
	m_pUI->m_paintView->refresh();


	return 1;

}

//----------------------------------------------------------------
// Clear the drawing canvas
// This is called by the UI when the clear canvas menu item is 
// chosen
//-----------------------------------------------------------------
int ImpressionistDoc::clearCanvas()
{

	// Release old storage
	if (m_ucPainting)
	{
		delete[] m_ucPainting;

		// allocate space for draw view
		m_ucPainting = new unsigned char[m_nPaintWidth*m_nPaintHeight * 3];
		memset(m_ucPainting, 0, m_nPaintWidth*m_nPaintHeight * 3);

		// refresh paint view as well	
		m_pUI->m_paintView->refresh();
	}

	return 0;
}

//------------------------------------------------------------------
// Get the color of the pixel in the original image at coord x and y
//------------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel(int x, int y)
{
	if (x < 0)
		x = 0;
	else if (x >= m_nWidth)
		x = m_nWidth - 1;

	if (y < 0)
		y = 0;
	else if (y >= m_nHeight)
		y = m_nHeight - 1;

	return (GLubyte*)(m_ucBitmap + 3 * (y*m_nWidth + x));
}

//----------------------------------------------------------------
// Get the color of the pixel in the original image at point p
//----------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel(const Point p)
{
	return GetOriginalPixel(p.x, p.y);
}

