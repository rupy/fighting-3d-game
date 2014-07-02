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

	HRESULT				hr;						//結果用
	DSBUFFERDESC		aDSBDesc;					//バッファ記述子
	WAVEFORMATEX		aWFEX;					//波形オーディオデータのフォーマット
	LPDIRECTSOUNDBUFFER	pPrimaryBuffer = NULL;	//プライマリバッファを確保するための仮のバッファ

	SAFE_RELEASE( pDirectSound );	//念のための初期化

	// デフォルトのサウンドディバイスを使って探す
	if( FAILED( hr = DirectSoundCreate8( NULL, &pDirectSound, NULL ) ) ){ return( hr );}

	//協調レベル
	if( FAILED( hr = pDirectSound->SetCooperativeLevel( hWnd, DSSCL_PRIORITY ) ) ){ return( hr );}

	// プライマリバッファを取得する
	// プライマリバッファはセカンダリバッファより前に確保しなければならない
	// またDSBCAPS_PRIMARYBUFFERとDSBCAPS_CTRLFXと組み合わせることは出来ない
	ZeroMemory( &aDSBDesc,	sizeof(DSBUFFERDESC) );
    aDSBDesc.dwSize			= sizeof(DSBUFFERDESC);
	aDSBDesc.dwFlags			= DSBCAPS_PRIMARYBUFFER;		// プライマリバッファとボリューム
    aDSBDesc.dwBufferBytes		= 0;							// プライマリバッファの場合は必ず0
    aDSBDesc.lpwfxFormat		= NULL;							// プライマリバッファの場合は必ずNULL

	// CreateSoundBufferのチェックは必ずFAILEDマクロで行う
	if( FAILED( hr = pDirectSound->CreateSoundBuffer( &aDSBDesc, &pPrimaryBuffer, NULL ) ) ){ return ( hr );}

	ZeroMemory( &aWFEX, sizeof(WAVEFORMATEX) ); 			// 構造体のゼロリセット（必ず行う）
	aWFEX.wFormatTag		= (WORD) WAVE_FORMAT_PCM;		// PCM波形フォーマットを用いる
	aWFEX.nChannels			= (WORD) PRIMARY_CH;			// チャネル数
	aWFEX.nSamplesPerSec	= (DWORD) PRIMARY_HZ;			//周波数
	aWFEX.wBitsPerSample	= (WORD) PRIMARY_BIT_RATE;		//ビットレート
	aWFEX.nBlockAlign		= (WORD) ((aWFEX.wBitsPerSample / 8) * aWFEX.nChannels);
	aWFEX.nAvgBytesPerSec	= (DWORD) (aWFEX.nSamplesPerSec * aWFEX.nBlockAlign);

	// プライマリバッファのフォーマットを設定
	if( FAILED( hr = pPrimaryBuffer->SetFormat(&aWFEX) ) ){ return ( hr );}

	// SetFormat関数が正常に終わったので不要
    SAFE_RELEASE( pPrimaryBuffer );

	// 3Dリスナーインタフェイスの取得
	if( FAILED( hr = Get3DListenerInterface( &pDSListener ))){ return( hr );}

	// パラメータの取得
	DS3DListenerParams.dwSize	= sizeof( DS3DLISTENER );//DS3DLISTENER構造体を使う場合は、まずこうして初期化する必要がある
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
	if( aWaveFileName == NULL){ return( E_FAIL );}	//ファイル名が無い

	LPDIRECTSOUNDBUFFER*	aDSBuffers		= NULL;	//仮バッファ
	aDSBuffers = new LPDIRECTSOUNDBUFFER[aNumOfBuffers];//サウンドバッファへのポインタを確保
	if( aDSBuffers == NULL ){ return( E_FAIL ); }	//メモリの確保に失敗？


	//波形ファイルの読み込み
	if( FAILED( hr = OpenWaveFile( aWaveFileName , aSound )))
	{
		sprintf( tmpString , "wavファイル『%s』が見つかりませんでした", aWaveFileName);
		DisplayErrMsg(tmpString);
		SAFE_DELETE_ARRAY( aDSBuffers );//仮のバッファを破棄
		return( E_FAIL );
	}

	//aCreationFlagsにDSBCAPS_CTRL3Dが指定されていたら波形ファイルのフォーマットを一旦チェックする
	if(( aCreationFlags & DSBCAPS_CTRL3D )&&
		(( aSound->sndWaveFormatEx.nChannels >1 )||( aSound->sndWaveFormatEx.wFormatTag != WAVE_FORMAT_PCM )))
	{
		sprintf( tmpString , "wavファイル『%s』は3D再生に適したフォーマットではありません", aWaveFileName);
		DisplayErrMsg(tmpString);
		SAFE_DELETE_ARRAY( aDSBuffers );//仮のバッファを破棄
		return( E_FAIL );
	}

	// DirectSoundバッファの作成
	// ハードウェアの機能とか利用できる場合などのフラグの設定も行う

	DSBUFFERDESC	aDSBDesc;	//バッファ記述子

	ZeroMemory( &aDSBDesc, sizeof(DSBUFFERDESC) );
	aDSBDesc.dwSize          = sizeof(DSBUFFERDESC);		//ここは必ずsizeof(DSBUFFERDESC)
	aDSBDesc.dwFlags         = aCreationFlags;				//生成時フラグ
	aDSBDesc.dwBufferBytes   = aSound->sndChunk.cksize;		//バッファサイズ（波形ファイルのデータと同じ）
	aDSBDesc.guid3DAlgorithm = guid3DAlgorithm;				//ソフトウェアミキシングで使うアルゴリズム
	aDSBDesc.lpwfxFormat     = &(aSound->sndWaveFormatEx);	//OepnWaveFileで開いたRIFFファイルに入っているPCM音声フォーマット


	// DirectSoundはPCMデータ以外の再生を保証していない。
	// それ以外のデータが再生できるかどうかは、サウンドカードのドライバ次第
	// DS_NO_VIRTUALIZATIONが帰ってきた場合は、サウンドバッファはステレオで代用されている
	// この場合、本来なら呼び出し元に知らせる必要がある
	if(FAILED(hr = pDirectSound->CreateSoundBuffer( &aDSBDesc, &aDSBuffers[0], NULL )))
	{
		DisplayErrMsg("FAIL:CreateSoundBuffer","セカンダリバッファ作成エラー");
		SAFE_DELETE_ARRAY( aDSBuffers );//仮のバッファを破棄
		return( E_FAIL );
	}
 
	//バッファの複製
	//一つの音に対して複数のセカンダリバッファを確保するのは、一つの効果音が同時に再生されるような場合に対応するため
	for(size_t i = 0 ; i < aNumOfBuffers ; i++ )
	{
		if( FAILED( hr = pDirectSound->DuplicateSoundBuffer( aDSBuffers[0] , &aDSBuffers[i] )) )
		{
			DisplayErrMsg("FAIL:DuplicateSoundBuffer", "セカンダリバッファ複製エラー");
			SAFE_DELETE_ARRAY( aDSBuffers );//仮のバッファを破棄
			return( E_FAIL );
		}
	}
	
	// theSouns構造体には、LPDIRECTSOUNDBUFFERへのポインタは用意されているが
	// LPDIRECTSOUNDBUFFERの実体は用意されていない
	// そこでnew演算子を使って、指定された数のバッファを作成してそのポインタを引き渡す
	aSound->sndDSSoundBuffer = new LPDIRECTSOUNDBUFFER[aNumOfBuffers];
	if( aSound->sndDSSoundBuffer == NULL )
	{
		DisplayErrMsg("FAIL:aDSSoundBuffer", "セカンダリバッファ作成エラー");
		SAFE_DELETE_ARRAY( aDSBuffers );//仮のバッファを破棄
		return( E_FAIL );
	}

	//バッファの数をtheSounds構造体に記録する
	aSound->sndNumOfBuffers = aNumOfBuffers;

	//バッファ領域のコピー
	for(size_t i = 0 ; i < aNumOfBuffers ; i++ ){ aSound->sndDSSoundBuffer[i] = aDSBuffers[i]; }

	//バッファに音を用意する
	FillupSoundBuffer( aSound , 0 );

	//aCreationFlagsにDSBCAPS_CTRL3Dが指定されていたらセカンダリバッファから3Dバッファを取得する
	if( aCreationFlags & DSBCAPS_CTRL3D )
	{
		hr = 0;//一回結果コードをリセット

		//3Dバッファとそのパラメータの実体を用意する
		aSound->sndDS3DSBuffer = new LPDIRECTSOUND3DBUFFER[aNumOfBuffers];
		aSound->sndDS3DSBufferParams = new DS3DBUFFER[aNumOfBuffers];

		//どっちかで失敗してたらアウト
		if(( aSound->sndDS3DSBuffer == NULL )||( aSound->sndDS3DSBufferParams == NULL ))
		{
			//念のため両方破棄する
			SAFE_DELETE_ARRAY( aSound->sndDS3DSBuffer );
			SAFE_DELETE_ARRAY( aSound->sndDS3DSBufferParams );

			DisplayErrMsg("FAIL:sndDS3DSBuffer and/or sndDS3DSBufferParams","3Dサウンドエラー");
			SAFE_DELETE_ARRAY( aDSBuffers );//仮のバッファを破棄
			return( E_FAIL );
		}

		//セカンダリバッファから3Dバッファインタフェイスを取得してみる
		for(size_t i = 0 ; i < aNumOfBuffers ; i++ )
		{ hr |= Get3DBufferInterface( aSound->sndDSSoundBuffer[i] , &(aSound->sndDS3DSBuffer[i]) ); }

		//ダメなら帰る
		if(FAILED( hr ) )
		{
			DisplayErrMsg("FAIL:Get3DBufferInterface","3Dサウンドエラー");
			SAFE_DELETE_ARRAY( aDSBuffers );//仮のバッファを破棄
			return( E_FAIL );
		}

		//3Dパラメータの設定
		for( size_t i = 0 ; i < aNumOfBuffers ; i++ )
		{
			aSound->sndDS3DSBufferParams[i].dwSize = sizeof(DS3DBUFFER);//必ずこう初期化する
			(aSound->sndDS3DSBuffer[i])->GetAllParameters( &(aSound->sndDS3DSBufferParams[i]) );

			//DS3SMODE_HEADRELATIVEは
			//指定するサウンドパラメータ (位置、速度、向き) がリスナーのパラメータに対して相対的であることを示す
			aSound->sndDS3DSBufferParams[i].dwMode = DS3DMODE_HEADRELATIVE;

			//DS3D_IMMEDIATEは、SetAllParametersが実行された瞬間に全てのパラメータを修正することを示す
			(aSound->sndDS3DSBuffer[i])->SetAllParameters( &(aSound->sndDS3DSBufferParams[i]) ,DS3D_IMMEDIATE);

		}
	}

	SAFE_DELETE_ARRAY( aDSBuffers );//仮のバッファを破棄
	return( S_OK );

}

