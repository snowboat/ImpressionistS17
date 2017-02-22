//
// CustomizedBrush.h
//
// The header file for Point Brush. 
//

#ifndef CustomizedBrush_H
#define CustomizedBrush_H

#include "ImpBrush.h"

class CustomizedBrush : public ImpBrush
{
public:
	CustomizedBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);
};

#endif#pragma once
