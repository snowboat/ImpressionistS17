//
// SaturationBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "SaturationBrush.h"
#include "math.h"
#include <iostream>

extern float frand();

SaturationBrush::SaturationBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void SaturationBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize();



	glPointSize((float)size);

	BrushMove(source, target);
}

void SaturationBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;


	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}
	if (withinBoundary(source)) {
		std::cout << "within boundary" << std::endl;
		glBegin(GL_POINTS);
		SetColor(source);
		std::cout << GetColor(source)[0] << " " << GetColor(source)[1] << " " << GetColor(source)[2] << std::endl;
		int greyscale = (GetColor(source)[0]*0.299 + GetColor(source)[1]*0.587 + GetColor(source)[2]*0.114);
		int saturatePixel[4];
		double alpha = -10;
		for (int i = 0; i < 3; i++) {
			saturatePixel[i] = alpha*GetColor(source)[i] + (1.0 - alpha)*greyscale;
		}
		saturatePixel[3] = (int)(pDoc->getAlpha() * 255);
		glColor4iv(saturatePixel);
		std::cout << saturatePixel[0] << "and" << saturatePixel[3];
		SetColor(source);
		glVertex2d(target.x, target.y);

		glEnd();
		glFlush();
	}
}

void SaturationBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}
