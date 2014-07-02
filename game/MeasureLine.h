#ifndef __MASURELINE_OBJECT_H__
#define __MASURELINE_OBJECT_H__

#include "Object.h"
#include <vector>

using namespace std;

class MeasureLine :public Object
{
public:
	int domain_start;
	int domain_end;
	int range_start;
	int range_end;

	size_t xAxisNum;
	size_t yAxisNum;

	MeasureLine(int domain_start = -10,int domain_end = 10,int range_start = -10,int range_end = 10);

	void Appear();

};

#endif	/*__MASURELINE_OBJECT_H__*/