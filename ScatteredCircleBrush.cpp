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
	float pi = 3.14159265359;
	double radius = pDoc->getSize() / 2;

	//Draw the central circle
	glBegin(GL_POLYGON);
	SetColor(source);
	for (double i = 0; i < 2 * pi; i += pi / 36) {
		glVertex2d(cos(i)*radius + target.x, sin(i)*radius + target.y);
	}
	glEnd();

	//Draw the surrounding circles
	int maxdist = 2 * radius;
	double dist1 = rand() % maxdist;
	double angle1 = frand() * 2 * pi;
	Point circleCtr1(source.x + dist1*cos(angle1), source.y + dist1*sin(angle1));
	glBegin(GL_POLYGON);
	SetColor(circleCtr1);
	for (double i = 0; i < 2 * pi; i += pi / 36) {
		glVertex2d(cos(i)*radius + circleCtr1.x, sin(i)*radius + circleCtr1.y);
	}
	glEnd();

	double dist2 = rand() % maxdist;
	double angle2 = frand() * 2 * pi;
	Point circleCtr2(source.x + dist2*cos(angle2), source.y + dist2*sin(angle2));
	glBegin(GL_POLYGON);
	SetColor(circleCtr2);
	for (double i = 0; i < 2 * pi; i += pi / 36) {
		glVertex2d(cos(i)*radius + circleCtr2.x, sin(i)*radius + circleCtr2.y);
	}
	glEnd();
	

}

void ScatteredCircleBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}
