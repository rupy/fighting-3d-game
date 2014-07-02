#ifndef __MASURELINE2D_OBJECT_H__
#define __MASURELINE2D_OBJECT_H__

#include "Object.h"
#include <vector>

using namespace std;

class MeasureLine2D :public Object
{
public:
	int increment;
	int emphasis;
	size_t xAxisNum;
	size_t yAxisNum;

	MeasureLine2D(int increment,int emphasis);

	void Appear();

};

#endif	/*__MASURELINE2D_OBJECT_H__*/