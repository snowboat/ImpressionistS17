//
// SharpeningBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "SharpeningBrush.h"
#include <iostream>
using namespace std;

extern float frand();

SharpeningBrush::SharpeningBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void SharpeningBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize();



	glPointSize((float)size);

	BrushMove(source, target);

}

void SharpeningBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;


	if (pDoc == NULL) {
		printf("SharpeningBrush::BrushMove  document is NULL\n");
		return;
	}

	int size = pDoc->getSize();
	glPointSize(1.0);
	glBegin(GL_POINTS);
	double sharpeningFilter[3][3] = {
		{ 0.0,-1.0,0.0 },
		{ -1.0,5.0,-1.0 },
		{ 0.0,-1.0,0.0 }
		//{ 0.0625,0.125,0.0625 },
		//{ 0.125,0.25,0.125 },
		//{ 0.0625,0.125,0.0625 }

	};

	for (int i = -size / 2; i < size / 2; i++) {
		for (int j = -size / 2; j < size / 2; j++) {
			//(xpos,ypos) is the coordinates of the point BEING PROCESSED
			int xpos = source.x + i;
			int ypos = source.y + j;

			//point not on boundary
			if (xpos >= 1 && xpos < pDoc->m_nPaintWidth && ypos >= 1 && ypos < pDoc->m_nPaintHeight) {
				int newRed = 0;
				int newGreen = 0;
				int newBlue = 0;
				//(xref,yref) is one of the ref points in the 3*3 matrix
				for (int xref = xpos - 1; xref <= xpos + 1; xref++) {
					for (int yref = ypos - 1; yref <= ypos + 1; yref++) {
						int* refpointRgb = GetColor(Point(xref, yref));
						newRed += (int)((double)refpointRgb[0] * sharpeningFilter[xref - xpos + 1][yref - ypos + 1]);
						//newGreen += (int)((double)refpointRgb[1]/(double)9);
						newGreen += (int)((double)refpointRgb[1] * sharpeningFilter[xref - xpos + 1][yref - ypos + 1]);
						newBlue += (int)((double)refpointRgb[2] * sharpeningFilter[xref - xpos + 1][yref - ypos + 1]);
						//newBlue += (int)((double)refpointRgb[2] /(double)9);
					}
				}
				glColor4ub(newRed, newGreen, newBlue, pDoc->m_pUI->getAlpha() * 255);
				glVertex2i(target.x + i, target.y + j);
			}
			//point on boundary
			else {
				Point samplePoint(source.x + i, source.y + j);
				SetColor(samplePoint);
				glVertex2i(target.x + i, target.y + j);
			}
		}
	}
	glEnd();
	glFlush();
}

void SharpeningBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

