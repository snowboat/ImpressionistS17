//
// BlurringBrush.h
//
// The header file for Point Brush. 
//

#ifndef BlurringBrush_H
#define BlurringBrush_H

#include "ImpBrush.h"

class BlurringBrush : public ImpBrush
{
public:
	BlurringBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);
};

#endif