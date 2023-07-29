#include "ImGuiManager.h"
#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>

void ImGuiManager::Initialize(WinApp* winApp, DirectXCommon* dxCommon)
{
	HRESULT result;

	// ImGuiのコンテキストを生成
	ImGui::CreateContext();
	// Imguiのスタイルを設定
	ImGui::StyleColorsDark();

	// Win32用の初期化関数
	ImGui_ImplWin32_Init(winApp->GetHwnd());

	// デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NumDescriptors = 1;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	// デスクリプタヒープ生成
	result = dxCommon->GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&srvHeap_));
	assert(SUCCEEDED(result));

	// DirectX12用初期化関数
	ImGui_ImplDX12_Init(
		dxCommon->GetDevice(),
		static_cast<int>(dxCommon->GetBackBufferCount()),
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, srvHeap_.Get(),
		srvHeap_->GetCPUDescriptorHandleForHeapStart(),
		srvHeap_->GetGPUDescriptorHandleForHeapStart()
	);

	// フォントの追加
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontDefault();
}

void ImGuiManager::Begin()
{
	// ImGuiフレーム開始
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImGuiManager::End()
{
	// 描画前準備
	ImGui::Render();
}

void ImGuiManager::Draw(DirectXCommon* dxCommon)
{
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

	// デスクリプタヒープの配列をセットするコマンド
	ID3D12DescriptorHeap* ppHeaps[] = { srvHeap_.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// 描画コマンドを発行
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), cmdList);
}

void ImGuiManager::Finalize()
{
	// 終了処理
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	// デスクリプタヒープを解放
	srvHeap_.Reset();
}

ImGuiManager* ImGuiManager::GetInstance()
{
	static ImGuiManager imGuiManager;

	return &imGuiManager;
}
