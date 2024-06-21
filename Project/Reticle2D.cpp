#include "Reticle2D.h"

void Reticle2D::Initialize()
{
	//レティクルのスプライト生成
	reticle_ = std::make_unique<Sprite>();
	texReticle_ = TextureManager::Load("reticle2");


}

void Reticle2D::Update()
{
	//マウス座標を取得する
	GetCursorPos(&mousePosition);

	//クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePosition);

	//マウス座標を2Dレティクルのスプライトに代入する
	reticle_->SetPosiotion({mousePosition.x,mousePosition.y});

	//
}
