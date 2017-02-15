//
// BlackandwhiteBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "BlackandwhiteBrush.h"
#include "math.h"
#include <iostream>

extern float frand();

BlackandwhiteBrush::BlackandwhiteBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void BlackandwhiteBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize();



	glPointSize((float)size);

	BrushMove(source, target);
}

void BlackandwhiteBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;


	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}
	int size = pDoc->getSize();
	glPointSize(1.0);
	glBegin(GL_POINTS);

	for (int i = -size / 2; i < size / 2; i++) {
		for (int j = -size / 2; j < size / 2; j++) {
			Point replacedPoint(source.x + i, source.y + j);
			int greyscale = (GetColor(replacedPoint)[0] * 0.299 + GetColor(replacedPoint)[1] * 0.587 + GetColor(replacedPoint)[2] * 0.114);
			GLubyte newColor[4];
			for (int i = 0; i < 3; i++) {
				newColor[i] = (GLubyte)greyscale;
			}
			glColor4ubv(newColor);
			
			newColor[3] = (GLubyte)(int)(pDoc->getAlpha() * 255);	//alpha channel
			glVertex2i(target.x + i, target.y + j);
		}
	}

	glEnd();
	glFlush();
}

void BlackandwhiteBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}
