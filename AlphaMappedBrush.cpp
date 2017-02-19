//
// AlphaMappedBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "AlphaMappedBrush.h"

extern float frand();

AlphaMappedBrush::AlphaMappedBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void AlphaMappedBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;


	BrushMove(source, target);
}

void AlphaMappedBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("AlphaMappedBrush::BrushMove  document is NULL\n");
		return;
	}
	glPointSize(1.0);
	glBegin(GL_POINTS);



	for (int yOffset = -pDoc->m_alphaHeight / 2; yOffset < pDoc->m_alphaHeight / 2; yOffset++) {
		for (int xOffset = -pDoc->m_alphaWidth / 2; xOffset < pDoc->m_alphaWidth / 2; xOffset++) {
			int* sourceColor = GetColor(source);
			
			//current pixel is the numPixel'th pixel in the alpha image.
			int rowNum = (yOffset + pDoc->m_alphaHeight / 2);
			int colNum = xOffset + pDoc->m_alphaWidth / 2;
			int numPixel = rowNum*pDoc->m_alphaWidth + colNum;

			glColor4ub(sourceColor[0], sourceColor[1], sourceColor[2], pDoc->m_alphaMappedValues[numPixel]);
			
			//boundary clipping
			int xUpperBound = pDoc->m_nPaintWidth;
			int yLowerBound = pDoc->m_pUI->m_paintView->getWindowHeight() - pDoc->m_nPaintHeight;

			if (target.x + xOffset <= xUpperBound && target.y + yOffset >= yLowerBound)
				glVertex2i(target.x + xOffset, target.y + yOffset);
		}
	}



	glEnd();
	glFlush();
}

void AlphaMappedBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}
