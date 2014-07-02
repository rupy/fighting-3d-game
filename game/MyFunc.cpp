#include "MyFunc.h"

// ƒLƒƒƒ‰ƒNƒ^p¨s—ñZoŠÖ”
D3DXMATRIX* CalcLookAtMatrix(
   D3DXMATRIX* pout,
   D3DXVECTOR3* pPos,
   D3DXVECTOR3* pLook,
   D3DXVECTOR3* pUp
) {
   D3DXVECTOR3 X, Y, Z;
   Z = *pLook - *pPos;
   D3DXVec3Normalize( &Z, &Z );
   D3DXVec3Cross( &X, D3DXVec3Normalize(&Y, pUp), &Z );
   D3DXVec3Normalize( &X, &X );
   D3DXVec3Normalize( &Y, D3DXVec3Cross( &Y, &Z, &X ));

   pout->_11 = X.x; pout->_12 = X.y; pout->_13 = X.z; pout->_14 = 0;
   pout->_21 = Y.x; pout->_22 = Y.y; pout->_23 = Y.z; pout->_24 = 0;
   pout->_31 = Z.x; pout->_32 = Z.y; pout->_33 = Z.z; pout->_34 = 0;
   pout->_41 = 0.0f; pout->_42 = 0.0f; pout->_43 = 0.0f; pout->_44 = 1.0f;

   return pout;
}


// ƒLƒƒƒ‰ƒNƒ^‘©”›p¨s—ñZoŠÖ”
D3DXMATRIX* CalcLookAtMatrixAxisFix(
   D3DXMATRIX* pout,
   D3DXVECTOR3* pPos,
   D3DXVECTOR3* pLook,
   D3DXVECTOR3* pUp
) {
   D3DXVECTOR3 X, Y, Z, D;
   D = *pLook - *pPos;
   D3DXVec3Normalize( &D, &D );
   D3DXVec3Cross( &X, D3DXVec3Normalize(&Y, pUp), &D );
   D3DXVec3Normalize( &X, &X );
   D3DXVec3Normalize( &Z, D3DXVec3Cross( &Z, &X, &Y ));

   pout->_11 = X.x; pout->_12 = X.y; pout->_13 = X.z; pout->_14 = 0;
   pout->_21 = Y.x; pout->_22 = Y.y; pout->_23 = Y.z; pout->_24 = 0;
   pout->_31 = Z.x; pout->_32 = Z.y; pout->_33 = Z.z; pout->_34 = 0;
   pout->_41 = 0.0f; pout->_42 = 0.0f; pout->_43 = 0.0f; pout->_44 = 1.0f;

   return pout;
}
