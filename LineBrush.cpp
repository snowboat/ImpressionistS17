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

	float lineAngle = pDoc->getLineAngle() * M_PI / 180; // Slider or Right Mouse, IN RADIANS

	int lineType = pDoc->getStrokeDirection();
	switch (lineType)
	{
	case 2: // Gradient
		if (pDoc->getFlagOfAnotherGradient()) {
			lineAngle = pDoc->getAnotherGradientAngle(source) + 90;
		}
		else {
			lineAngle = pDoc->getGradientAngle(source) + 90;
		}
		break;
	case 3: // Brush Direction
		lineAngle =(int)(atan2((source.y - lastPos.y),(source.x - lastPos.x)) / M_PI * 180);
		break;
	default:
		break;
	}
	
	/*
		glPushMatrix();
	glTranslated(target.x, target.y, 0);
	glRotated(lineAngle, 0.0, 0.0, 1.0);

	glBegin(GL_POLYGON);
	SetColor(source);
	int xUpperBound = pDoc->m_nPaintWidth;
	int yLowerBound = pDoc->m_pUI->m_paintView->getWindowHeight() - pDoc->m_nPaintHeight;
	//if (target.x + (double)-lineLength / 2 <= xUpperBound && target.y + (double)-lineWidth / 2 >= yLowerBound)
	glVertex3d((double)-lineLength / 2, (double)-lineWidth / 2, 0.0);
	//if (target.x + (double)lineLength / 2 <= xUpperBound && target.y + (double)-lineWidth / 2 >= yLowerBound)
	glVertex3d((double)lineLength / 2, (double)-lineWidth / 2, 0.0);
	//if (target.x + (double)lineLength / 2 <= xUpperBound && target.y + (double)lineWidth / 2 >= yLowerBound)
	glVertex3d((double)lineLength / 2, (double)lineWidth / 2, 0.0);
	//if (target.x + (double)-lineLength / 2 <= xUpperBound && target.y + (double)lineWidth / 2 >= yLowerBound)
	glVertex3d((double)-lineLength / 2, (double)lineWidth / 2, 0.0);
	glEnd();

	//glTranslated(-target.x, -target.y, 0.0);
	glPopMatrix();
	*/

	


		int xUpperBound = pDoc->m_nPaintWidth;
	int yLowerBound = pDoc->m_pUI->m_paintView->getWindowHeight() - pDoc->m_nPaintHeight;

	glLineWidth((float)lineWidth);
	glBegin(GL_LINES);
	SetColor(source);
	std::cout << lineAngle << std::endl;
	int x1 = (lineLength / 2) * cos(lineAngle) + target.x;
	int y1 = (lineLength / 2)*sin(lineAngle) + target.y;
	int x2 = -(lineLength / 2) * cos(lineAngle) + target.x;
	int y2 = -(lineLength / 2)*sin(lineAngle) + target.y;
	
	
	if (y1 < yLowerBound) {
		float xl = (float)(target.x - x1)* (float)(yLowerBound - y1) / (float)(target.y - y1);
		int newXoffset = (target.x - x1 - xl);
		y1 = yLowerBound;
		x1 = target.x - newXoffset;
	}
	if (y2 < yLowerBound) {
		float xl =(float)(target.x-x2)* (float)(yLowerBound - y2) / (float)(target.y - y2);
		int newXoffset = (target.x - x2 - xl);
		y2 = yLowerBound;
		x2 = target.x - newXoffset;
	}

	if (x1 > xUpperBound) {
		float yl = (float)(y1-target.y)*(float)(x1 - xUpperBound) / (float)(x1 - target.x);
		int newyOffset = (y1 - target.y - yl);
		y1 = target.y + newyOffset;
		x1 = xUpperBound;
	}


	glVertex2i(x1, y1);
	glVertex2i(x2, y2);
	glEnd();

	lastPos = source;
}

void LineBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}