HRESULT Sound::OpenWaveFile( char	*aWaveFileName , SoundData *aSound )
{
	MMCKINFO		aChunkInfo;			//チャンク情報を収める
	PCMWAVEFORMAT	aPCMWaveFormat;		//ロードするPCMの情報を一時的に保存する

	aSound->sndHmmio = mmioOpen( aWaveFileName , NULL , MMIO_ALLOCBUF | MMIO_READ);
	if( aSound->sndHmmio == NULL )
	{
		DisplayErrMsg("FAIL:mmioOpen", "Waveファイルエラー");
		return( E_FAIL );
	}

	// mmioOpen関数で開いた波形ファイルを先頭から探索する。
	if( ( MMSYSERR_NOERROR != mmioDescend( aSound->sndHmmio, &(aSound->sndChunkRiff) , NULL, 0 ) ) )
	{
		DisplayErrMsg("FAIL:mmioDescend", "Waveファイルエラー");
		CloseWaveFile( aSound );
		return ( E_FAIL );
	}

	// 有効なファイルか確認する
	if( (aSound->sndChunkRiff.ckid != FOURCC_RIFF) || (aSound->sndChunkRiff.fccType != mmioFOURCC('W', 'A', 'V', 'E') ) )
	{
		DisplayErrMsg("FAIL:mmioFOURCC", "Waveファイルエラー");
		CloseWaveFile( aSound );
		return ( E_FAIL );
	}

	// もし、有効な入力ファイルであれば、'fmt 'チャンクを探す
	aChunkInfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if( MMSYSERR_NOERROR != mmioDescend( aSound->sndHmmio, &aChunkInfo, &(aSound->sndChunkRiff), MMIO_FINDCHUNK ) )
	{
		DisplayErrMsg("FAIL:mmioDescend2", "Waveファイルエラー");
		CloseWaveFile( aSound );
		return ( E_FAIL );
	}

	// もし'fmt 'チャンクが特別なパラメータを持つ場合、チャンクは少なくとも<PCMWAVEFORMAT>よりも大きなものになる
	// そうした特別なパラメータは処理できないので処理を中断する。
	if( aChunkInfo.cksize < (LONG) sizeof(PCMWAVEFORMAT) )
	{
		DisplayErrMsg("FAIL:PCMWAVEFORMAT", "Waveファイルエラー");
		CloseWaveFile( aSound );
		return ( E_FAIL );
	}

	// 'fmt 'チャンクを<PCMWAVEFORMAT>へ読み出しす。
	if( mmioRead( aSound->sndHmmio, (HPSTR) &aPCMWaveFormat, sizeof(aPCMWaveFormat)) != sizeof(aPCMWaveFormat) )
	{
		DisplayErrMsg("FAIL:mmioRead", "Waveファイルエラー");
		CloseWaveFile( aSound );
		return ( E_FAIL );
	}


	// waveformatexを配置する
	if( aPCMWaveFormat.wf.wFormatTag == WAVE_FORMAT_PCM )
	{
		// PCM構造体をwaveformatEX構造体にコピーする
		aSound->sndWaveFormatEx.wFormatTag			= aPCMWaveFormat.wf.wFormatTag;
		aSound->sndWaveFormatEx.nChannels			= aPCMWaveFormat.wf.nChannels;
		aSound->sndWaveFormatEx.nSamplesPerSec		= aPCMWaveFormat.wf.nSamplesPerSec;
		aSound->sndWaveFormatEx.nAvgBytesPerSec		= aPCMWaveFormat.wf.nAvgBytesPerSec;
		aSound->sndWaveFormatEx.nBlockAlign			= aPCMWaveFormat.wf.nBlockAlign;
		aSound->sndWaveFormatEx.wBitsPerSample		= aPCMWaveFormat.wBitsPerSample;
		// 上の構造体のコピーは、次の一行で済むが、分かりやすさを優先
		// memcpy( aSound->sndWaveFormatEx, &aPCMWaveFormat, sizeof(aPCMWaveFormat) );

		aSound->sndWaveFormatEx.cbSize = 0;//追加フォーマットではないのでcbSizeは0
	}
	else
	{
		// PCMフォーマットでなかったら帰る
		DisplayErrMsg("FAIL:mmioRead", "Waveファイルエラー");
		CloseWaveFile( aSound );
		return ( E_FAIL );
	}

	// 入力ファイルの'fmt 'チャンクから抜け出す
	if( MMSYSERR_NOERROR != mmioAscend( aSound->sndHmmio, &aChunkInfo, 0 ) )
	{
		DisplayErrMsg("FAIL:mmioAscend", "Waveファイルエラー");
		CloseWaveFile( aSound );
		return ( E_FAIL );
	}

	// 正常終了時には、aSound.paWaveFormatExに指定されたRIFFファイルに入っていたPCM音声フォーマットが収められている

	if( FAILED(RewindWaveFile( aSound )) )
	{
		DisplayErrMsg("FAIL:RewindWaveFile", "Waveファイルエラー");
		CloseWaveFile( aSound );
		return ( E_FAIL );
	}

	return( S_OK );
}

