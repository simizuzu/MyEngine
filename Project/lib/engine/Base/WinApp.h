#pragma once
#include "SuppressWarning.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <Windows.h>
#include <cstdint>
#include <string>
#include <wrl.h>
MYENGINE_SUPPRESS_WARNINGS_END

class WinApp
{
public: // 静的メンバ関数
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public: // 定数
	// ウィンドウ横幅
	static const int window_height = 720;
	// ウィンドウ縦幅
	static const int window_width = 1280;

public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// メッセージの処理
	/// </summary>
	bool ProccessMessage();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// 解放処理
	/// </summary>
	void Delete();

	// シングルトン
	static WinApp* GetInstance();

public: // ゲッター
	HWND GetHwnd() const;
	HINSTANCE GetHinstance() const;

private: // メンバ変数
	// ウィンドウハンドル
	HWND hwnd = nullptr;
	// ウィンドウクラスの設定
	WNDCLASSEX w{};
	static WinApp* winApp_;

private:
	WinApp();
	~WinApp();
	WinApp& operator=(const WinApp&) = delete;
	WinApp(const WinApp&) = delete;
};
