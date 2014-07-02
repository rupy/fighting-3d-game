#ifndef __SOUND_H__
#define __SOUND_H__


#include <dsound.h>
#include <d3dx9.h>

#define NUM_OF_SOUNDTYPE	10
#define PRIMARY_CH			2					//�X�e���I
#define	PRIMARY_HZ			44100				// ���g���iHz�P�ʁj
#define	PRIMARY_BIT_RATE	16					// 16bit

class	SoundData
{
public:
	WAVEFORMATEX			sndWaveFormatEx;	// �g�`�f�[�^���
	HMMIO					sndHmmio;			// �g�`�f�[�^��mmio�ւ̃n���h��
	MMCKINFO				sndChunk;			// data�`�����N�̏��
	MMCKINFO				sndChunkRiff;		// �g�`�t�@�C�����J���Ƃ��Ɏg��
	DWORD					sndCreationFlags;	// �����t���O
	DWORD					sndNumOfBuffers;	// �o�b�t�@�̐�

	LPDIRECTSOUND3DBUFFER*	sndDS3DSBuffer;		// 3D�o�b�t�@�ւ̃|�C���^�ƃp�����[�^
	DS3DBUFFER*				sndDS3DSBufferParams;

	LPDIRECTSOUNDBUFFER*	sndDSSoundBuffer;	// �o�b�t�@�ւ̃|�C���^

	int						sndVolume;			// �{�����[��
	bool					sndVolumeFlag;

public:
	SoundData();
	~SoundData();
};


class Sound{
public:
	LPDIRECTSOUND8			pDirectSound;				// DirectSound�I�u�W�F�N�g

	LPDIRECTSOUND3DLISTENER	pDSListener;				// 3D���X�i�[
	DS3DLISTENER			DS3DListenerParams;				// 3D���X�i�[�̃p�����[�^

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