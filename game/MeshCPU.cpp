#include "MeshCPU.h"

MeshCPU::MeshCPU(MeshObject* obj)
:obj(obj),mode(aaaa)
{
}

void MeshCPU::Move(){
	switch(mode){
		case 0:
			obj->Pos.x+=0.1f;
			break;
		default:
			break;
	}
}