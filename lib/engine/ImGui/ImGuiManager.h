#pragma once
#include <wrl.h>

#include "WinApp.h"
#include "DirectXCommon.h"

class ImGuiManager final
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WinApp* winApp, DirectXCommon* dxCommon);

	/// <summary>
	/// ImGui受付開始
	/// </summary>
	void Begin();
	
	/// <summary>
	/// ImGui受付終了
	/// </summary>
	void End();

	/// <summary>
	/// 画面への描画
	/// </summary>
	void Draw(DirectXCommon* dxCommon);

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	// シングルトン
	static ImGuiManager* GetInstance();

private:
	// SRV用デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_;

private:
	ImGuiManager() = default;
	~ImGuiManager() = default;
	ImGuiManager& operator=(const ImGuiManager&) = delete;
	ImGuiManager(const ImGuiManager&) = delete;
};
