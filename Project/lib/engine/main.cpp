#include "SuppressWarning.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <Windows.h>
#include <memory>
MYENGINE_SUPPRESS_WARNINGS_END

#include "MainGame.h"
#include "Framework.h"

 /**
 * @file main.cpp
 * @brief mainの処理について書いてあります
 */

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int){

	std::unique_ptr<Framework> game = std::make_unique<MainGame>();

	game->Run();

	return 0;
}
