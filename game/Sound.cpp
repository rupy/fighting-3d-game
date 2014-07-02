#include "Sound.h"
#include <stdio.h>
#include "Common.h"
#include "Error.h"

SoundData::SoundData()
{
	sndCreationFlags		=0;
	sndNumOfBuffers			= -1;
	sndDSSoundBuffer		= NULL;

	sndDS3DSBuffer			= NULL;
	sndDS3DSBufferParams	= NULL;
	sndVolume				= 0;
	sndVolumeFlag			= true;

}
SoundData::~SoundData(){
		SAFE_DELETE_ARRAY( sndDSSoundBuffer );
		SAFE_DELETE_ARRAY( sndDS3DSBuffer );
		SAFE_DELETE_ARRAY( sndDS3DSBufferParams );
}




Sound::Sound()
:pDirectSound(NULL),pDSListener(NULL)
{
}
Sound::~Sound(){

	for( DWORD i0 = 0 ; i0 < NUM_OF_SOUNDTYPE ; i0++)
	{
//		CloseWaveFile( &SE[i0] );
	}
}

HRESULT Sound::InitDirectSound( HWND hWnd ){

	HRESULT				hr;						//���ʗp
	DSBUFFERDESC		aDSBDesc;					//�o�b�t�@�L�q�q
	WAVEFORMATEX		aWFEX;					//�g�`�I�[�f�B�I�f�[�^�̃t�H�[�}�b�g
	LPDIRECTSOUNDBUFFER	pPrimaryBuffer = NULL;	//�v���C�}���o�b�t�@���m�ۂ��邽�߂̉��̃o�b�t�@

	SAFE_RELEASE( pDirectSound );	//�O�̂��߂̏�����

	// �f�t�H���g�̃T�E���h�f�B�o�C�X���g���ĒT��
	if( FAILED( hr = DirectSoundCreate8( NULL, &pDirectSound, NULL ) ) ){ return( hr );}

	//�������x��
	if( FAILED( hr = pDirectSound->SetCooperativeLevel( hWnd, DSSCL_PRIORITY ) ) ){ return( hr );}

	// �v���C�}���o�b�t�@���擾����
	// �v���C�}���o�b�t�@�̓Z�J���_���o�b�t�@���O�Ɋm�ۂ��Ȃ���΂Ȃ�Ȃ�
	// �܂�DSBCAPS_PRIMARYBUFFER��DSBCAPS_CTRLFX�Ƒg�ݍ��킹�邱�Ƃ͏o���Ȃ�
	ZeroMemory( &aDSBDesc,	sizeof(DSBUFFERDESC) );
    aDSBDesc.dwSize			= sizeof(DSBUFFERDESC);
	aDSBDesc.dwFlags			= DSBCAPS_PRIMARYBUFFER;		// �v���C�}���o�b�t�@�ƃ{�����[��
    aDSBDesc.dwBufferBytes		= 0;							// �v���C�}���o�b�t�@�̏ꍇ�͕K��0
    aDSBDesc.lpwfxFormat		= NULL;							// �v���C�}���o�b�t�@�̏ꍇ�͕K��NULL

	// CreateSoundBuffer�̃`�F�b�N�͕K��FAILED�}�N���ōs��
	if( FAILED( hr = pDirectSound->CreateSoundBuffer( &aDSBDesc, &pPrimaryBuffer, NULL ) ) ){ return ( hr );}

	ZeroMemory( &aWFEX, sizeof(WAVEFORMATEX) ); 			// �\���̂̃[�����Z�b�g�i�K���s���j
	aWFEX.wFormatTag		= (WORD) WAVE_FORMAT_PCM;		// PCM�g�`�t�H�[�}�b�g��p����
	aWFEX.nChannels			= (WORD) PRIMARY_CH;			// �`���l����
	aWFEX.nSamplesPerSec	= (DWORD) PRIMARY_HZ;			//���g��
	aWFEX.wBitsPerSample	= (WORD) PRIMARY_BIT_RATE;		//�r�b�g���[�g
	aWFEX.nBlockAlign		= (WORD) ((aWFEX.wBitsPerSample / 8) * aWFEX.nChannels);
	aWFEX.nAvgBytesPerSec	= (DWORD) (aWFEX.nSamplesPerSec * aWFEX.nBlockAlign);

	// �v���C�}���o�b�t�@�̃t�H�[�}�b�g��ݒ�
	if( FAILED( hr = pPrimaryBuffer->SetFormat(&aWFEX) ) ){ return ( hr );}

	// SetFormat�֐�������ɏI������̂ŕs�v
    SAFE_RELEASE( pPrimaryBuffer );

	// 3D���X�i�[�C���^�t�F�C�X�̎擾
	if( FAILED( hr = Get3DListenerInterface( &pDSListener ))){ return( hr );}

	// �p�����[�^�̎擾
	DS3DListenerParams.dwSize	= sizeof( DS3DLISTENER );//DS3DLISTENER�\���̂��g���ꍇ�́A�܂��������ď���������K�v������
	pDSListener->GetAllParameters( &DS3DListenerParams ); 


	return S_OK;
}


