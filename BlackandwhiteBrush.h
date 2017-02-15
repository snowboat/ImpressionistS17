#pragma once
#pragma once
//
// PointBrush.h
//
// The header file for Point Brush. 
//

#ifndef BLACKANDWHITEBRUSH_H
#define BLACKANDWHITEBRUSH_H

#include "ImpBrush.h"

class BlackandwhiteBrush : public ImpBrush
{
public:
	BlackandwhiteBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);
};

#endif