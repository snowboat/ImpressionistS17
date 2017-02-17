// 
// impressionistDoc.h
//
// header file for Doc 
//

#ifndef ImpressionistDoc_h
#define ImpressionistDoc_h

#include "impressionist.h"
#include "bitmap.h"

class ImpressionistUI;

class ImpressionistDoc
{
public:
	ImpressionistDoc();

	void	setUI(ImpressionistUI* ui);		// Assign the UI to use

	int		loadImage(char *iname);			// called by the UI to load image
	int		saveImage(char *iname);			// called by the UI to save image


	int     clearCanvas();                  // called by the UI to clear the drawing canvas
	void	setBrushType(int type);			// called by the UI to set the brushType
	int		getSize();						// get the UI size
	void	setSize(int size);				// set the UI size
	int     getLineWidth();
	void    setLineWidth(int lineWidth);
	int     getLineAngle();
	void    setLineAngle(int lineAngle);
	double 	getAlpha();					//return the alpha as double for setcolor() to use
	void	setAlpha(float alpha);
	char*	getImageName();					// get the current image name
	void 	setStrokeDirection(int type);

	//apply the color manipulation on original image
	void applyManipulation();

	// Attributes
public:
	// Dimensions of original window.
	int				m_nWidth,
					m_nHeight;

	// Dimensions of the paint window.
	int				m_nPaintWidth,
					m_nPaintHeight;
	// Bitmaps for original image and painting.
	unsigned char*	m_ucBitmap;//original image
	unsigned char*  backupBitmap;
	unsigned char*	m_ucPainting;//painting image


	// The current active brush.
	ImpBrush*			m_pCurrentBrush;
	// Size of the brush.
	int m_nSize;
	// Current Stroke Direction
	int strokeDirection;		//TODO: actually I think it may not need to be recorded as an integer. but let's keep it for now.					
	ImpressionistUI*	m_pUI;

	// Operations
public:
	// Get the color of the original picture at the specified coord
	GLubyte* GetOriginalPixel(int x, int y);
	// Get the color of the original picture at the specified point	
	GLubyte* GetOriginalPixel(const Point p);


private:
	char			m_imageName[256];

};

extern void MessageBox(char *message);

#endif