HRESULT Sound::LoadFromWaveFile(char		*aWaveFileName,
								SoundData	*aSound,
								DWORD		aCreationFlags, 
								GUID		guid3DAlgorithm,
								DWORD		aNumOfBuffers ){

	HRESULT		hr;
	char		tmpString[100];
	if( aWaveFileName == NULL){ return( E_FAIL );}	//�t�@�C����������

	LPDIRECTSOUNDBUFFER*	aDSBuffers		= NULL;	//���o�b�t�@
	aDSBuffers = new LPDIRECTSOUNDBUFFER[aNumOfBuffers];//�T�E���h�o�b�t�@�ւ̃|�C���^���m��
	if( aDSBuffers == NULL ){ return( E_FAIL ); }	//�������̊m�ۂɎ��s�H


	//�g�`�t�@�C���̓ǂݍ���
	if( FAILED( hr = OpenWaveFile( aWaveFileName , aSound )))
	{
		sprintf( tmpString , "wav�t�@�C���w%s�x��������܂���ł���", aWaveFileName);
		DisplayErrMsg(tmpString);
		SAFE_DELETE_ARRAY( aDSBuffers );//���̃o�b�t�@��j��
		return( E_FAIL );
	}

	//aCreationFlags��DSBCAPS_CTRL3D���w�肳��Ă�����g�`�t�@�C���̃t�H�[�}�b�g����U�`�F�b�N����
	if(( aCreationFlags & DSBCAPS_CTRL3D )&&
		(( aSound->sndWaveFormatEx.nChannels >1 )||( aSound->sndWaveFormatEx.wFormatTag != WAVE_FORMAT_PCM )))
	{
		sprintf( tmpString , "wav�t�@�C���w%s�x��3D�Đ��ɓK�����t�H�[�}�b�g�ł͂���܂���", aWaveFileName);
		DisplayErrMsg(tmpString);
		SAFE_DELETE_ARRAY( aDSBuffers );//���̃o�b�t�@��j��
		return( E_FAIL );
	}

	// DirectSound�o�b�t�@�̍쐬
	// �n�[�h�E�F�A�̋@�\�Ƃ����p�ł���ꍇ�Ȃǂ̃t���O�̐ݒ���s��

	DSBUFFERDESC	aDSBDesc;	//�o�b�t�@�L�q�q

	ZeroMemory( &aDSBDesc, sizeof(DSBUFFERDESC) );
	aDSBDesc.dwSize          = sizeof(DSBUFFERDESC);		//�����͕K��sizeof(DSBUFFERDESC)
	aDSBDesc.dwFlags         = aCreationFlags;				//�������t���O
	aDSBDesc.dwBufferBytes   = aSound->sndChunk.cksize;		//�o�b�t�@�T�C�Y�i�g�`�t�@�C���̃f�[�^�Ɠ����j
	aDSBDesc.guid3DAlgorithm = guid3DAlgorithm;				//�\�t�g�E�F�A�~�L�V���O�Ŏg���A���S���Y��
	aDSBDesc.lpwfxFormat     = &(aSound->sndWaveFormatEx);	//OepnWaveFile�ŊJ����RIFF�t�@�C���ɓ����Ă���PCM�����t�H�[�}�b�g


	// DirectSound��PCM�f�[�^�ȊO�̍Đ���ۏ؂��Ă��Ȃ��B
	// ����ȊO�̃f�[�^���Đ��ł��邩�ǂ����́A�T�E���h�J�[�h�̃h���C�o����
	// DS_NO_VIRTUALIZATION���A���Ă����ꍇ�́A�T�E���h�o�b�t�@�̓X�e���I�ő�p����Ă���
	// ���̏ꍇ�A�{���Ȃ�Ăяo�����ɒm�点��K�v������
	if(FAILED(hr = pDirectSound->CreateSoundBuffer( &aDSBDesc, &aDSBuffers[0], NULL )))
	{
		DisplayErrMsg("FAIL:CreateSoundBuffer","�Z�J���_���o�b�t�@�쐬�G���[");
		SAFE_DELETE_ARRAY( aDSBuffers );//���̃o�b�t�@��j��
		return( E_FAIL );
	}
 
	//�o�b�t�@�̕���
	//��̉��ɑ΂��ĕ����̃Z�J���_���o�b�t�@���m�ۂ���̂́A��̌��ʉ��������ɍĐ������悤�ȏꍇ�ɑΉ����邽��
	for(size_t i = 0 ; i < aNumOfBuffers ; i++ )
	{
		if( FAILED( hr = pDirectSound->DuplicateSoundBuffer( aDSBuffers[0] , &aDSBuffers[i] )) )
		{
			DisplayErrMsg("FAIL:DuplicateSoundBuffer", "�Z�J���_���o�b�t�@�����G���[");
			SAFE_DELETE_ARRAY( aDSBuffers );//���̃o�b�t�@��j��
			return( E_FAIL );
		}
	}
	
	// theSouns�\���̂ɂ́ALPDIRECTSOUNDBUFFER�ւ̃|�C���^�͗p�ӂ���Ă��邪
	// LPDIRECTSOUNDBUFFER�̎��̂͗p�ӂ���Ă��Ȃ�
	// ������new���Z�q���g���āA�w�肳�ꂽ���̃o�b�t�@���쐬���Ă��̃|�C���^�������n��
	aSound->sndDSSoundBuffer = new LPDIRECTSOUNDBUFFER[aNumOfBuffers];
	if( aSound->sndDSSoundBuffer == NULL )
	{
		DisplayErrMsg("FAIL:aDSSoundBuffer", "�Z�J���_���o�b�t�@�쐬�G���[");
		SAFE_DELETE_ARRAY( aDSBuffers );//���̃o�b�t�@��j��
		return( E_FAIL );
	}

	//�o�b�t�@�̐���theSounds�\���̂ɋL�^����
	aSound->sndNumOfBuffers = aNumOfBuffers;

	//�o�b�t�@�̈�̃R�s�[
	for(size_t i = 0 ; i < aNumOfBuffers ; i++ ){ aSound->sndDSSoundBuffer[i] = aDSBuffers[i]; }

	//�o�b�t�@�ɉ���p�ӂ���
	FillupSoundBuffer( aSound , 0 );

	//aCreationFlags��DSBCAPS_CTRL3D���w�肳��Ă�����Z�J���_���o�b�t�@����3D�o�b�t�@���擾����
	if( aCreationFlags & DSBCAPS_CTRL3D )
	{
		hr = 0;//��񌋉ʃR�[�h�����Z�b�g

		//3D�o�b�t�@�Ƃ��̃p�����[�^�̎��̂�p�ӂ���
		aSound->sndDS3DSBuffer = new LPDIRECTSOUND3DBUFFER[aNumOfBuffers];
		aSound->sndDS3DSBufferParams = new DS3DBUFFER[aNumOfBuffers];

		//�ǂ������Ŏ��s���Ă���A�E�g
		if(( aSound->sndDS3DSBuffer == NULL )||( aSound->sndDS3DSBufferParams == NULL ))
		{
			//�O�̂��ߗ����j������
			SAFE_DELETE_ARRAY( aSound->sndDS3DSBuffer );
			SAFE_DELETE_ARRAY( aSound->sndDS3DSBufferParams );

			DisplayErrMsg("FAIL:sndDS3DSBuffer and/or sndDS3DSBufferParams","3D�T�E���h�G���[");
			SAFE_DELETE_ARRAY( aDSBuffers );//���̃o�b�t�@��j��
			return( E_FAIL );
		}

		//�Z�J���_���o�b�t�@����3D�o�b�t�@�C���^�t�F�C�X���擾���Ă݂�
		for(size_t i = 0 ; i < aNumOfBuffers ; i++ )
		{ hr |= Get3DBufferInterface( aSound->sndDSSoundBuffer[i] , &(aSound->sndDS3DSBuffer[i]) ); }

		//�_���Ȃ�A��
		if(FAILED( hr ) )
		{
			DisplayErrMsg("FAIL:Get3DBufferInterface","3D�T�E���h�G���[");
			SAFE_DELETE_ARRAY( aDSBuffers );//���̃o�b�t�@��j��
			return( E_FAIL );
		}

		//3D�p�����[�^�̐ݒ�
		for( size_t i = 0 ; i < aNumOfBuffers ; i++ )
		{
			aSound->sndDS3DSBufferParams[i].dwSize = sizeof(DS3DBUFFER);//�K����������������
			(aSound->sndDS3DSBuffer[i])->GetAllParameters( &(aSound->sndDS3DSBufferParams[i]) );

			//DS3SMODE_HEADRELATIVE��
			//�w�肷��T�E���h�p�����[�^ (�ʒu�A���x�A����) �����X�i�[�̃p�����[�^�ɑ΂��đ��ΓI�ł��邱�Ƃ�����
			aSound->sndDS3DSBufferParams[i].dwMode = DS3DMODE_HEADRELATIVE;

			//DS3D_IMMEDIATE�́ASetAllParameters�����s���ꂽ�u�ԂɑS�Ẵp�����[�^���C�����邱�Ƃ�����
			(aSound->sndDS3DSBuffer[i])->SetAllParameters( &(aSound->sndDS3DSBufferParams[i]) ,DS3D_IMMEDIATE);

		}
	}

	SAFE_DELETE_ARRAY( aDSBuffers );//���̃o�b�t�@��j��
	return( S_OK );

}

