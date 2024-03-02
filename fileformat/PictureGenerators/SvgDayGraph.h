#ifndef SVGDAYGRAPH_H
#define SVGDAYGRAPH_H

#include "PictureGenerator.h"

class SvgDayGraph : public PictureGenerator {
	Q_DECLARE_TR_FUNCTIONS(SvgDayGraph)
protected:
	static QString addSvgHull(const QString& content, int width);
	static QString drawBorderAndTimeAxis();
};

#endif
