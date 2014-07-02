#ifndef __MOVIE_H__
#define __MOVIE_H__

#pragma comment(lib, "strmiids.lib")
#include <dshow.h>
#include <d3d9.h>
#include <Vmr9.h>

#define WM_GRAPHNOTIFY WM_APP + 1

class Movie{
public:
	IGraphBuilder	*pGB;
	ICaptureGraphBuilder2 *pCGB2;
	IBaseFilter		*pVM;
	IMediaControl	*pMC;
	LONGLONG pCurrent;
	LONGLONG pStop;

	Movie();
	~Movie();

	HRESULT InitMovie(HWND hWnd);
};

#endif	/*__MOVIE_H__*/