HRESULT Sound::OpenWaveFile( char	*aWaveFileName , SoundData *aSound )
{
	MMCKINFO		aChunkInfo;			//�`�����N�������߂�
	PCMWAVEFORMAT	aPCMWaveFormat;		//���[�h����PCM�̏����ꎞ�I�ɕۑ�����

	aSound->sndHmmio = mmioOpen( aWaveFileName , NULL , MMIO_ALLOCBUF | MMIO_READ);
	if( aSound->sndHmmio == NULL )
	{
		DisplayErrMsg("FAIL:mmioOpen", "Wave�t�@�C���G���[");
		return( E_FAIL );
	}

	// mmioOpen�֐��ŊJ�����g�`�t�@�C����擪����T������B
	if( ( MMSYSERR_NOERROR != mmioDescend( aSound->sndHmmio, &(aSound->sndChunkRiff) , NULL, 0 ) ) )
	{
		DisplayErrMsg("FAIL:mmioDescend", "Wave�t�@�C���G���[");
		CloseWaveFile( aSound );
		return ( E_FAIL );
	}

	// �L���ȃt�@�C�����m�F����
	if( (aSound->sndChunkRiff.ckid != FOURCC_RIFF) || (aSound->sndChunkRiff.fccType != mmioFOURCC('W', 'A', 'V', 'E') ) )
	{
		DisplayErrMsg("FAIL:mmioFOURCC", "Wave�t�@�C���G���[");
		CloseWaveFile( aSound );
		return ( E_FAIL );
	}

	// �����A�L���ȓ��̓t�@�C���ł���΁A'fmt '�`�����N��T��
	aChunkInfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if( MMSYSERR_NOERROR != mmioDescend( aSound->sndHmmio, &aChunkInfo, &(aSound->sndChunkRiff), MMIO_FINDCHUNK ) )
	{
		DisplayErrMsg("FAIL:mmioDescend2", "Wave�t�@�C���G���[");
		CloseWaveFile( aSound );
		return ( E_FAIL );
	}

	// ����'fmt '�`�����N�����ʂȃp�����[�^�����ꍇ�A�`�����N�͏��Ȃ��Ƃ�<PCMWAVEFORMAT>�����傫�Ȃ��̂ɂȂ�
	// �����������ʂȃp�����[�^�͏����ł��Ȃ��̂ŏ����𒆒f����B
	if( aChunkInfo.cksize < (LONG) sizeof(PCMWAVEFORMAT) )
	{
		DisplayErrMsg("FAIL:PCMWAVEFORMAT", "Wave�t�@�C���G���[");
		CloseWaveFile( aSound );
		return ( E_FAIL );
	}

	// 'fmt '�`�����N��<PCMWAVEFORMAT>�֓ǂݏo�����B
	if( mmioRead( aSound->sndHmmio, (HPSTR) &aPCMWaveFormat, sizeof(aPCMWaveFormat)) != sizeof(aPCMWaveFormat) )
	{
		DisplayErrMsg("FAIL:mmioRead", "Wave�t�@�C���G���[");
		CloseWaveFile( aSound );
		return ( E_FAIL );
	}


	// waveformatex��z�u����
	if( aPCMWaveFormat.wf.wFormatTag == WAVE_FORMAT_PCM )
	{
		// PCM�\���̂�waveformatEX�\���̂ɃR�s�[����
		aSound->sndWaveFormatEx.wFormatTag			= aPCMWaveFormat.wf.wFormatTag;
		aSound->sndWaveFormatEx.nChannels			= aPCMWaveFormat.wf.nChannels;
		aSound->sndWaveFormatEx.nSamplesPerSec		= aPCMWaveFormat.wf.nSamplesPerSec;
		aSound->sndWaveFormatEx.nAvgBytesPerSec		= aPCMWaveFormat.wf.nAvgBytesPerSec;
		aSound->sndWaveFormatEx.nBlockAlign			= aPCMWaveFormat.wf.nBlockAlign;
		aSound->sndWaveFormatEx.wBitsPerSample		= aPCMWaveFormat.wBitsPerSample;
		// ��̍\���̂̃R�s�[�́A���̈�s�ōςނ��A������₷����D��
		// memcpy( aSound->sndWaveFormatEx, &aPCMWaveFormat, sizeof(aPCMWaveFormat) );

		aSound->sndWaveFormatEx.cbSize = 0;//�ǉ��t�H�[�}�b�g�ł͂Ȃ��̂�cbSize��0
	}
	else
	{
		// PCM�t�H�[�}�b�g�łȂ�������A��
		DisplayErrMsg("FAIL:mmioRead", "Wave�t�@�C���G���[");
		CloseWaveFile( aSound );
		return ( E_FAIL );
	}

	// ���̓t�@�C����'fmt '�`�����N���甲���o��
	if( MMSYSERR_NOERROR != mmioAscend( aSound->sndHmmio, &aChunkInfo, 0 ) )
	{
		DisplayErrMsg("FAIL:mmioAscend", "Wave�t�@�C���G���[");
		CloseWaveFile( aSound );
		return ( E_FAIL );
	}

	// ����I�����ɂ́AaSound.paWaveFormatEx�Ɏw�肳�ꂽRIFF�t�@�C���ɓ����Ă���PCM�����t�H�[�}�b�g�����߂��Ă���

	if( FAILED(RewindWaveFile( aSound )) )
	{
		DisplayErrMsg("FAIL:RewindWaveFile", "Wave�t�@�C���G���[");
		CloseWaveFile( aSound );
		return ( E_FAIL );
	}

	return( S_OK );
}

