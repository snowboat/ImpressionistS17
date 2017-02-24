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

	int width = pDoc->m_nWidth;
	// the difference between target and source is the height of lower bound of paint plane
	int lowerHeight = target.y - source.y; 

	int maxdist = lineLength;
	int t = rand() % 2 + 3; // generate 3 or 4 lines

	if (maxdist == 0) {
		maxdist = 1;
	}

	for (int i = 0; i < t; i++) {
		if (maxdist <= 0)
			maxdist = 1;
		double dist = rand() % maxdist;
		double angle = frand() * 2 * M_PI;
	    Point lineCtrl(target.x + dist*cos(angle), target.y + dist*sin(angle));
		Point lineCtrlSource(source.x + dist*cos(angle), source.y + dist*sin(angle));

		// do not draw outside the boundary
		if ((lineCtrlSource.y < 0) || (lineCtrlSource.x > width)) continue; 

		int lineAngle = pDoc->getLineAngle();

		int lineType = pDoc->getStrokeDirection();
		switch (lineType)
		{
		case 2: // Gradient
			if (pDoc->getFlagOfAnotherGradient()) {
				lineAngle = pDoc->getAnotherGradientAngle(lineCtrlSource) + 90;
			}
			else {
				lineAngle = pDoc->getGradientAngle(lineCtrlSource) + 90;
			}
			break;
		case 3: // Brush Direction
			lineAngle = (int)(atan2((lineCtrlSource.y - lastPosition.y), (lineCtrlSource.x - lastPosition.x)) / M_PI * 180);
			break;
		default:
			break;
		}

		double lineAngleRadius = lineAngle * M_PI / 180;

		// std::cout << lineAngleRadius << std::endl;
		int x1 = round((lineLength / 2) * cos(lineAngleRadius) + lineCtrlSource.x);
		int y1 = round((lineLength / 2) * sin(lineAngleRadius) + lineCtrlSource.y);
		int x2 = round(-(lineLength / 2) * cos(lineAngleRadius) + lineCtrlSource.x);
		int y2 = round(-(lineLength / 2) * sin(lineAngleRadius) + lineCtrlSource.y);

		// check edge clipping
		if (pDoc->getFlagOfEdgeClipping()) {
			int standard = (int)(pDoc->m_ucEdge[lineCtrlSource.y*width + lineCtrlSource.x]);
			// std::cout << standard << std::endl;
			// x1 < lineCtrlSource.x < x2
			if (x1 < lineCtrlSource.x) {
				for (int i = lineCtrlSource.x - 1; i >= x1; i--) {
					int j = lineCtrlSource.y - ((lineCtrlSource.y - y1)*(lineCtrlSource.x - i) / (lineCtrlSource.x - x1)); // calculate the y of this point
					if ((int)(pDoc->m_ucEdge[j * width + i]) != standard)
					{
						x1 = i;
						y1 = j;
						break;
					}
				}
				for (int i = lineCtrlSource.x + 1; i <= x2; i++) {
					int j = lineCtrlSource.y - ((lineCtrlSource.y - y2)*(lineCtrlSource.x - i) / (lineCtrlSource.x - x2)); // calculate the y of this point
					if ((int)(pDoc->m_ucEdge[j * width + i]) != standard)
					{
						x2 = i;
						y2 = j;
						break;
					}
				}
			}

			// x2 < lineCtrlSource.x < x1
			if (x1 > lineCtrlSource.x) {
				for (int i = lineCtrlSource.x - 1; i >= x2; i--) {
					int j = lineCtrlSource.y - ((lineCtrlSource.y - y2)*(lineCtrlSource.x - i) / (lineCtrlSource.x - x2)); // calculate the y of this point
					if ((int)(pDoc->m_ucEdge[j * width + i]) != standard)
					{
						x2 = i;
						y2 = j;
						break;
					}
				}
				for (int i = lineCtrlSource.x + 1; i <= x1; i++) {
					int j = lineCtrlSource.y - ((lineCtrlSource.y - y1)*(lineCtrlSource.x - i) / (lineCtrlSource.x - x1)); // calculate the y of this point
					if (((int)(pDoc->m_ucEdge[j * width + i])) != standard)
					{
						x1 = i;
						y1 = j;
						break;
					}
				}
			}

			// x1 = lineCtrlSource.x = x2
			if (x1 == x2) {
				int i = x1;
				if (y1 < y2) {
					for (int j = lineCtrlSource.y - 1; j >= y1; j--) {
						if (((int)(pDoc->m_ucEdge[j * width + i])) != standard)
						{
							x1 = i;
							y1 = j;
							break;
						}
					}
					for (int j = lineCtrlSource.y + 1; j <= y2; j++) {
						if (((int)(pDoc->m_ucEdge[j * width + i])) != standard)
						{
							x2 = i;
							y2 = j;
							break;
						}
					}
				}
				else {
					for (int j = lineCtrlSource.y - 1; j >= y2; j--) {
						if (((int)(pDoc->m_ucEdge[j * width + i])) != standard)
						{
							x2 = i;
							y2 = j;
							break;
						}
					}
					for (int j = lineCtrlSource.y + 1; j <= y1; j++) {
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

		// check if outside the boundary
		if (x1 != x2)
		{
			if (x1 > width)
			{
				y1 = lineCtrlSource.y - ((lineCtrlSource.y - y1)*(lineCtrlSource.x - width) / (lineCtrlSource.x - x1));
				x1 = width;
			}
			if (x2 > width)
			{
				y2 = lineCtrlSource.y - ((lineCtrlSource.y - y2)*(lineCtrlSource.x - width) / (lineCtrlSource.x - x2));
				x2 = width;
			}
		}


		if (y1 != y2) {
			if (y1 < 0) {
				if (x1 != x2) {
					x1 = lineCtrlSource.x - ((lineCtrlSource.x - x1)*(lineCtrlSource.y - 0) / (lineCtrlSource.y - y1));
				}
				y1 = 0;
			}
			if (y2 < 0) {
				if (x1 != x2) {
					x2 = lineCtrlSource.x - ((lineCtrlSource.x - x2)*(lineCtrlSource.y - 0) / (lineCtrlSource.y - y2));
				}
				y2 = 0;
			}
		}

		y1 += lowerHeight;
		y2 += lowerHeight;

		glLineWidth((float)lineWidth);
		glBegin(GL_LINES);
		SetColor(lineCtrlSource);

		//std::cout << "1 is " << x1 << " " << y1 << std::endl;
		//std::cout << "2 is " << x2 << " " << y2 << std::endl;

		glVertex2i(x1, y1);
		glVertex2i(x2, y2);
		glEnd();

	}

	lastPosition = source;
}

void ScatteredLineBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

