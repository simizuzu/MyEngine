#pragma once
#include "SuppressWarning.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <Windows.h>
#include <vector>
#include <string>
#include <list>
#include <wrl.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <xaudio2.h>
#include <system_error>
MYENGINE_SUPPRESS_WARNINGS_END

#pragma comment(lib,"xaudio2.lib")
#pragma comment(lib, "Mf.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "Mfreadwrite.lib")
#pragma comment(lib, "mfuuid.lib")

/**
 * @class AudioManager.h
 * @brief サウンド関連のクラス
 */

/// <summary>
/// オーディオデータ
/// </summary>
class AudioData
{
public:
	std::string filePass;
	IMFSourceReader* pMFSourceReader = nullptr;
	WAVEFORMATEX* waveFormat = nullptr;
	std::vector<BYTE> mediaData;
	char* pBuffer = nullptr;
	IXAudio2SourceVoice* pSourceVoice = nullptr;
	float volume = 1.0f;
	bool loop = false;
	bool playTrigger = false;
	char PADDING2[2]{};
	AudioData(std::string FilePass);
	void Unload();
};

/// <summary>
/// XAudio2Voiceコールバック
/// </summary>
class XAudio2VoiceCallback : public IXAudio2VoiceCallback
{
public:

	virtual ~XAudio2VoiceCallback()
	{
	};

	//ボイス処理パスの開始時
	STDMETHOD_(void, OnVoiceProcessingPassStart)(THIS_ UINT32 BytesRequired)
	{
		static_cast<void>(BytesRequired);
	};

	//ボイス処理パスの終了時
	STDMETHOD_(void, OnVoiceProcessingPassEnd) (THIS)
	{
	};

	//バッファストリームの再生が終了したとき
	STDMETHOD_(void, OnStreamEnd) (THIS)
	{
	};

	//バッファの使用開始時
	STDMETHOD_(void, OnBufferStart) (THIS_ void* pBufferContext)
	{
		static_cast<void>(pBufferContext);

	};

	//バッファの末尾に達した時
	STDMETHOD_(void, OnBufferEnd) (THIS_ void* pBufferContext)
	{
		static_cast<void>(pBufferContext);
	};

	//再生がループ位置に達した時
	STDMETHOD_(void, OnLoopEnd) (THIS_ void* pBufferContext)
	{
		static_cast<void>(pBufferContext);
	};

	//ボイスの実行エラー時
	STDMETHOD_(void, OnVoiceError) (THIS_ void* pBufferContext, HRESULT Error)
	{
		static_cast<void>(pBufferContext);
		static_cast<void>(Error);
	};
};

struct PlayAudioArray
{
	std::vector<uint32_t>handles;
	int32_t nowIdx = 0;
	char PADDING[4]{};
	PlayAudioArray(const std::vector<uint32_t>& Handles);
};

/// <summary>
/// オーディオマネージャ
/// </summary>
class AudioManager
{
private:
	static AudioManager* instance;
	Microsoft::WRL::ComPtr<IXAudio2>xAudio2;
	IXAudio2MasteringVoice* masterVoice = nullptr;
	XAudio2VoiceCallback voiceCallback;
	std::list<AudioData>audios;
	std::vector<PlayAudioArray>playHandleArray;

public:
	//インスタンス生成
	static AudioManager* GetInstance();
	//インスタンス破棄
	void Destroy();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 今音は流れているのか
	/// </summary>
	/// <param name="handle">ハンドル</param>
	/// <returns>ture or false</returns>
	bool NowPlay(const uint32_t& handle);

	/// <summary>
	/// 音のボリューム調整
	/// </summary>
	/// <param name="handle">ハンドル名</param>
	/// <param name="volume">音量</param>
	void ChangeVolume(const uint32_t& handle, float volume);

	/// <summary>
	/// 音のボリュームを取得
	/// </summary>
	/// <param name="handle">ハンドル</param>
	/// <returns>音量</returns>
	float GetVolume(const uint32_t& handle);

	/// <summary>
	/// ファイルから音の読み込み
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	/// <param name="volume">音量(デフォルトは1.0f)</param>
	/// <returns>ハンドル</returns>
	uint32_t LoadAudio(std::string fileName, const float& volume = 1.0f);

	/// <summary>
	/// 音楽を再生
	/// </summary>
	/// <param name="handle">ハンドル名</param>
	/// <param name="loopFlag">ループさせるか(デフォルトはfalse)</param>
	/// <returns>再生できている</returns>
	int32_t PlayWave(const uint32_t& handle, bool loopFlag = false);

	/// <summary>
	/// 音楽を再生配列
	/// </summary>
	/// <param name="handles">ハンドル名</param>
	/// <returns>再生できている</returns>
	int32_t PlayWaveArray(const std::vector<uint32_t>& handles);

	/// <summary>
	/// 音楽を止める
	/// </summary>
	/// <param name="handle">ハンドル名</param>
	void StopWave(const uint32_t& handle);

	/// <summary>
	/// stringからwstringへ
	/// </summary>
	/// <param name="string">string</param>
	/// <returns>std::wstring(dest.begin(), dest.end())</returns>
	std::wstring StringToWstring(const std::string& string);

private:
	AudioManager() = default;
	~AudioManager();

	//コピーコンストラクタ・代入演算子削除
	AudioManager& operator=(const AudioManager&) = delete;
	AudioManager(const AudioManager&) = delete;
};
