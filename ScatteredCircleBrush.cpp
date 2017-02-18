//
// ScatteredCircleBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ScatteredCircleBrush.h"
#include "math.h"
#include <iostream>
using namespace std;

extern float frand();

ScatteredCircleBrush::ScatteredCircleBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void ScatteredCircleBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize();



	glPointSize((float)size);

	BrushMove(source, target);
}

void ScatteredCircleBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("ScatteredCircleBrush::BrushMove  document is NULL\n");
		return;
	}

	double radius = pDoc->getSize() / 2;

	//Draw the surrounding circles
	int maxdist = 2 * radius;
	int t = rand() % 2 + 3; // generate 3 or 4 circles

	for (int i = 0; i < t; i++) {
		double dist = rand() % maxdist;
		double angle = frand() * 2 * M_PI;
		Point circleCtr1(target.x + dist*cos(angle), target.y + dist*sin(angle));
		Point circleCtr1Source(source.x + dist*cos(angle), source.y + dist*sin(angle));
		glBegin(GL_POLYGON);
		SetColor(circleCtr1Source);
		for (double i = 0; i < 2 * M_PI; i += M_PI / 36) {
			int xToDraw = (int)(cos(i)*radius + circleCtr1.x);
			int xUpperBound = pDoc->m_nPaintWidth;
			int yToDraw = (int)(sin(i)*radius + circleCtr1.y);
			int yLowerBound = pDoc->m_pUI->m_paintView->getWindowHeight() - pDoc->m_nPaintHeight;
			if ( xToDraw<= xUpperBound && yToDraw >= yLowerBound ) {
				glVertex2i(xToDraw, yToDraw);
			}
		}
		glEnd();
	}
	
	glFlush();
	
}

void ScatteredCircleBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}
