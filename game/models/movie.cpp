#include "movie.h"


Movie::Movie()
:pGB(NULL),pVM(NULL),pMC(NULL){
	pCurrent = 1;
	pStop = 1;
}
Movie::~Movie(){}

HRESULT Movie::InitMovie(HWND hWnd){

	// COMの初期化
    if(FAILED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED))){
		return E_FAIL;
	}

	//COMインターフェース取得
	if(FAILED(CoCreateInstance(CLSID_FilterGraph,NULL,CLSCTX_INPROC_SERVER,IID_IGraphBuilder,(void**)&pGB))){
			CoUninitialize();
			return E_FAIL;
	}
	//COMインターフェース取得
	if(FAILED(CoCreateInstance(CLSID_VideoMixingRenderer9,NULL,CLSCTX_INPROC_SERVER,IID_IBaseFilter,(void**)&pVM))){
			CoUninitialize();
			return E_FAIL;
	}
	pGB->AddFilter(pVM, L"VMR9");

	// VRM9をウィンドウレスモードにする
	IVMRFilterConfig *pVMRCfg = NULL;
	if(FAILED(pVM->QueryInterface( IID_IVMRFilterConfig9, (void**)&pVMRCfg )))
	{pGB->Release(); CoUninitialize(); return E_FAIL;}
	pVMRCfg->SetRenderingMode( VMRMode_Windowless );
	pVMRCfg->Release();     // IVMRFilterConfigはもう必要ない
	
	// 描画ウィンドウの指定
	IVMRWindowlessControl9 *pVMRWndCont = NULL;
	if(FAILED(pVM->QueryInterface( IID_IVMRWindowlessControl9, (void**)&pVMRWndCont ))) {
		pGB->Release(); CoUninitialize(); return E_FAIL;
	}
	pVMRWndCont->SetVideoClippingWindow( hWnd );

	WCHAR wFileName[] = L"OP.AVI";
	IBaseFilter *pSource = NULL;
	pGB->AddSourceFilter(wFileName, wFileName, &pSource);
	
	pCGB2 = NULL;
	if(FAILED(CoCreateInstance( CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC_SERVER, IID_ICaptureGraphBuilder2, (void**)&pCGB2))){
		pCGB2->Release(); pGB->Release(); CoUninitialize(); return E_FAIL;
	}
	pCGB2->RenderStream(0, &MEDIATYPE_Audio, pSource, 0, 0);

	// 描画領域の設定（接続後でないとエラーになる）
	LONG W,H;
	RECT SrcR, DestR;
	pVMRWndCont->GetNativeVideoSize(&W, &H, NULL, NULL);
	SetRect(&SrcR, 0, 0, W, H);
	GetClientRect(hWnd, &DestR);
	pVMRWndCont->SetVideoPosition(&SrcR, &DestR);
	pVMRWndCont->Release();         // ウィンドウレスコントロールはもう必要ない


	// メディアコントロールインターフェイスの取得
	if(FAILED(pGB->QueryInterface( IID_IMediaControl, (void**)&pMC ))) {
		pCGB2->Release(); pGB->Release(); CoUninitialize(); return E_FAIL;
	}

	// ストリームの再生（グラフの実行）
	pMC->Run();


	return S_OK;

}