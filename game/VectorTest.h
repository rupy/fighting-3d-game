#ifndef __VECTOR_TEST_H__
#define __VECTOR_TEST_H__

#include "MeshObject.h"
#define VECTOR_MODEL_FILENAME "models/arrow.x"


//���F�N�g���̑傫�������o�I�Ɋm�F����N���X
class VectorTest:public MeshObject{
private:
	static VectorTest* This; 
	static float veclong;
public:	
	VectorTest(vector<Effect*>& eff);

	void Shadow();
	void Appear();

	static void SetDir(D3DXVECTOR3 dir);
};
#endif	/*__VECTOR_TEST_H__*/