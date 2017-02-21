//
// SobelFilter.cpp
//
// The implementation of the sobel filter
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "SobelFilter.h"
#include <math.h>
#include <iostream>
using namespace std;

SobelFilter::SobelFilter(ImpressionistDoc*	pDoc) :
	m_pDoc(pDoc)
{
}

//---------------------------------------------------
// Return m_pDoc, which connects the UI and brushes
//---------------------------------------------------
ImpressionistDoc* SobelFilter::GetDocument(void)
{
	return m_pDoc;
}

int SobelFilter::getGreyscale(const Point source)
{
	ImpressionistDoc* pDoc = GetDocument();
	GLubyte color[3];
	memcpy(color, pDoc->GetOriginalPixel(source), 3);
	return color[0] * 0.299 + color[1] * 0.587 + color[2] * 0.114;
}

int SobelFilter::getGreyscale(int x, int y)
{
	ImpressionistDoc* pDoc = GetDocument();
	GLubyte color[3];
	memcpy(color, pDoc->GetOriginalPixel(x,y), 3);
	return color[0] * 0.299 + color[1] * 0.587 + color[2] * 0.114;
}

int SobelFilter::getAnotherGreyscale(const Point source)
{
	ImpressionistDoc* pDoc = GetDocument();
	GLubyte color[3];
	memcpy(color, pDoc->GetAnotherPixel(source), 3);
	return color[0] * 0.299 + color[1] * 0.587 + color[2] * 0.114;
}


// Get the gradient angle of a particular point from original image 
int SobelFilter::getGradientAngle(const Point source) {
	//  2 5 8
	//  1 4 7
	//  0 3 6
	int intensity[9];
	int k = 0;
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			Point p = source;
			p.x += i;
			p.y += j;
			intensity[k] = getGreyscale(p);
			k++;
		}
	}

	int gradientX = - intensity[0] - 2 * intensity[1] - intensity[2] + intensity[6] + 2 * intensity[7] + intensity[8];
	int gradientY = - intensity[0] - 2 * intensity[3] - intensity[6] + intensity[2] + 2 * intensity[5] + intensity[8];

	if (gradientX == 0)
		return 90;
    return (int)(atan2(gradientY,gradientX) / M_PI * 180);
}

// Get the gradient angle of a particular point from another image 
int SobelFilter::getAnotherGradientAngle(const Point source) {
	//  2 5 8
	//  1 4 7
	//  0 3 6
	int intensity[9];
	int k = 0;
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			Point p = source;
			p.x += i;
			p.y += j;
			intensity[k] = getAnotherGreyscale(p);
			k++;
		}
	}

	int gradientX = -intensity[0] - 2 * intensity[1] - intensity[2] + intensity[6] + 2 * intensity[7] + intensity[8];
	int gradientY = -intensity[0] - 2 * intensity[3] - intensity[6] + intensity[2] + 2 * intensity[5] + intensity[8];

	if (gradientX == 0)
		return 90;
	return (int)(atan2(gradientY, gradientX) / M_PI * 180);
}

void SobelFilter::getEdgeMapAt(int threshold) {
	int width = m_pDoc->m_nWidth;
	int height = m_pDoc->m_nHeight;

	unsigned char*	data = new unsigned char[width*height * 3];

	cout << "edge" << endl;

	for (int x = 0; x < width; x++) 
	{
		for (int y = 0; y < height; y++) 
		{
			if (getGradient(x, y) > threshold) {
				data[y*width + x] = 255;
				data[y*width + x+1] = 255;
				data[y*width + x+2] = 255;
			}
			else {
				data[y*width + x] = 0;
				data[y*width + x + 1] = 0;
				data[y*width + x + 2] = 0;
			}
		}
	}	

	if (m_pDoc->m_ucEdgeMap) delete[] m_pDoc->m_ucEdgeMap;
	m_pDoc->m_ucEdgeMap = data;
	m_pDoc->m_ucBitmap = m_pDoc->m_ucEdgeMap;
}

int SobelFilter::getGradient(int x, int y) {
	//  2 5 8
	//  1 4 7
	//  0 3 6
	int intensity[9];
	int k = 0;
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			intensity[k] = getGreyscale(x+i, y+j);
			k++;
		}
	}

	int gradientX = -intensity[0] - 2 * intensity[1] - intensity[2] + intensity[6] + 2 * intensity[7] + intensity[8];
	int gradientY = -intensity[0] - 2 * intensity[3] - intensity[6] + intensity[2] + 2 * intensity[5] + intensity[8];

	return (int)(sqrt(gradientX * gradientX + gradientY * gradientY));
}