//
// SobelFilter.h
//
// The header file for the sobel filter
//

#ifndef SOBELFILTER_h
#define SOBELFILTER_h

class ImpressionistDoc; // Pre-declaring class

class SobelFilter
{
public:

    SobelFilter(ImpressionistDoc* pDoc);

	// get Doc to communicate with it
	ImpressionistDoc* GetDocument(void);

	int getGreyscale(const Point source);
	int getAnotherGreyscale(const Point source);

	int getGradientAngle(const Point source);
	int getAnotherGradientAngle(const Point source);


private:
	ImpressionistDoc*	m_pDoc;
	
};

#endif
