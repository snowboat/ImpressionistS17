//
// PointBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "PointBrush.h"
#include <iostream>

extern float frand();

PointBrush::PointBrush( ImpressionistDoc* pDoc, char* name ) :
	ImpBrush(pDoc,name)
{
}

void PointBrush::BrushBegin( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	int size = pDoc->getSize();



	glPointSize( (float)size );


	//std::cout << "dimensions:" << pDoc->m_nPaintWidth << " " << pDoc->m_nPaintHeight << std::endl;
	//std::cout << pDoc->m_nWidth << " " << pDoc->m_nHeight << std::endl;
	BrushMove( source, target );

}

void PointBrush::BrushMove( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;


	if ( pDoc == NULL ) {
		printf( "PointBrush::BrushMove  document is NULL\n" );
		return;
	}
	//if (withinBoundary(source)) {
	//	std::cout << "within boundary" << std::endl;
	//	glBegin(GL_POINTS);
	//	SetColor(source);
	//	//std::cout << GetColor(source)[0] << " " << GetColor(source)[1] << " "<<GetColor(source)[2] << std::endl;
	//	glVertex2d(target.x, target.y);

	//	glEnd();
	//}
	glBegin(GL_POINTS);
	SetColor(source);
	my_glVertex2d(source,target);
	glEnd();
	glFlush();
}

void PointBrush::BrushEnd( const Point source, const Point target )
{
	// do nothing so far
}