HRESULT	Sound::CloseWaveFile( SoundData *aSound )
{
	// 念のためのエラーチェック
	if( aSound->sndHmmio == NULL){ return( E_FAIL );}

	//サウンドを閉じる
	if( FAILED ( mmioClose(aSound->sndHmmio , 0) ))
	{
		DisplayErrMsg("FAIL:mmioClose", "Waveファイルエラー");
		return( E_FAIL );
	}

	//正常終了時
	return( S_OK );
}

HRESULT Sound::RewindWaveFile( SoundData *aSound )
{
	// 念のためのエラーチェック
	if( aSound->sndHmmio == NULL){ return( E_FAIL );}

	// 波形ファイルの頭だし
	// mmioSeekはファイルの頭だしをする
	// SEEK_SETはファイルの先頭からのバイト数でシーク位置を指定する
	// 成功した場合は新しいファイルポジションの位置を返す。失敗すると-1
	if( -1 == mmioSeek( aSound->sndHmmio, aSound->sndChunkRiff.dwDataOffset + sizeof(FOURCC) , SEEK_SET ) )
	{
		DisplayErrMsg("FAIL:mmioSeek", "Waveファイルエラー");
		return( E_FAIL );
	}

	// 入力ファイルのデータチャンクを探して、そのチャンクの情報をaSound.aChunkに収める
	aSound->sndChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if( MMSYSERR_NOERROR != mmioDescend( aSound->sndHmmio, &(aSound->sndChunk), &(aSound->sndChunkRiff), MMIO_FINDCHUNK ) )
	{
		DisplayErrMsg("FAIL:mmioDescend", "Waveファイルエラー");
		return( E_FAIL );
	}

	//正常終了時
	return( S_OK );
}
HRESULT Sound::FillupSoundBuffer( SoundData *aSound , DWORD aLockBuffer )
{
	HRESULT hr;										// 結果用

	void*	apLockedBuffer			= NULL;			// ロックされたDirectSoundバッファへのポインタ
	DWORD	aLockedBufferSize		= 0;			// ロックされたDirectSoundバッファのサイズ
	DWORD	aReadedData				= 0;			// 波形ファイルから読み出されたデータのバイト数 

	if( aSound->sndDSSoundBuffer[aLockBuffer] == NULL )
	{ return (E_FAIL);}

    // バッファが確実に存在することを確かめる。
	// 無かったら復旧を試みて、それでもダメなら帰る
	if( FAILED( hr = RestoreDSBuffer( aSound->sndDSSoundBuffer[aLockBuffer], NULL ) ) )
	{
		DisplayErrMsg("FAIL:RestoreBuffer", "DirectSoundエラー");
		return( hr );
	}

	// バッファをロックする
	if( FAILED( hr =  aSound->sndDSSoundBuffer[aLockBuffer]->Lock( 0, aSound->sndChunk.cksize , &apLockedBuffer , &aLockedBufferSize, NULL, NULL, 0L ) ) )
	{
		DisplayErrMsg("FAIL:Lock", "DirectSoundエラー");
		return( hr );
	}

	// 波形ファイルの頭だしをする
    RewindWaveFile( aSound );

	if( FAILED( hr = ReadWaveToBuffer( aSound , (BYTE*)apLockedBuffer , aLockedBufferSize , &aReadedData ) ) )           
	{
		// 失敗したら帰る
		DisplayErrMsg("FAIL:Read", "DirectSoundエラー");
		return( hr );
	}

	if( aReadedData == 0 )
	{
		// 波形データが無かったら無音で埋める
		FillMemory( (BYTE*) apLockedBuffer, 
					aLockedBufferSize, 
					(BYTE)(aSound->sndWaveFormatEx.wBitsPerSample == 8 ? 128 : 0 ) );
	}
	else if( aReadedData < aLockedBufferSize )
	{
		// 波形ファイルを繰り返さないで無音で埋める
		FillMemory(	(BYTE*) apLockedBuffer + aReadedData, 
					aLockedBufferSize - aReadedData, 
					(BYTE)(aSound->sndWaveFormatEx.wBitsPerSample == 8 ? 128 : 0 ) );
	}

    // 必要がなくなったので、バッファをアンロックする
	aSound->sndDSSoundBuffer[aLockBuffer]->Unlock( apLockedBuffer, aLockedBufferSize, NULL, 0 );

    return S_OK;
}
HRESULT	Sound::RestoreDSBuffer( LPDIRECTSOUNDBUFFER aDSBuffer , BOOL* pRestoreFlag )
{
	HRESULT hr;

	// 復旧確認フラグがあれば、最初にリセットしておく
	if( pRestoreFlag != NULL ){ *pRestoreFlag = FALSE; }

	// バッファの状態を確認する
	DWORD aStatus;
	if( FAILED( hr = aDSBuffer->GetStatus( &aStatus ) ) )
	{
		DisplayErrMsg("FAIL:GetStatus", "DirectSoundエラー");
		return( hr );
	}

	// ロストしていたら復旧を試みる
	if( aStatus & DSBSTATUS_BUFFERLOST )
	{
		// アプリケーションがアクティブ化されたとしても
		// DirectSoundはまだ制御をアプリケーションに引き渡していない場合がある
		// このときバッファの復旧は失敗する
		// そのためDirectSoundが制御を引き渡すまでSleep関数で処理を停止させ
		// 制御の引き渡しを待つ
		do 
		{
			hr = aDSBuffer->Restore();
			if( hr == DSERR_BUFFERLOST ){ Sleep( 10 ); }
		}
        while( ( hr = aDSBuffer->Restore() ) == DSERR_BUFFERLOST );

		// 復旧が成功していたらフラグをTRUEに
		if( pRestoreFlag != NULL ){ *pRestoreFlag = TRUE;}

	return( S_OK );
	}
	else
	{ return( S_FALSE ); }
}

