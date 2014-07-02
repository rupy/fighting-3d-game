#ifndef __RESOURCE_BASE_H__
#define __RESOURCE_BASE_H__

//プログラムで使うリソースはすべてデバイスを使うので、リソースすべてにこのクラスを継承させて使用する
//このクラスを使う場合には静的メンバを必ず初期化しておくこと

#include <d3dx9.h>


class ResourceBase{
private:
	static LPDIRECT3DDEVICE9 pDevice;//デバイス

public:
	//デバイスを受け取るアクセサメソッド
	inline LPDIRECT3DDEVICE9 GetDevice(){
		return pDevice;
	}
	static inline void SetDevice(LPDIRECT3DDEVICE9 device){
		pDevice = device;
	}
};

#endif	/*__RESOURCE_BASE_H__*/