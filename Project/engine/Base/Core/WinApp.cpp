#include "WinApp.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <imgui_impl_win32.h>
MYENGINE_SUPPRESS_WARNINGS_END

#pragma comment(lib,"winmm.lib")

 /**
 * @file WinApp.cpp
 * @brief WinAppの処理について書いてあります
 */

WinApp::WinApp(){}
WinApp::~WinApp(){}

WinApp* WinApp::winApp_ = nullptr;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

LRESULT WinApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	// ImGui用ウィンドウプロシージャ呼び出し
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
		return true;

	// メッセージで分岐
	switch (msg)
	{
	case WM_DESTROY: // ウィンドウ破棄
		PostQuitMessage(0); // OSに対して、アプリの終了を伝える
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam); // 標準の処理を行う
}

void WinApp::Initialize()
{
	//ウィンドウクラス設定
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc;		// ウィンドウプロシージャを設定
	w.lpszClassName = "Assault";				// ウィンドウクラス名（ヘッダーで名前変更可能）
	w.hInstance = GetModuleHandle(nullptr);		// ウィンドウハンドル
	w.hCursor = LoadCursor(NULL, IDC_ARROW);	// カーソル設定

	// ウィンドウクラスをOSに登録する
	RegisterClassEx(&w);
	// ウィンドウサイズ{ X座標 Y座標 横幅 縦幅 }
	RECT wrc = { 0,0,window_width,window_height };
	// 自動でサイズを補正する
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	//ウィンドウオブジェクトの生成
	hwnd = CreateWindow(
		w.lpszClassName,		// クラス名
		"Assault",				// タイトルバーの文字
		WS_OVERLAPPEDWINDOW,	// 標準的なウィンドウスタイル（ヘッダーで変更可能）
		CW_USEDEFAULT,			// 表示X座標（OSに任せる）
		CW_USEDEFAULT,			// 表示Y座標（OSに任せる）
		wrc.right - wrc.left,	// ウィンドウ横幅
		wrc.bottom - wrc.top,	// ウィンドウ縦幅
		nullptr,				// 親ウィンドウハンドル
		nullptr,				// メニューハンドル
		w.hInstance,			// 呼び出しアプリケーションハンドル
		nullptr);				// 追加パラメータ

	// ウィンドウを表示状態にする
	ShowWindow(hwnd, SW_SHOW);

	// システムタイマーの分解能を上げる
	timeBeginPeriod(1);
}

bool WinApp::ProccessMessage()
{
	MSG msg{};

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	//×ボタンで終了メッセージが来たらゲームループを抜ける
	if (msg.message == WM_QUIT)
	{
		return true;
	}

	return false;
}

void WinApp::Finalize()
{
	// ウィンドウクラスを登録解除
	UnregisterClass(w.lpszClassName, w.hInstance);
}

void WinApp::Delete()
{
	delete winApp_;
}

WinApp* WinApp::GetInstance()
{
	if ( winApp_ == nullptr )
	{
		winApp_ = new WinApp();
	}

	return winApp_;
}

HWND WinApp::GetHwnd() const 
{ 
	return hwnd;
}

HINSTANCE WinApp::GetHinstance() const
{
	return w.hInstance;
}