HRESULT	Sound::CloseWaveFile( SoundData *aSound )
{
	// �O�̂��߂̃G���[�`�F�b�N
	if( aSound->sndHmmio == NULL){ return( E_FAIL );}

	//�T�E���h�����
	if( FAILED ( mmioClose(aSound->sndHmmio , 0) ))
	{
		DisplayErrMsg("FAIL:mmioClose", "Wave�t�@�C���G���[");
		return( E_FAIL );
	}

	//����I����
	return( S_OK );
}

HRESULT Sound::RewindWaveFile( SoundData *aSound )
{
	// �O�̂��߂̃G���[�`�F�b�N
	if( aSound->sndHmmio == NULL){ return( E_FAIL );}

	// �g�`�t�@�C���̓�����
	// mmioSeek�̓t�@�C���̓�����������
	// SEEK_SET�̓t�@�C���̐擪����̃o�C�g���ŃV�[�N�ʒu���w�肷��
	// ���������ꍇ�͐V�����t�@�C���|�W�V�����̈ʒu��Ԃ��B���s�����-1
	if( -1 == mmioSeek( aSound->sndHmmio, aSound->sndChunkRiff.dwDataOffset + sizeof(FOURCC) , SEEK_SET ) )
	{
		DisplayErrMsg("FAIL:mmioSeek", "Wave�t�@�C���G���[");
		return( E_FAIL );
	}

	// ���̓t�@�C���̃f�[�^�`�����N��T���āA���̃`�����N�̏���aSound.aChunk�Ɏ��߂�
	aSound->sndChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if( MMSYSERR_NOERROR != mmioDescend( aSound->sndHmmio, &(aSound->sndChunk), &(aSound->sndChunkRiff), MMIO_FINDCHUNK ) )
	{
		DisplayErrMsg("FAIL:mmioDescend", "Wave�t�@�C���G���[");
		return( E_FAIL );
	}

	//����I����
	return( S_OK );
}
HRESULT Sound::FillupSoundBuffer( SoundData *aSound , DWORD aLockBuffer )
{
	HRESULT hr;										// ���ʗp

	void*	apLockedBuffer			= NULL;			// ���b�N���ꂽDirectSound�o�b�t�@�ւ̃|�C���^
	DWORD	aLockedBufferSize		= 0;			// ���b�N���ꂽDirectSound�o�b�t�@�̃T�C�Y
	DWORD	aReadedData				= 0;			// �g�`�t�@�C������ǂݏo���ꂽ�f�[�^�̃o�C�g�� 

	if( aSound->sndDSSoundBuffer[aLockBuffer] == NULL )
	{ return (E_FAIL);}

    // �o�b�t�@���m���ɑ��݂��邱�Ƃ��m���߂�B
	// ���������畜�������݂āA����ł��_���Ȃ�A��
	if( FAILED( hr = RestoreDSBuffer( aSound->sndDSSoundBuffer[aLockBuffer], NULL ) ) )
	{
		DisplayErrMsg("FAIL:RestoreBuffer", "DirectSound�G���[");
		return( hr );
	}

	// �o�b�t�@�����b�N����
	if( FAILED( hr =  aSound->sndDSSoundBuffer[aLockBuffer]->Lock( 0, aSound->sndChunk.cksize , &apLockedBuffer , &aLockedBufferSize, NULL, NULL, 0L ) ) )
	{
		DisplayErrMsg("FAIL:Lock", "DirectSound�G���[");
		return( hr );
	}

	// �g�`�t�@�C���̓�����������
    RewindWaveFile( aSound );

	if( FAILED( hr = ReadWaveToBuffer( aSound , (BYTE*)apLockedBuffer , aLockedBufferSize , &aReadedData ) ) )           
	{
		// ���s������A��
		DisplayErrMsg("FAIL:Read", "DirectSound�G���[");
		return( hr );
	}

	if( aReadedData == 0 )
	{
		// �g�`�f�[�^�����������疳���Ŗ��߂�
		FillMemory( (BYTE*) apLockedBuffer, 
					aLockedBufferSize, 
					(BYTE)(aSound->sndWaveFormatEx.wBitsPerSample == 8 ? 128 : 0 ) );
	}
	else if( aReadedData < aLockedBufferSize )
	{
		// �g�`�t�@�C�����J��Ԃ��Ȃ��Ŗ����Ŗ��߂�
		FillMemory(	(BYTE*) apLockedBuffer + aReadedData, 
					aLockedBufferSize - aReadedData, 
					(BYTE)(aSound->sndWaveFormatEx.wBitsPerSample == 8 ? 128 : 0 ) );
	}

    // �K�v���Ȃ��Ȃ����̂ŁA�o�b�t�@���A�����b�N����
	aSound->sndDSSoundBuffer[aLockBuffer]->Unlock( apLockedBuffer, aLockedBufferSize, NULL, 0 );

    return S_OK;
}
HRESULT	Sound::RestoreDSBuffer( LPDIRECTSOUNDBUFFER aDSBuffer , BOOL* pRestoreFlag )
{
	HRESULT hr;

	// �����m�F�t���O������΁A�ŏ��Ƀ��Z�b�g���Ă���
	if( pRestoreFlag != NULL ){ *pRestoreFlag = FALSE; }

	// �o�b�t�@�̏�Ԃ��m�F����
	DWORD aStatus;
	if( FAILED( hr = aDSBuffer->GetStatus( &aStatus ) ) )
	{
		DisplayErrMsg("FAIL:GetStatus", "DirectSound�G���[");
		return( hr );
	}

	// ���X�g���Ă����畜�������݂�
	if( aStatus & DSBSTATUS_BUFFERLOST )
	{
		// �A�v���P�[�V�������A�N�e�B�u�����ꂽ�Ƃ��Ă�
		// DirectSound�͂܂�������A�v���P�[�V�����Ɉ����n���Ă��Ȃ��ꍇ������
		// ���̂Ƃ��o�b�t�@�̕����͎��s����
		// ���̂���DirectSound������������n���܂�Sleep�֐��ŏ������~����
		// ����̈����n����҂�
		do 
		{
			hr = aDSBuffer->Restore();
			if( hr == DSERR_BUFFERLOST ){ Sleep( 10 ); }
		}
        while( ( hr = aDSBuffer->Restore() ) == DSERR_BUFFERLOST );

		// �������������Ă�����t���O��TRUE��
		if( pRestoreFlag != NULL ){ *pRestoreFlag = TRUE;}

	return( S_OK );
	}
	else
	{ return( S_FALSE ); }
}

