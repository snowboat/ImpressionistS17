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

extern float frand();

Point lastPosition;

LineBrush::LineBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void LineBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	lastPosition = target;

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
		lineAngle = pDoc->m_sobelFilter->getGradientAngle(source) + 90;
		break;
	case 3: // Brush Direction
		lineAngle =(int)(atan2((source.y - lastPosition.y),(source.x - lastPosition.x)) / M_PI * 180);
		break;
	default:
		break;
	}

	glPushMatrix();
	glTranslated(target.x, target.y, 0);
	glRotated(lineAngle, 0.0, 0.0, 1.0);
	
	glBegin(GL_POLYGON);
		SetColor(source);
		glVertex3d((double)-lineLength / 2, (double)-lineWidth / 2, 0.0);
		glVertex3d((double)lineLength / 2, (double)-lineWidth / 2, 0.0);
		glVertex3d((double)lineLength / 2, (double)lineWidth / 2, 0.0);
		glVertex3d((double)-lineLength / 2, (double)lineWidth / 2, 0.0);
	glEnd();

	//glTranslated(-target.x, -target.y, 0.0);
	glPopMatrix();

	lastPosition = source;
}

void LineBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}
