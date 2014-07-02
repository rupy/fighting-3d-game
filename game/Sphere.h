#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "MeshObject.h"
#include "AttachObject.h"


#define SPHERE_FILE_PATH "models/sword.x"

class Sphere:public AttachObject{
public:
	
	Mesh sphere;
	Sphere(vector<Effect*>& eff);
	~Sphere();

	HRESULT SetupModel();

};

#endif	/*__SPHERE_H__*/