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
	/*
	std::cout << pDoc->getAlpha() << std::endl;*/
	color[3] = (int)(pDoc->getAlpha() * 255);/*
	std::cout << (double)color[3] << std::endl;
	std::cout << color[3] << std::endl;
	std::cout << (double)color[0] << " " << (double)color[1] << " " << (double)color[2] << " " << (double)color[3] << std::endl;
	*/
	glColor4ubv( color );

}