HRESULT Sound::ReadWaveToBuffer( SoundData *aSound , BYTE* aBuffer, DWORD aSizeToRead, DWORD* aReadedData )
{
	MMIOINFO mmioInfoIn; // aSound->sndHmmioの現在の状況

	if( aSound->sndHmmio == NULL ){ return( E_FAIL );}
	if( aBuffer == NULL || aSizeToRead == NULL ){ return( E_FAIL );}

	*aReadedData = 0;	//結果（読み込んだバイト数）を初期化

	// mmioGetInfoでaSound->sndHmmioの情報をmmioInfoInに納める
	if( 0 != mmioGetInfo( aSound->sndHmmio, &mmioInfoIn, 0 ) )
	{
		// 失敗したら帰る
		DisplayErrMsg("FAIL:mmioGetInfo", "Waveファイルエラー");
		return( E_FAIL );
	}

	// 読み込むデータのサイズを調べる。
	// チャンクの大きさ（つまり波形データのサイズ）より大きかったらチャンクのサイズに合わせる
	// cksizeはチャンクのデータメンバのサイズ(DWORD)データメンバのサイズには、４文字のチャンク識別子や
	// データメンバの最後にある（オプショナルパッドバイト）は含まれない
	UINT aNumOfReadBytes = aSizeToRead;
	if( aNumOfReadBytes > aSound->sndChunk.cksize ) { aNumOfReadBytes = aSound->sndChunk.cksize; }

	// 実際の読み出しループ
	for( DWORD i0 = 0; i0 < aNumOfReadBytes; i0++ )
	{
		//ファイルがmmioOpenによって開かれた直後の状態ではpchNextとpchEndReadは、いずれもバッファの先頭を指している
		//そのため、両者が一致していている場合は、まずmmioAdvance関数を呼び出し、バッファの状態を更新する
		if( mmioInfoIn.pchNext == mmioInfoIn.pchEndRead )
		{

			// ファイルの終わり（EOF）に到達した場合でも、mmioAdvance関数はエラーを返さない。
			// EOFに達したかどうかを確かめる場合は、mmioAdvance関数を呼び出した「後で」
			// MMIOINFO構造体のpchNextとpchEndReadが等しいか調べる。
			// もしEOFに達していない場合、mmioAdvance関数はバッファの内容と、pchNextとpchEndReadの値を更新する
			if( 0 != mmioAdvance( aSound->sndHmmio, &mmioInfoIn, MMIO_READ ) )
			{
				// 失敗したら帰る
				DisplayErrMsg("FAIL:mmioAdvance", "Waveファイルエラー");
				return( E_FAIL );
			}

			// 読み込むデータのサイズを上で確認しているのにファイルの終わりがきている
			if( mmioInfoIn.pchNext == mmioInfoIn.pchEndRead )
			{
				// 失敗したら帰る
				DisplayErrMsg("FAIL:mmioInfoIn.pchNext", "Waveファイルエラー");
				return( E_FAIL );
			}
		}

		// 実際のコピー
		*((BYTE*)aBuffer + i0 ) = *((BYTE*)mmioInfoIn.pchNext);
		mmioInfoIn.pchNext++;
	}

	// mmioOpen関数を使って開いたファイルについて、mmioGetInfo関数で取得した情報を更新します。
	// バッファリングされた入出力用に開かれたファイルに対する、バッファの直接アクセスを終了するために使う。
	if( 0 != mmioSetInfo( aSound->sndHmmio , &mmioInfoIn, 0 ) )
	{
		// 失敗したら帰る
		DisplayErrMsg("FAIL:mmioSetInfo", "Waveファイルエラー");
		return( E_FAIL );
	}


	//正常終了
	*aReadedData = aNumOfReadBytes;	//結果を引き渡し
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
    HRESULT				hr;							// 結果返し用
    DSBUFFERDESC		aDSBDesc;					// 仮のプライマリバッファ用のバッファ記述子
    LPDIRECTSOUNDBUFFER	tmpPrimaryBuffer	= NULL;	// 仮のプライマリバッファ

	// DirectSoundが初期化されてなかったり引数が無効だったりしたら帰る
	if(( aDS3DListener == NULL )||( pDirectSound == NULL )){ return (E_FAIL); }

	// ポインタを初期化しておく
	*aDS3DListener = NULL;// ポインタの初期化

	// 3Dコントロールができるか確認するため、仮のプライマリバッファを作成する
	ZeroMemory( &aDSBDesc, sizeof(DSBUFFERDESC) );
	aDSBDesc.dwSize		= sizeof(DSBUFFERDESC);						//バッファ記述子のサイズ
	aDSBDesc.dwFlags	= DSBCAPS_CTRL3D | DSBCAPS_PRIMARYBUFFER;	//3Dコントロールのできるプライマリバッファ

	// 仮のプライマリバッファの作成
	if( FAILED( hr = pDirectSound->CreateSoundBuffer( &aDSBDesc, &tmpPrimaryBuffer, NULL ) ) )
	{
		DisplayErrMsg("FAIL:CreateSoundBuffer", "3Dサウンドエラー");
		return ( E_FAIL );
	}

	// 3Dリスナーインタフェイスは作れる？
	if( FAILED( hr = tmpPrimaryBuffer->QueryInterface( IID_IDirectSound3DListener, (void**)aDS3DListener ) ) )
	{
		SAFE_RELEASE( tmpPrimaryBuffer );
		DisplayErrMsg("FAIL:QueryInterface", "3Dサウンドエラー");
		return ( E_FAIL );
	}

	// 要らなくなったからプライマリバッファを開放
	SAFE_RELEASE( tmpPrimaryBuffer );

	return( S_OK );
}


