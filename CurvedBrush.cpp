//
// CurvedBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "CurvedBrush.h"
#include <math.h>
#include <iostream>

extern float frand();

CurvedBrush::CurvedBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void CurvedBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	BrushMove(source, target);
}

void CurvedBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;


	if (pDoc == NULL) {
		printf("CurvedBrush::BrushMove  document is NULL\n");
		return;
	}

	int size = pDoc->getSize(); // get the size from UI size
	double radius = pDoc->getSize() / 2;

	glBegin(GL_POLYGON);
	SetColor(source);
	for (double i = 0; i < 2 * M_PI; i += M_PI / 36) {
		int xToDraw = (int)(cos(i)*radius + target.x);
		int xUpperBound = pDoc->m_nPaintWidth;
		int yToDraw = (int)(sin(i)*radius + target.y);
		int yLowerBound = pDoc->m_pUI->m_paintView->getWindowHeight() - pDoc->m_nPaintHeight;

		//if x is to large or y is too small, then mark the polygon vertex on the nearest boundary point.
		glVertex2i(min(xToDraw, xUpperBound), max(yToDraw, yLowerBound));
	}
	glEnd();



	int k = 1;
	int lastDirection;
	Point lastSource = source;
	Point lastTarget = target;
	
	while (k < size) {
		int gradientAngle = pDoc->getGradientAngle(lastSource);
		int currentDirection = gradientAngle + 90;

		if ((k != 1) && (abs(currentDirection - lastDirection) > 90)) {
			currentDirection = gradientAngle - 90;
		}

		double directionAngleRadius = currentDirection * M_PI / 180;
		Point currentSource, currentTarget;
		currentSource.x = round((size / 2) * cos(directionAngleRadius) + lastSource.x);
		currentSource.y = round((size / 2) * sin(directionAngleRadius) + lastSource.y);
		currentTarget.x = round((size / 2) * cos(directionAngleRadius) + lastTarget.x);
		currentTarget.y = round((size / 2) * sin(directionAngleRadius) + lastTarget.y);
		
		
		if (abs(pDoc->getGreyscale(source) - pDoc->getGreyscale(lastSource)) > abs(pDoc->getGreyscale(source) - pDoc->getGreyscale(currentSource)))
		{
			break;
		}

		glBegin(GL_POLYGON);
		SetColor(source);
		for (double i = 0; i < 2 * M_PI; i += M_PI / 36) {
			int xToDraw = (int)(cos(i)*radius + currentTarget.x);
			int xUpperBound = pDoc->m_nPaintWidth;
			int yToDraw = (int)(sin(i)*radius + currentTarget.y);
			int yLowerBound = pDoc->m_pUI->m_paintView->getWindowHeight() - pDoc->m_nPaintHeight;

			//if x is to large or y is too small, then mark the polygon vertex on the nearest boundary point.
			glVertex2i(min(xToDraw, xUpperBound), max(yToDraw, yLowerBound));
		}
		glEnd();

		lastDirection = currentDirection;
		lastSource = currentSource;
		lastTarget = currentTarget;
	}

}

void CurvedBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}