HRESULT Sound::ReadWaveToBuffer( SoundData *aSound , BYTE* aBuffer, DWORD aSizeToRead, DWORD* aReadedData )
{
	MMIOINFO mmioInfoIn; // aSound->sndHmmio�̌��݂̏�

	if( aSound->sndHmmio == NULL ){ return( E_FAIL );}
	if( aBuffer == NULL || aSizeToRead == NULL ){ return( E_FAIL );}

	*aReadedData = 0;	//���ʁi�ǂݍ��񂾃o�C�g���j��������

	// mmioGetInfo��aSound->sndHmmio�̏���mmioInfoIn�ɔ[�߂�
	if( 0 != mmioGetInfo( aSound->sndHmmio, &mmioInfoIn, 0 ) )
	{
		// ���s������A��
		DisplayErrMsg("FAIL:mmioGetInfo", "Wave�t�@�C���G���[");
		return( E_FAIL );
	}

	// �ǂݍ��ރf�[�^�̃T�C�Y�𒲂ׂ�B
	// �`�����N�̑傫���i�܂�g�`�f�[�^�̃T�C�Y�j���傫��������`�����N�̃T�C�Y�ɍ��킹��
	// cksize�̓`�����N�̃f�[�^�����o�̃T�C�Y(DWORD)�f�[�^�����o�̃T�C�Y�ɂ́A�S�����̃`�����N���ʎq��
	// �f�[�^�����o�̍Ō�ɂ���i�I�v�V���i���p�b�h�o�C�g�j�͊܂܂�Ȃ�
	UINT aNumOfReadBytes = aSizeToRead;
	if( aNumOfReadBytes > aSound->sndChunk.cksize ) { aNumOfReadBytes = aSound->sndChunk.cksize; }

	// ���ۂ̓ǂݏo�����[�v
	for( DWORD i0 = 0; i0 < aNumOfReadBytes; i0++ )
	{
		//�t�@�C����mmioOpen�ɂ���ĊJ���ꂽ����̏�Ԃł�pchNext��pchEndRead�́A��������o�b�t�@�̐擪���w���Ă���
		//���̂��߁A���҂���v���Ă��Ă���ꍇ�́A�܂�mmioAdvance�֐����Ăяo���A�o�b�t�@�̏�Ԃ��X�V����
		if( mmioInfoIn.pchNext == mmioInfoIn.pchEndRead )
		{

			// �t�@�C���̏I���iEOF�j�ɓ��B�����ꍇ�ł��AmmioAdvance�֐��̓G���[��Ԃ��Ȃ��B
			// EOF�ɒB�������ǂ������m���߂�ꍇ�́AmmioAdvance�֐����Ăяo�����u��Łv
			// MMIOINFO�\���̂�pchNext��pchEndRead�������������ׂ�B
			// ����EOF�ɒB���Ă��Ȃ��ꍇ�AmmioAdvance�֐��̓o�b�t�@�̓��e�ƁApchNext��pchEndRead�̒l���X�V����
			if( 0 != mmioAdvance( aSound->sndHmmio, &mmioInfoIn, MMIO_READ ) )
			{
				// ���s������A��
				DisplayErrMsg("FAIL:mmioAdvance", "Wave�t�@�C���G���[");
				return( E_FAIL );
			}

			// �ǂݍ��ރf�[�^�̃T�C�Y����Ŋm�F���Ă���̂Ƀt�@�C���̏I��肪���Ă���
			if( mmioInfoIn.pchNext == mmioInfoIn.pchEndRead )
			{
				// ���s������A��
				DisplayErrMsg("FAIL:mmioInfoIn.pchNext", "Wave�t�@�C���G���[");
				return( E_FAIL );
			}
		}

		// ���ۂ̃R�s�[
		*((BYTE*)aBuffer + i0 ) = *((BYTE*)mmioInfoIn.pchNext);
		mmioInfoIn.pchNext++;
	}

	// mmioOpen�֐����g���ĊJ�����t�@�C���ɂ��āAmmioGetInfo�֐��Ŏ擾���������X�V���܂��B
	// �o�b�t�@�����O���ꂽ���o�͗p�ɊJ���ꂽ�t�@�C���ɑ΂���A�o�b�t�@�̒��ڃA�N�Z�X���I�����邽�߂Ɏg���B
	if( 0 != mmioSetInfo( aSound->sndHmmio , &mmioInfoIn, 0 ) )
	{
		// ���s������A��
		DisplayErrMsg("FAIL:mmioSetInfo", "Wave�t�@�C���G���[");
		return( E_FAIL );
	}


	//����I��
	*aReadedData = aNumOfReadBytes;	//���ʂ������n��
	return( S_OK );
}
HRESULT Sound::Get3DBufferInterface( LPDIRECTSOUNDBUFFER aDSSoundBuffer, LPDIRECTSOUND3DBUFFER* aDS3DBuffer )
{
	if( aDSSoundBuffer == NULL ){ return( E_FAIL );}
 
	*aDS3DBuffer = NULL;

	//
	return( aDSSoundBuffer->QueryInterface( IID_IDirectSound3DBuffer , (void**)aDS3DBuffer ) );
}

