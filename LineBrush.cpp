//
// LineBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "LineBrush.h"
#include <math.h>
#include <iostream>

extern float frand();

Point lastPos;

LineBrush::LineBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void LineBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	lastPos = target;

	BrushMove(source, target);
}

void LineBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;


	if (pDoc == NULL) {
		printf("LineBrush::BrushMove  document is NULL\n");
		return;
	}

	int lineLength = pDoc->getSize(); // get the line length from UI size
	int lineWidth = pDoc->getLineWidth();

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
		lineAngle =(int)(atan2((source.y - lastPos.y),(source.x - lastPos.x)) / M_PI * 180);
		break;
	default:
		break;
	}

	glPushMatrix();
	glTranslated(target.x, target.y, 0);
	//glRotated(lineAngle, 0.0, 0.0, 1.0);
	
	glBegin(GL_POLYGON);
		SetColor(source);
		int xUpperBound = pDoc->m_nPaintWidth;
		int yLowerBound = pDoc->m_pUI->m_paintView->getWindowHeight() - pDoc->m_nPaintHeight;
		//if (target.x + (double)-lineLength / 2 <= xUpperBound && target.y + (double)-lineWidth / 2 >= yLowerBound)
			glVertex3d((double)-lineLength / 2, (double)-lineWidth / 2, 0.0);
		//std::cout << "point at" << target.x + (double)-lineLength / 2 << " " << target.y + (double)-lineWidth / 2 << std::endl;
		//if (target.x + (double)lineLength / 2 <= xUpperBound && target.y + (double)-lineWidth / 2 >= yLowerBound)
			glVertex3d((double)lineLength / 2, (double)-lineWidth / 2, 0.0);
		//if (target.x + (double)lineLength / 2 <= xUpperBound && target.y + (double)lineWidth / 2 >= yLowerBound)
			glVertex3d((double)lineLength / 2, (double)lineWidth / 2, 0.0);
		//if (target.x + (double)-lineLength / 2 <= xUpperBound && target.y + (double)lineWidth / 2 >= yLowerBound)
			glVertex3d((double)-lineLength / 2, (double)lineWidth / 2, 0.0);
	glEnd();

	//glTranslated(-target.x, -target.y, 0.0);
	glPopMatrix();

	lastPos = source;
}

void LineBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}