HRESULT Sound::PlayASound( SoundData *aSound , DWORD aPriority ,DWORD aFlag ,BOOL aOverride)
{
	HRESULT	hr;			// 結果返し用
	BOOL	aRestored;	// バッファが復旧された？

	DWORD	aFreeDSB = SelectFreeBuffer( aSound , aOverride );

	if( aSound->sndDSSoundBuffer[aFreeDSB] == NULL )
	{
		DisplayErrMsg("FAIL:SelectFreeBuffer", "Sound再生エラー");
		return( E_FAIL );
	}

	// バッファが失われていないか確認し、失われている場合は復旧を試みる
	if( FAILED( hr = RestoreDSBuffer( aSound->sndDSSoundBuffer[aFreeDSB] , &aRestored )) )
	{
		DisplayErrMsg("FAIL:RestoreDSBuffer", "Sound再生エラー");
		return( E_FAIL );
	}

	// バッファが復旧された場合、データが失われている可能性があるので改めて読み込む
	if( aRestored )
	{
		if( FAILED(hr = FillupSoundBuffer( aSound , aFreeDSB)) )
		{
			DisplayErrMsg("FAIL:FillupSoundBuffer", "Sound再生エラー");
			return( E_FAIL );
		}
	}

	// 第一引数は予約済み
	// 第二引数はサウンドのプライオリティ（優先度）（0x00000000から0xFFFFFFFFまで、数の多いほうが優先度が高い）
	// 第三引数はDSBPLAY_LOOPINGを指定するとループ再生
	return( aSound->sndDSSoundBuffer[aFreeDSB]->Play( 0 , aPriority , aFlag ) );
}


