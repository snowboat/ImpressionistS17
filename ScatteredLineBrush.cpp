//
// ScatteredLineBrush.cpp
//
// The implementation of Scattered Line Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ScatteredLineBrush.h"
#include <iostream>
#include <math.h>
using namespace std;

extern float frand();

Point lastPosition;

ScatteredLineBrush::ScatteredLineBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void ScatteredLineBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	lastPosition = target;

	BrushMove(source, target);
}

void ScatteredLineBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("ScatteredLineBrush::BrushMove  document is NULL\n");
		return;
	}

	int lineLength = pDoc->getSize(); // get the line length from UI size
	int lineWidth = pDoc->getLineWidth();

	int maxdist = lineLength;
	int t = rand() % 2 + 3; // generate 3 or 4 lines

	for (int i = 0; i < t; i++) {
		double dist = rand() % maxdist;
		double angle = frand() * 2 * M_PI;
		Point lineCtrl(target.x + dist*cos(angle), target.y + dist*sin(angle));
		Point lineCtrlSource(source.x + dist*cos(angle), source.y + dist*sin(angle));

		int lineAngle = pDoc->getLineAngle(); // Slider or Right Mouse
		int lineType = pDoc->getStrokeDirection();
		switch (lineType)
		{
		case 2: // Gradient
			if (pDoc->getFlagOfAnotherGradient()) {
				lineAngle = pDoc->m_sobelFilter->getAnotherGradientAngle(source) + 90;
			}
			else {
				lineAngle = pDoc->m_sobelFilter->getGradientAngle(source) + 90;
			}
			break;
		case 3: // Brush Direction
			lineAngle = (int)(atan2((source.y - lastPosition.y), (source.x - lastPosition.x)) / M_PI * 180);
			break;
		default:
			break;
		}

		glPushMatrix();
		glTranslated(lineCtrl.x, lineCtrl.y, 0);
		glRotated(lineAngle, 0.0, 0.0, 1.0);

		//boundaries of drawing
		int xUpperBound = pDoc->m_nPaintWidth;
		int yLowerBound = pDoc->m_pUI->m_paintView->getWindowHeight() - pDoc->m_nPaintHeight;
		/*cout << xUpperBound << " " << yLowerBound << endl;
		cout << "vertex1: " << (double)(-lineLength / 2) + lineCtrl.x << " " << (double)(lineWidth / 2) + lineCtrl.y << endl;
		cout << "vertex2: " << (double)(-lineLength / 2) + lineCtrl.x << " " << (double)(-lineWidth / 2) + lineCtrl.y << endl;
		cout << "vertex3: " << (double)(lineLength / 2) + lineCtrl.x << " " << (double)(-lineWidth / 2) + lineCtrl.y << endl;
		cout << "vertex4: " << (double)(lineLength / 2) + lineCtrl.x << " " << (double)(lineWidth / 2) + lineCtrl.y << endl;
		*/glBegin(GL_POLYGON);
		SetColor(lineCtrlSource);
		if ((double)(-lineLength / 2)+lineCtrl.x <= xUpperBound && ((double)(-lineWidth / 2) + lineCtrl.y) >= yLowerBound)
			glVertex3d((double)-lineLength / 2, (double)-lineWidth / 2, 0.0);
		if ((double)(lineLength / 2) + lineCtrl.x <= xUpperBound && ((double)(-lineWidth / 2) + lineCtrl.y) >= yLowerBound)
			glVertex3d((double)lineLength / 2, (double)-lineWidth / 2, 0.0);
		if ((double)(lineLength / 2) + lineCtrl.x <= xUpperBound && ((double)(lineWidth / 2) + lineCtrl.y) >= yLowerBound)
			glVertex3d((double)lineLength / 2, (double)lineWidth / 2, 0.0);
		if ((double)(-lineLength / 2) + lineCtrl.x <= xUpperBound && ((double)(lineWidth / 2) + lineCtrl.y) >= yLowerBound)
			glVertex3d((double)-lineLength / 2, (double)lineWidth / 2, 0.0);
		glEnd();

		//glTranslated(-target.x, -target.y, 0.0);
		glPopMatrix();
	}

	lastPosition = source;
}

void ScatteredLineBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}
