#pragma once
#pragma once
//
// PointBrush.h
//
// The header file for Point Brush. 
//

#ifndef SATURATIONBRUSH_H
#define SATURATIONBRUSH_H

#include "ImpBrush.h"

class SaturationBrush : public ImpBrush
{
public:
	SaturationBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);
};

#endif