HRESULT Sound::Get3DListenerInterface( LPDIRECTSOUND3DLISTENER* aDS3DListener )
{
    HRESULT				hr;							// ���ʕԂ��p
    DSBUFFERDESC		aDSBDesc;					// ���̃v���C�}���o�b�t�@�p�̃o�b�t�@�L�q�q
    LPDIRECTSOUNDBUFFER	tmpPrimaryBuffer	= NULL;	// ���̃v���C�}���o�b�t�@

	// DirectSound������������ĂȂ�����������������������肵����A��
	if(( aDS3DListener == NULL )||( pDirectSound == NULL )){ return (E_FAIL); }

	// �|�C���^�����������Ă���
	*aDS3DListener = NULL;// �|�C���^�̏�����

	// 3D�R���g���[�����ł��邩�m�F���邽�߁A���̃v���C�}���o�b�t�@���쐬����
	ZeroMemory( &aDSBDesc, sizeof(DSBUFFERDESC) );
	aDSBDesc.dwSize		= sizeof(DSBUFFERDESC);						//�o�b�t�@�L�q�q�̃T�C�Y
	aDSBDesc.dwFlags	= DSBCAPS_CTRL3D | DSBCAPS_PRIMARYBUFFER;	//3D�R���g���[���̂ł���v���C�}���o�b�t�@

	// ���̃v���C�}���o�b�t�@�̍쐬
	if( FAILED( hr = pDirectSound->CreateSoundBuffer( &aDSBDesc, &tmpPrimaryBuffer, NULL ) ) )
	{
		DisplayErrMsg("FAIL:CreateSoundBuffer", "3D�T�E���h�G���[");
		return ( E_FAIL );
	}

	// 3D���X�i�[�C���^�t�F�C�X�͍���H
	if( FAILED( hr = tmpPrimaryBuffer->QueryInterface( IID_IDirectSound3DListener, (void**)aDS3DListener ) ) )
	{
		SAFE_RELEASE( tmpPrimaryBuffer );
		DisplayErrMsg("FAIL:QueryInterface", "3D�T�E���h�G���[");
		return ( E_FAIL );
	}

	// �v��Ȃ��Ȃ�������v���C�}���o�b�t�@���J��
	SAFE_RELEASE( tmpPrimaryBuffer );

	return( S_OK );
}


