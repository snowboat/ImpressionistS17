//
// CustomizedBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "CustomizedBrush.h"
#include <iostream>
#include <string>
using namespace std;

extern float frand();

CustomizedBrush::CustomizedBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void CustomizedBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize();



	glPointSize((float)size);

	BrushMove(source, target);

}

void CustomizedBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;


	if (pDoc == NULL) {
		printf("CustomizedBrush::BrushMove  document is NULL\n");
		return;
	}
	int size = pDoc->getSize();


	int filterRows = dlg->getFilterRows();
	int filterCols = dlg->getFilterCols();



	glPointSize(1.0);
	glBegin(GL_POINTS);

	for (int i = -size / 2; i < size / 2; i++) {
		for (int j = -size / 2; j < size / 2; j++) {//these two loops traverse all points covered by the brush.
			//(xpos,ypos) is the coordinates of the point BEING PROCESSED by the brush
			int xpos = source.x + i;
			int ypos = source.y + j;

			int verticalBleed = (filterRows -1) / 2;
			int horizontalBleed = (filterCols - 1) / 2;
			
			int newRed = 0;
			int newGreen = 0;
			int newBlue = 0;

			int counter = 0;
			if (!dlg->m_vectorOfInputBoxes.empty()) {
				//the following two for loops scans thru the ref grid, right to left, top to bottom
				for (int yref = ypos - verticalBleed; yref <= ypos + verticalBleed; yref++) {
					for (int xref = xpos - horizontalBleed; xref <= xpos + horizontalBleed; xref++) {
						
						Point samplePoint = Point(min(xref, pDoc->m_nPaintWidth), min(yref, pDoc->m_nPaintHeight));
						float refpointWeight = std::stof(dlg->m_vectorOfInputBoxes[counter]->value());
						float refRed = (float)getColorRed(samplePoint);
						float refGreen = (float)getColorGreen(samplePoint);
						float refBlue = (float)getColorBlue(samplePoint);
						
						
						
						newRed += (int)(refpointWeight *refRed);
						newGreen += (int)(refpointWeight * refGreen);
						newBlue += (int)(refpointWeight * refBlue);
						


							
						counter++;
					}
				}
				if (newRed < 0)
					newRed = 0;
				if (newRed > 255)
					newRed = 255;
				if (newGreen < 0)
					newGreen = 0;
				if (newGreen > 255)
					newGreen = 255;
				if (newBlue < 0)
					newBlue = 0;
				if (newBlue > 255)
					newBlue = 255;
				glColor4ub(newRed, newGreen, newBlue,pDoc->m_pUI->getAlpha() * 255);
				//glColor3ub(0, 255, 0);
				glVertex2i(target.x + i, target.y + j);
			}

		}
	}
	glEnd();
	glFlush();
}

void CustomizedBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}
