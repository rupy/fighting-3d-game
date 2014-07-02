#ifndef __MY_FUNC_H__
#define __MY_FUNC_H__

#include <d3dx9.h>

D3DXMATRIX* CalcLookAtMatrix(
   D3DXMATRIX* pout,
   D3DXVECTOR3* pPos,
   D3DXVECTOR3* pLook,
   D3DXVECTOR3* pUp
);

D3DXMATRIX* CalcLookAtMatrixAxisFix(
   D3DXMATRIX* pout,
   D3DXVECTOR3* pPos,
   D3DXVECTOR3* pLook,
   D3DXVECTOR3* pUp
);

#endif	/*__MY_FUNC_H__*/