HRESULT Sound::PlayASound( SoundData *aSound , DWORD aPriority ,DWORD aFlag ,BOOL aOverride)
{
	HRESULT	hr;			// ���ʕԂ��p
	BOOL	aRestored;	// �o�b�t�@���������ꂽ�H

	DWORD	aFreeDSB = SelectFreeBuffer( aSound , aOverride );

	if( aSound->sndDSSoundBuffer[aFreeDSB] == NULL )
	{
		DisplayErrMsg("FAIL:SelectFreeBuffer", "Sound�Đ��G���[");
		return( E_FAIL );
	}

	// �o�b�t�@�������Ă��Ȃ����m�F���A�����Ă���ꍇ�͕��������݂�
	if( FAILED( hr = RestoreDSBuffer( aSound->sndDSSoundBuffer[aFreeDSB] , &aRestored )) )
	{
		DisplayErrMsg("FAIL:RestoreDSBuffer", "Sound�Đ��G���[");
		return( E_FAIL );
	}

	// �o�b�t�@���������ꂽ�ꍇ�A�f�[�^�������Ă���\��������̂ŉ��߂ēǂݍ���
	if( aRestored )
	{
		if( FAILED(hr = FillupSoundBuffer( aSound , aFreeDSB)) )
		{
			DisplayErrMsg("FAIL:FillupSoundBuffer", "Sound�Đ��G���[");
			return( E_FAIL );
		}
	}

	// �������͗\��ς�
	// �������̓T�E���h�̃v���C�I���e�B�i�D��x�j�i0x00000000����0xFFFFFFFF�܂ŁA���̑����ق����D��x�������j
	// ��O������DSBPLAY_LOOPING���w�肷��ƃ��[�v�Đ�
	return( aSound->sndDSSoundBuffer[aFreeDSB]->Play( 0 , aPriority , aFlag ) );
}


