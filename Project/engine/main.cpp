#include "SuppressWarning.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <Windows.h>
#include <memory>
#include<dxgidebug.h>
MYENGINE_SUPPRESS_WARNINGS_END

#include "MainGame.h"
#include "Framework.h"

#pragma comment(lib,"dxguid.lib")
#pragma    comment(lib,"d3dcompiler.lib")
#pragma    comment(lib, "d3d12.lib")
#pragma    comment(lib,"dxgi.lib")

 /**
 * @file main.cpp
 * @brief mainの処理について書いてあります
 */

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int){

	std::unique_ptr<Framework> game = std::make_unique<MainGame>();

	game->Run();

#ifdef _DEBUG
	//リソースリークチェック
	IDXGIDebug1* debug;
	if ( SUCCEEDED(DXGIGetDebugInterface1(0,IID_PPV_ARGS(&debug))) )
	{
		debug->ReportLiveObjects(DXGI_DEBUG_ALL,DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_APP,DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_D3D12,DXGI_DEBUG_RLO_ALL);
		debug->Release();
	}
#endif

	return 0;
}
