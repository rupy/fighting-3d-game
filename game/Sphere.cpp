#include "Sphere.h"
#include "Error.h"

Sphere::Sphere(vector<Effect*>& eff)
:AttachObject(eff,NULL)
{
	SetupModel();
}
Sphere::~Sphere(){
}
HRESULT Sphere::SetupModel(){
	
	if(FAILED(sphere.LoadModelsFromXFile(GetDevice(),SPHERE_FILE_PATH))){
		DisplayErrMsg("モデルの読み込みに失敗しました");
		return(E_FAIL);
	}
	MeshType = &sphere;
	return S_OK;
}