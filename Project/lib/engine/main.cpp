#include <Windows.h>
#include <memory>

#include "MainGame.h"
#include "Framework.h"

#pragma warning(push)
#pragma warning(disable: 4091)
#pragma warning(pop)

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int){

	std::unique_ptr<Framework> game = std::make_unique<MainGame>();

	game->Run();

	return 0;
}