//-----------------------------------------------------------------------------playA3DSound()
//
//�@3D�T�E���h�̍Đ�
//
//-----------------------------------------------------------------------------
HRESULT Sound::PlayA3DSound(	SoundData	*aSound,
						D3DXVECTOR3	*aPosition,
						D3DXVECTOR3	*aVerocity,
						DWORD		aPriority,
						DWORD		aFlag,
						BOOL		aOverride)
{
	HRESULT	hr;			// ���ʕԂ��p
	BOOL	aRestored;	// �o�b�t�@���������ꂽ�H

	DWORD	aFreeDSB = SelectFreeBuffer( aSound , aOverride );

	// �I�[�o�[���[�h�t���O���g���Ă��Ȃ���Ԃŋ󂫃o�b�t�@���Ȃ�
	if( aFreeDSB == -1 ){ return( S_OK );}

	if( aSound->sndDSSoundBuffer[aFreeDSB] == NULL )
	{
		DisplayErrMsg("FAIL:SelectFreeBuffer", "Sound�Đ��G���[");
		return( E_FAIL );
	}

	// �o�b�t�@�������Ă��Ȃ����m�F���A�����Ă���ꍇ�͕��������݂�
	if( FAILED( hr = RestoreDSBuffer( aSound->sndDSSoundBuffer[aFreeDSB] , &aRestored )) )
	{
		DisplayErrMsg("FAIL:RestoreDSBuffer", "Sound�Đ��G���[");
		return( E_FAIL );
	}

	// �o�b�t�@���������ꂽ�ꍇ�A�f�[�^�������Ă���\��������̂ŉ��߂ēǂݍ���
	if( aRestored )
	{
		if( FAILED(hr = FillupSoundBuffer( aSound , aFreeDSB)) )
		{
			DisplayErrMsg("FAIL:FillupSoundBuffer", "Sound�Đ��G���[");
			return( E_FAIL );
		}
	}

	// 3D�o�b�t�@�ɑ΂���p�����[�^�̐ݒ�Ƃ�
	Setup3DBufferParams( aSound , aFreeDSB , aPosition , aVerocity);

	hr = (aSound->sndDS3DSBuffer[aFreeDSB])->SetAllParameters( &(aSound->sndDS3DSBufferParams[aFreeDSB]) , DS3D_IMMEDIATE );

	//���ۂ̔���
	if( SUCCEEDED( hr ) ){ hr = aSound->sndDSSoundBuffer[aFreeDSB]->Play( 0, aPriority, aFlag ); }

	if( hr != DS_OK){DisplayErrMsg("FAIL:Play", "Sound�Đ��G���[");}

	return( hr );
}
DWORD Sound::SelectFreeBuffer( SoundData *aSound , BOOL aOverride )
{
	DWORD i0;

	// �o�b�t�@��NULL��������A��
	if( aSound->sndDSSoundBuffer[0] == NULL ){ return( E_FAIL );}

	// �o�b�t�@�̐������J��Ԃ�
	for( i0 = 0 ; i0 < aSound->sndNumOfBuffers ; i0++ )
	{
		if( aSound->sndDSSoundBuffer[i0] != NULL )
		{
			DWORD					aStatus;

			// �X�e�[�^�X���`�F�b�N���čĐ����łȂ����̂�T��
			aSound->sndDSSoundBuffer[i0]->GetStatus( &aStatus );
			if(( aStatus & DSBSTATUS_PLAYING ) == 0 ){ break;}
		}
	}

	// ���g�p�o�b�t�@����������i0�̒l��kNumOfBuffer�ƈقȂ�̂ŁAi0��Ԃ�
	if( i0 == aSound->sndNumOfBuffers )
	{
		// �����ŃI�[�o�[���C�h�t���O�������Ă�����o�b�t�@�̒����烉���_���őI��ŕԂ�
		if(aOverride)
		{
			i0 = rand() % (aSound->sndNumOfBuffers);

			//�I�񂾃o�b�t�@�̍Đ����~���Ċ����߂�
			aSound->sndDSSoundBuffer[i0]->Stop();
			aSound->sndDSSoundBuffer[i0]->SetCurrentPosition(0);
		}
		else{ i0 = -1;}
	}

	return( i0 );
}

HRESULT	Sound::Setup3DBufferParams( SoundData *aSound , DWORD aIndexNum , D3DXVECTOR3 *aPosition , D3DXVECTOR3 *aVerocity)
{
	if( aSound->sndNumOfBuffers < aIndexNum ){ return( E_FAIL );}

	//�ʒu�Ƒ��x�̐ݒ�
	aSound->sndDS3DSBufferParams[aIndexNum].vPosition.x = aPosition->x;
	aSound->sndDS3DSBufferParams[aIndexNum].vPosition.y = aPosition->y;
	aSound->sndDS3DSBufferParams[aIndexNum].vPosition.z = aPosition->z;

	aSound->sndDS3DSBufferParams[aIndexNum].vVelocity.x = aVerocity->x;
	aSound->sndDS3DSBufferParams[aIndexNum].vVelocity.y = aVerocity->y;
	aSound->sndDS3DSBufferParams[aIndexNum].vVelocity.z = aVerocity->z;


	//�{���͉����Ƃɕω�������ׂ��萔
	aSound->sndDS3DSBufferParams[aIndexNum].flMaxDistance = 400.0f;
	aSound->sndDS3DSBufferParams[aIndexNum].flMinDistance = 20.0f;

	//�p�����[�^�̐ݒ�
	if( aSound->sndDS3DSBuffer[aIndexNum] )
	{ aSound->sndDS3DSBuffer[aIndexNum]->SetAllParameters( &aSound->sndDS3DSBufferParams[aIndexNum], DS3D_IMMEDIATE );}
	return( S_OK );
}



HRESULT Sound::StopASound( SoundData *aSound )
{
	HRESULT	hr = 0;

	if( aSound->sndDSSoundBuffer == NULL ){ return( E_FAIL );}

	//�S�Ẵo�b�t�@�̉����~�߂�;
	for( DWORD i0 = 0 ; i0 < aSound->sndNumOfBuffers ; i0++ )
	{
		hr |= aSound->sndDSSoundBuffer[i0]->Stop();
	}
	return(hr);
}

HRESULT Sound::ChangeVolume( SoundData *aSound ,long volume)
{
	HRESULT	hr = 0;

	if( aSound->sndDSSoundBuffer == NULL ){ return( E_FAIL );}

	//�S�Ẵo�b�t�@�̉����~�߂�;
	for( DWORD i0 = 0 ; i0 < aSound->sndNumOfBuffers ; i0++ )
	{
		hr |= aSound->sndDSSoundBuffer[i0]->SetVolume(volume);
	}
	return(hr);
}
//�t�F�[�h�A�E�g���I�������false��Ԃ����\�b�h
bool Sound::FadeOutStopASound( SoundData *aSound ,int downVolumeSpeed)
{

	if( aSound->sndDSSoundBuffer == NULL||!aSound->sndVolumeFlag ){ return( false );}
	if(	aSound->sndVolume > -10000){
		aSound->sndVolume-=downVolumeSpeed;
		ChangeVolume(aSound,aSound->sndVolume);
	}else{
		StopASound(aSound);
		aSound->sndVolumeFlag=false;
		return (false);
	}
	return(true);
}