//-----------------------------------------------------------------------------playA3DSound()
//
//　3Dサウンドの再生
//
//-----------------------------------------------------------------------------
HRESULT Sound::PlayA3DSound(	SoundData	*aSound,
						D3DXVECTOR3	*aPosition,
						D3DXVECTOR3	*aVerocity,
						DWORD		aPriority,
						DWORD		aFlag,
						BOOL		aOverride)
{
	HRESULT	hr;			// 結果返し用
	BOOL	aRestored;	// バッファが復旧された？

	DWORD	aFreeDSB = SelectFreeBuffer( aSound , aOverride );

	// オーバーラードフラグを使っていない状態で空きバッファがない
	if( aFreeDSB == -1 ){ return( S_OK );}

	if( aSound->sndDSSoundBuffer[aFreeDSB] == NULL )
	{
		DisplayErrMsg("FAIL:SelectFreeBuffer", "Sound再生エラー");
		return( E_FAIL );
	}

	// バッファが失われていないか確認し、失われている場合は復旧を試みる
	if( FAILED( hr = RestoreDSBuffer( aSound->sndDSSoundBuffer[aFreeDSB] , &aRestored )) )
	{
		DisplayErrMsg("FAIL:RestoreDSBuffer", "Sound再生エラー");
		return( E_FAIL );
	}

	// バッファが復旧された場合、データが失われている可能性があるので改めて読み込む
	if( aRestored )
	{
		if( FAILED(hr = FillupSoundBuffer( aSound , aFreeDSB)) )
		{
			DisplayErrMsg("FAIL:FillupSoundBuffer", "Sound再生エラー");
			return( E_FAIL );
		}
	}

	// 3Dバッファに対するパラメータの設定とか
	Setup3DBufferParams( aSound , aFreeDSB , aPosition , aVerocity);

	hr = (aSound->sndDS3DSBuffer[aFreeDSB])->SetAllParameters( &(aSound->sndDS3DSBufferParams[aFreeDSB]) , DS3D_IMMEDIATE );

	//実際の発声
	if( SUCCEEDED( hr ) ){ hr = aSound->sndDSSoundBuffer[aFreeDSB]->Play( 0, aPriority, aFlag ); }

	if( hr != DS_OK){DisplayErrMsg("FAIL:Play", "Sound再生エラー");}

	return( hr );
}
DWORD Sound::SelectFreeBuffer( SoundData *aSound , BOOL aOverride )
{
	DWORD i0;

	// バッファがNULLだったら帰る
	if( aSound->sndDSSoundBuffer[0] == NULL ){ return( E_FAIL );}

	// バッファの数だけ繰り返し
	for( i0 = 0 ; i0 < aSound->sndNumOfBuffers ; i0++ )
	{
		if( aSound->sndDSSoundBuffer[i0] != NULL )
		{
			DWORD					aStatus;

			// ステータスをチェックして再生中でないものを探す
			aSound->sndDSSoundBuffer[i0]->GetStatus( &aStatus );
			if(( aStatus & DSBSTATUS_PLAYING ) == 0 ){ break;}
		}
	}

	// 未使用バッファがあったらi0の値はkNumOfBufferと異なるので、i0を返す
	if( i0 == aSound->sndNumOfBuffers )
	{
		// 同じでオーバーライドフラグが立っていたらバッファの中からランダムで選んで返す
		if(aOverride)
		{
			i0 = rand() % (aSound->sndNumOfBuffers);

			//選んだバッファの再生を停止して巻き戻す
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

	//位置と速度の設定
	aSound->sndDS3DSBufferParams[aIndexNum].vPosition.x = aPosition->x;
	aSound->sndDS3DSBufferParams[aIndexNum].vPosition.y = aPosition->y;
	aSound->sndDS3DSBufferParams[aIndexNum].vPosition.z = aPosition->z;

	aSound->sndDS3DSBufferParams[aIndexNum].vVelocity.x = aVerocity->x;
	aSound->sndDS3DSBufferParams[aIndexNum].vVelocity.y = aVerocity->y;
	aSound->sndDS3DSBufferParams[aIndexNum].vVelocity.z = aVerocity->z;


	//本来は音ごとに変化させるべき定数
	aSound->sndDS3DSBufferParams[aIndexNum].flMaxDistance = 400.0f;
	aSound->sndDS3DSBufferParams[aIndexNum].flMinDistance = 20.0f;

	//パラメータの設定
	if( aSound->sndDS3DSBuffer[aIndexNum] )
	{ aSound->sndDS3DSBuffer[aIndexNum]->SetAllParameters( &aSound->sndDS3DSBufferParams[aIndexNum], DS3D_IMMEDIATE );}
	return( S_OK );
}



HRESULT Sound::StopASound( SoundData *aSound )
{
	HRESULT	hr = 0;

	if( aSound->sndDSSoundBuffer == NULL ){ return( E_FAIL );}

	//全てのバッファの音を止める;
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

	//全てのバッファの音を止める;
	for( DWORD i0 = 0 ; i0 < aSound->sndNumOfBuffers ; i0++ )
	{
		hr |= aSound->sndDSSoundBuffer[i0]->SetVolume(volume);
	}
	return(hr);
}
//フェードアウトが終わったらfalseを返すメソッド
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