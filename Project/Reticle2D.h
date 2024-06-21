#pragma once
#include "DirectX12Math.h"
#include "WinApp.h"
#include "Sprite.h"
#include "TextureManager.h"

class Reticle2D
{
public:
	void Initialize();

	void Update();

	void Draw();

private:
	std::unique_ptr<Sprite> reticle_;
	TextureData texReticle_;

private:
	POINT mousePosition;
};

