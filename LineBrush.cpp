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

	lastPos = source;

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

	int lineAngle = pDoc->getLineAngle();

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

	double lineAngleRadius = lineAngle * M_PI / 180;

	// std::cout << lineAngleRadius << std::endl;
	int x1 = (lineLength / 2) * cos(lineAngleRadius) + source.x + 0.5;
	int y1 = (lineLength / 2) * sin(lineAngleRadius) + source.y + 0.5;
	int x2 = -(lineLength / 2) * cos(lineAngleRadius) + source.x + 0.5;
	int y2 = -(lineLength / 2) * sin(lineAngleRadius) + source.y + 0.5;

	int width = pDoc->m_nWidth;
	int lowerHeight = target.y - source.y;

	// check edge clipping
	if (pDoc->getFlagOfEdgeClipping()) {
		int standard = (int)(pDoc->m_ucEdge[source.y*width + source.x]);
		// std::cout << standard << std::endl;
		// x1 < source.x < x2
		if (x1 < source.x) {
			for (int i = source.x-1; i >= x1; i--) {
				int j = source.y - ((source.y - y1)*(source.x - i) / (source.x - x1)); // calculate the y of this point
				if ((int)(pDoc->m_ucEdge[j * width + i]) != standard)
				{
					x1 = i;
					y1 = j;
					break;
				}
			}
			for (int i = source.x+1; i <= x2; i++) {
				int j = source.y - ((source.y - y2)*(source.x - i) / (source.x - x2)); // calculate the y of this point
				if ((int)(pDoc->m_ucEdge[j * width + i]) != standard)
				{
					x2 = i;
					y2 = j;
					break;
				}
			}
		}

		// x2 < source.x < x1
		if (x1 > source.x) {
			for (int i = source.x - 1; i >= x2; i--) {
				int j = source.y - ((source.y - y2)*(source.x - i) / (source.x - x2)); // calculate the y of this point
				if ((int)(pDoc->m_ucEdge[j * width + i]) != standard)
				{
					x2 = i;
					y2 = j;
					break;
				}
			}
			for (int i = source.x + 1; i <= x1; i++) {
				int j = source.y - ((source.y - y1)*(source.x - i) / (source.x - x1)); // calculate the y of this point
				if (((int)(pDoc->m_ucEdge[j * width + i])) != standard)
				{
					x1 = i;
					y1 = j;
					break;
				}
			}
		}

		// x1 = source.x = x2
		if (x1 == x2) {
			int i = x1;
			if (y1 < y2) {
				for (int j = source.y - 1; j >= y1; j--) {
					if (((int)(pDoc->m_ucEdge[j * width + i])) != standard)
					{
						x1 = i;
						y1 = j;
						break;
					}
				}
				for (int j = source.y + 1; j <= y2; j++) {
					if (((int)(pDoc->m_ucEdge[j * width + i])) != standard)
					{
						x2 = i;
						y2 = j;
						break;
					}
				}
			}
			else {
				for (int j = source.y - 1; j >= y2; j--) {
					if (((int)(pDoc->m_ucEdge[j * width + i])) != standard)
					{
						x2 = i;
						y2 = j;
						break;
					}
				}
				for (int j = source.y + 1; j <= y1; j++) {
					if (((int)(pDoc->m_ucEdge[j * width + i])) != standard)
					{
						x1 = i;
						y1 = j;
						break;
					}
				}
			}
		}
	}

	/*
	int xUpperBound = pDoc->m_nPaintWidth;
	int yLowerBound = pDoc->m_pUI->m_paintView->getWindowHeight() - pDoc->m_nPaintHeight;

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
	*/

	// boundary check
	if (x1 != x2) 
	{
		if (x1 > width) 
		{
			y1 = source.y - ((source.y - y1)*(source.x - width) / (source.x - x1));
			x1 = width;
		}
		if (x2 > width) 
		{
			y2 = source.y - ((source.y - y2)*(source.x - width) / (source.x - x2));
			x2 = width;
		}
	}
	
	
	if (y1 != y2) {
		if (y1 < 0) {
			if (x1 != x2) {
				x1 = source.x - ((source.x - x1)*(source.y - 0) / (source.y - y1));
			}
			y1 = 0;
		}
		if (y2 < 0) {
			if (x1 != x2) {
				x2 = source.x - ((source.x - x2)*(source.y - 0) / (source.y - y2));
			}
			y2 = 0;
		}
	}

	y1 += lowerHeight;
	y2 += lowerHeight;

	glLineWidth((float)lineWidth);
	glBegin(GL_LINES);
	SetColor(source);
	
	std::cout << "1 is " << x1 << " " << y1 << std::endl;
	std::cout << "2 is " << x2 << " " << y2 << std::endl;

	glVertex2i(x1, y1);
	glVertex2i(x2, y2);
	glEnd();

	lastPos = source;
}

void LineBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}
