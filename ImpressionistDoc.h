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
	void    setFilter(SobelFilter* filter); // Assign the filter to use

	int		loadImage(char *iname);			// called by the UI to load image
	int		saveImage(char *iname);			// called by the UI to save image
	int		loadDissolveImage(char* iname);	// load the dissolve image (1 bell)
	int		loadMuralImage(char* iname);	// load the mural image (1.5 bells)
	int		loadAlphaMappedImage(char* iname);	// load the alpha image, then store its alpha values into m_alphaMappedValues
	int     loadAnotherImage(char* iname); // load another image for gradient painting (1.5 bells)


	int     clearCanvas();                  // called by the UI to clear the drawing canvas
	void	setBrushType(int type);			// called by the UI to set the brushType
	int		getSize();						// get the UI size
	int     getLineWidth();
	int     getLineAngle();
	double 	getAlpha();					// return the alpha as double for setcolor() to use
	int     getEdgeThreshold();
	char*	getImageName();					// get the current image name
	void 	setStrokeDirection(int type);
	int     getStrokeDirection();
	bool    getFlagOfEdgeClipping();
	bool    getFlagOfAnotherGradient();

	//apply the color manipulation on original image
	void applyManipulation();

	//undo
	void undo();

	// Attributes
public:
	// Dimensions of original window.
	int				m_nWidth,
					m_nHeight;

	// Dimensions of the paint window.
	int				m_nPaintWidth,
					m_nPaintHeight;
	
	//dimensions of the alpha image
	int				m_alphaWidth, m_alphaHeight;

	// Bitmaps for original image and painting.
	unsigned char*	m_ucBitmap;//original image
	unsigned char*  backupBitmap;	//immutable bitmap which records the original stagte of left image
	unsigned char*	m_ucPainting;//painting image
	unsigned char*  m_undoImage;
	unsigned char*  m_alphaMappedValues; //the alpha value of each pixel in the alphamapped brush (in 0-255 scale)

	unsigned char*  m_ucEdgeMap;
	unsigned char*  m_ucAnotherBitmap;

	// The current active brush.
	ImpBrush*			m_pCurrentBrush;
	// Size of the brush.
	int m_nSize;
	// Current Stroke Direction
	int strokeDirection;	
	int brushType;

	ImpressionistUI*	m_pUI;

	// set the filter to use
	SobelFilter* m_sobelFilter;

	// Operations
public:
	// Get the color of the original picture at the specified coord
	GLubyte* GetOriginalPixel(int x, int y);
	// Get the color of the original picture at the specified point	
	GLubyte* GetOriginalPixel(const Point p);
	// Get the color of the another picture at the specified coord
	GLubyte* GetAnotherPixel(int x, int y);
	// Get the color of the another picture at the specified point	
	GLubyte* GetAnotherPixel(const Point p);


private:
	char			m_imageName[256];

};

extern void MessageBox(char *message);

#endif
