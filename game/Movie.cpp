#include "Movie.h"


Movie::Movie()
:pGB(NULL),pCGB2(NULL),pMC(NULL),pVMR9(NULL),pSource(NULL),pME(NULL)
{
}
Movie::~Movie(){
		SAFE_RELEASE(pMC);
		SAFE_RELEASE(pVMR9);
		SAFE_RELEASE(pSource);
		SAFE_RELEASE(pCGB2);
		SAFE_RELEASE(pGB);
		SAFE_RELEASE(pME);
		CoUninitialize();
}

HRESULT Movie::InitMovie(LPCWSTR fileName){

	HWND hWnd=FindWindow(WINDOW_TITLE,NULL);

	HRESULT hr;

	// COMの初期化
	hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED); 
	if(FAILED(hr)) {return E_FAIL;}

	// フィルタグラフマネージャの作成
	hr = CoCreateInstance( CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&pGB);
	if(FAILED(hr)) {return E_FAIL;}

	// VRM9フィルタの作成と登録
	hr = CoCreateInstance( CLSID_VideoMixingRenderer9, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)&pVMR9);
	if(FAILED(hr)) {return E_FAIL;}
	pGB->AddFilter( pVMR9, L"VMR9" );       // フィルタグラフに登録

	// VRM9をウィンドウレスモードにする
	IVMRFilterConfig *pVMRCfg = NULL;
	hr = pVMR9->QueryInterface( IID_IVMRFilterConfig9, (void**)&pVMRCfg );
	if(FAILED(hr)) {return E_FAIL;}
	pVMRCfg->SetRenderingMode( VMRMode_Windowless );
	pVMRCfg->Release();     // IVMRFilterConfigはもう必要ない

	// 描画ウィンドウの指定
	IVMRWindowlessControl9 *pVMRWndCont = NULL;
	hr = pVMR9->QueryInterface( IID_IVMRWindowlessControl9, (void**)&pVMRWndCont );
	if(FAILED(hr)) {return E_FAIL;}
	hr = pVMRWndCont->SetVideoClippingWindow( hWnd );
	if(FAILED(hr)) {return E_FAIL;}

	// ソースフィルタの生成と登録
	hr = pGB->AddSourceFilter(fileName,fileName, &pSource); 
	if(FAILED(hr)) {return E_FAIL;}

	// CaptureGraphBuilder2インターフェイスの取得
	hr = CoCreateInstance( CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC_SERVER, IID_ICaptureGraphBuilder2, (void**)&pCGB2);
	if(FAILED(hr)) {return E_FAIL;}
	pCGB2->SetFiltergraph( pGB ); 

	// フィルタの接続
	hr = pCGB2->RenderStream(0, 0, pSource, 0, pVMR9);
	if(FAILED(hr)) {return E_FAIL;}
	hr = pCGB2->RenderStream(0, &MEDIATYPE_Audio, pSource, 0, 0);

	// 描画領域の設定（接続後でないとエラーになる）
	LONG W,H;
	RECT SrcR, DestR;
	hr = pVMRWndCont->GetNativeVideoSize(&W, &H, NULL, NULL);
	SetRect(&SrcR, 0, 0, W, H);
	GetClientRect(hWnd, &DestR);
	hr = pVMRWndCont->SetVideoPosition(&SrcR, &DestR);
	pVMRWndCont->Release();         // ウィンドウレスコントロールはもう必要ない

	// メディアコントロールインターフェイスの取得
	hr = pGB->QueryInterface( IID_IMediaControl, (void**)&pMC );
	if(FAILED(hr)) {return E_FAIL;}

	
	if(FAILED(pGB->QueryInterface(IID_IMediaEventEx, (void **)&pME))){
		return E_FAIL;
	}

	return S_OK;

}

HRESULT Movie::PlayMovie(){
	// ストリームの再生（グラフの実行）
	pMC->Run();
	return S_OK;
}
HRESULT Movie::PauseMovie(){
	// ストリームの一時停止
	pMC->Pause();
	return S_OK;
}
HRESULT Movie::StopMovie(){
	// ストリームの一時停止
	pMC->Stop();
	return S_OK;
}

HRESULT Movie::ChangePauseMovie(){
	if(GetMovieState()){
		PlayMovie();
	}else{
		PauseMovie();
	}
	return S_OK;
}

bool Movie::GetMovieState(){

	OAFilterState   pfs;
	if(FAILED(pMC->GetState(0,&pfs))){
		return false;
	}
	if(pfs==State_Paused||pfs==State_Stopped){
		return true;
	}
	return false;
}

bool Movie::CheckNowPlaying(){
	LONG v = 0;
	pME->WaitForCompletion( 100, &v );
	if ( v == EC_COMPLETE ) {
		return false;
	}
	return true;
}