#ifndef __MESH_CPU_H__
#define __MESH_CPU_H__

#include "MeshObject.h"
#include "Operator.h"

class MeshCPU:public Operator{
public:
	enum CPUMode{
		aaaa,
		bbb,
		cc,
	};
	MeshObject*	obj;
	CPUMode		mode;


	MeshCPU(MeshObject* obj);

	void Move();
};

#endif	/*__MESH_CPU_H__*/