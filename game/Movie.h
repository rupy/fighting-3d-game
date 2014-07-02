#ifndef __MOVIE_H__
#define __MOVIE_H__

#pragma warning(disable:4995)
//DirectShowÇ…ÇÊÇÈåxçêÇÃÇΩÇﬂ

#include "Common.h"
#include <dshow.h>
#include <d3d9.h>
#include <Vmr9.h>

#define WM_GRAPHNOTIFY WM_APP + 1

class Movie{
public:
	IGraphBuilder	*pGB;
	ICaptureGraphBuilder2 *pCGB2;
	IMediaControl	*pMC;
	IBaseFilter		*pVMR9;
	IBaseFilter *pSource;
	IMediaEventEx *pME;

	Movie();
	~Movie();

	HRESULT InitMovie(LPCWSTR fileName);
	HRESULT PlayMovie();
	HRESULT PauseMovie();
	HRESULT StopMovie();
	HRESULT ChangePauseMovie();
	bool GetMovieState();
	bool CheckNowPlaying();
};

#endif	/*__MOVIE_H__*/