//
// ImpBrush.cpp
//
// The implementation of virtual brush. All the other brushes inherit from it.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ImpBrush.h"
#include <iostream>

// Static class member initializations
int			ImpBrush::c_nBrushCount	= 0;
ImpBrush**	ImpBrush::c_pBrushes	= NULL;

ImpBrush::ImpBrush(ImpressionistDoc*	pDoc, 
				   char*				name) :
					m_pDoc(pDoc), 
					m_pBrushName(name)
{
}

//---------------------------------------------------
// Return m_pDoc, which connects the UI and brushes
//---------------------------------------------------
ImpressionistDoc* ImpBrush::GetDocument(void)
{
	return m_pDoc;
}

//---------------------------------------------------
// Return the name of the current brush
//---------------------------------------------------
char* ImpBrush::BrushName(void)
{
	return m_pBrushName;
}

bool ImpBrush::withinBoundary(Point target)
{
	if (target.x >= 0 && target.x <= m_pDoc->m_nPaintWidth && target.y >= 0 && target.y <= m_pDoc->m_nPaintHeight) {
		return true;
	}
	else {
		return false;
	}
}

void ImpBrush::my_glVertex2d(Point source, Point target)
{
	if (source.x >= 0 && source.x <= m_pDoc->m_nPaintWidth && source.y >= 0 && source.y <= m_pDoc->m_nPaintHeight) {
		glVertex2d(target.x, target.y);
	}
	//std::cout << m_pDoc->m_nPaintWidth << "and " << m_pDoc->m_nPaintHeight << std::endl;
	//std::cout << m_pDoc->m_nWidth << "with" << m_pDoc->m_nHeight << std::endl;

}

//----------------------------------------------------
// Set the color to paint with to the color at source,
// which is the coord at the original window to sample 
// the color from
//----------------------------------------------------
void ImpBrush::SetColor (const Point source)
{
	ImpressionistDoc* pDoc = GetDocument();

	GLubyte color[4];

	memcpy ( color, pDoc->GetOriginalPixel( source ), 3 );
	color[3] = (int)(pDoc->getAlpha() * 255);
	glColor4ubv( color );

}

//Retrieve the color values from a particular point from original image (NOT from color selector)
int* ImpBrush::GetColor(const Point source) {
	ImpressionistDoc* pDoc = GetDocument();
	GLubyte color[3];
	memcpy(color, pDoc->GetOriginalPixel(source), 3);
	int rgbColor[3];
	for (int i = 0; i < 3; i++) {
		rgbColor[i] = (int)(color[i]);
	}
	return rgbColor;
}