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
	int getGreyscale(int x, int y);
	int getAnotherGreyscale(const Point source);

	int getGradientAngle(const Point source);
	int getAnotherGradientAngle(const Point source);

	void getEdgeMapAt(int threshold);
	int getGradient(int x, int y);

private:
	ImpressionistDoc*	m_pDoc;
	
};

#endif
