#ifndef __SOUND_H__
#define __SOUND_H__


#include <dsound.h>
#include <d3dx9.h>

#define NUM_OF_SOUNDTYPE	10
#define PRIMARY_CH			2					//ステレオ
#define	PRIMARY_HZ			44100				// 周波数（Hz単位）
#define	PRIMARY_BIT_RATE	16					// 16bit

class	SoundData
{
public:
	WAVEFORMATEX			sndWaveFormatEx;	// 波形データ情報
	HMMIO					sndHmmio;			// 波形データのmmioへのハンドル
	MMCKINFO				sndChunk;			// dataチャンクの情報
	MMCKINFO				sndChunkRiff;		// 波形ファイルを開くときに使う
	DWORD					sndCreationFlags;	// 生成フラグ
	DWORD					sndNumOfBuffers;	// バッファの数

	LPDIRECTSOUND3DBUFFER*	sndDS3DSBuffer;		// 3Dバッファへのポインタとパラメータ
	DS3DBUFFER*				sndDS3DSBufferParams;

	LPDIRECTSOUNDBUFFER*	sndDSSoundBuffer;	// バッファへのポインタ

	int						sndVolume;			// ボリューム
	bool					sndVolumeFlag;

public:
	SoundData();
	~SoundData();
};


class Sound{
public:
	LPDIRECTSOUND8			pDirectSound;				// DirectSoundオブジェクト

	LPDIRECTSOUND3DLISTENER	pDSListener;				// 3Dリスナー
	DS3DLISTENER			DS3DListenerParams;				// 3Dリスナーのパラメータ

public:
	Sound();
	~Sound();

	HRESULT InitDirectSound( HWND hWnd );
	HRESULT LoadFromWaveFile(	char		*aWaveFileName,
								SoundData	*aSound,
								DWORD		aCreationFlags, 
								GUID		guid3DAlgorithm,
								DWORD		aNumOfBuffers );
	HRESULT OpenWaveFile( char	*aWaveFileName , SoundData *aSound );
private:
	HRESULT	CloseWaveFile( SoundData *aSound );
	HRESULT RewindWaveFile( SoundData *aSound );
	HRESULT FillupSoundBuffer( SoundData *aSound , DWORD aLockBuffer );
	HRESULT	RestoreDSBuffer( LPDIRECTSOUNDBUFFER aDSBuffer , BOOL* pRestoreFlag );
	HRESULT ReadWaveToBuffer( SoundData *aSound , BYTE* aBuffer, DWORD aSizeToRead, DWORD* aReadedData );
	HRESULT Get3DBufferInterface( LPDIRECTSOUNDBUFFER aDSSoundBuffer, LPDIRECTSOUND3DBUFFER* aDS3DBuffer );
	HRESULT Get3DListenerInterface( LPDIRECTSOUND3DLISTENER* aDS3DListener );

public:
	HRESULT PlayASound( SoundData *aSound , DWORD aPriority ,DWORD aFlag ,BOOL aOverride);
	DWORD SelectFreeBuffer( SoundData *aSound , BOOL aOverride );
	HRESULT PlayA3DSound(	SoundData	*aSound,
						D3DXVECTOR3	*aPosition,
						D3DXVECTOR3	*aVerocity,
						DWORD		aPriority,
						DWORD		aFlag,
						BOOL		aOverride);
	HRESULT	Setup3DBufferParams( SoundData *aSound , DWORD aIndexNum , D3DXVECTOR3 *aPosition , D3DXVECTOR3 *aVerocity);
	HRESULT StopASound( SoundData *aSound );
	HRESULT ChangeVolume( SoundData *aSound ,long volume);
	bool FadeOutStopASound( SoundData *aSound,int downVolumeSpeed);
};


#endif	/*__SOUND_H__*/