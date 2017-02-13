//
// ScatteredPointBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ScatteredPointBrush.h"

extern float frand();

ScatteredPointBrush::ScatteredPointBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void ScatteredPointBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize();



	//glPointSize((float)size);

	BrushMove(source, target);
}

void ScatteredPointBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("ScatteredPointBrush::BrushMove  document is NULL\n");
		return;
	}
	int size = pDoc->getSize();
	glBegin(GL_POINTS);
	for (int i = -size/2; i < size/2; i++) {
		for (int j = -size/2; j < size/2; j++) {
			if (rand()%5 == 1) {
				Point samplePoint(source.x + i, source.y + j);
				SetColor(samplePoint);
				glVertex2i(source.x + i, source.y + j);
				
			}
		}
	}
	glEnd();
	glFlush();
	/*	glBegin(GL_POINTS);
	SetColor(source);

	glVertex2d(target.x, target.y);

	glEnd();*/

}

void ScatteredPointBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}
