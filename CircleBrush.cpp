//
// CircleBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "CircleBrush.h"
#include <math.h>

extern float frand();

CircleBrush::CircleBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void CircleBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize();



	glPointSize((float)size);	

	BrushMove(source, target);
}

void CircleBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("CircleBrush::BrushMove  document is NULL\n");
		return;
	}

	glBegin(GL_POLYGON);
	SetColor(source);
	double radius =  pDoc->getSize()/2;
	for (double i = 0; i < 2 * M_PI; i += M_PI / 36) {
		int xToDraw = (int)(cos(i)*radius + target.x);
		int xUpperBound = pDoc->m_nPaintWidth;
		int yToDraw = (int)(sin(i)*radius + target.y);
		int yLowerBound = pDoc->m_pUI->m_paintView->getWindowHeight() - pDoc->m_nPaintHeight;
		
		//if x is to large or y is too small, then mark the polygon vertex on the nearest boundary point.
		glVertex2i(min(xToDraw,xUpperBound), max(yToDraw,yLowerBound));
	}
	glEnd();
}

void CircleBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}
