//
// SharpeningBrush.h
//
// The header file for Point Brush. 
//

#ifndef SharpeningBrush_H
#define SharpeningBrush_H

#include "ImpBrush.h"

class SharpeningBrush : public ImpBrush
{
public:
	SharpeningBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);
};

#endif