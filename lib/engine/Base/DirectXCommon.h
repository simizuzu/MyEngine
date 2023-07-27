#pragma once
#include <d3d12.h>
#include <vector>
#include <dxgi1_6.h>

#include "WinApp.h"

class DirectXCommon
{
private:
	// DirectX 初期化処理
	HRESULT result;
	Microsoft::WRL::ComPtr<ID3D12Device> device;
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory;
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> cmdAllocator;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap;
	Microsoft::WRL::ComPtr<ID3D12Fence> fence;
	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap;
	std::vector< Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffers;

	// スワップチェーンの設定
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{}; // 外に出さなきゃエラー起きる
	// デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;
	// フェンスの生成
	UINT64 fenceVal = 0;
	//バリアーデスク
	D3D12_RESOURCE_BARRIER barrierDesc{};
	// ビューポート
	D3D12_VIEWPORT viewport{};
	// シザー矩形
	D3D12_RECT scissorRect{};
	// 背景色
	FLOAT clearColor[4] = { 0.1f,0.25f, 0.5f,0.0f }; // 黄緑色

	// DXGIまわり初期化
	void InitializeDXGI();
	// 最終的なレンダーターゲットの生成
	void InitializeRtv();
	// スワップチェインの生成
	void InitializeSwapChain();
	// コマンドまわり初期化
	void InitializeCommand();
	// フェンス生成
	void InitializeFence();
	// 深度バッファの初期化
	void InitializeDepthBuffer();
	//デバッグレイヤーを有効にする
	void EnableDebugLayer();
	void BreakOnSeverity();

	WinApp* winApp_ = nullptr;

public:
	static DirectXCommon* GetInstance();

	// DirectX毎フレーム処理ここから
	void Initialize();
	void PreDraw(WinApp* winApp);
	void PostDraw();
	void ExecuteCommand();
	// DirectX毎フレーム処理ここまで

	// セッター
	//背景色変更(RGBA)
	void SetBackScreenColor(float red, float green, float blue, float alpha);

	// ゲッター
	ID3D12Device* GetDevice();
	IDXGIFactory7* GetDxgiFactory();
	IDXGISwapChain4* GetSwapChain();
	ID3D12CommandAllocator* GetCmdAllocator();
	ID3D12GraphicsCommandList* GetCommandList();
	ID3D12CommandQueue* GetCommandQueue();
	ID3D12DescriptorHeap* GetRtvHeap();
	ID3D12Fence* GetFence();
	UINT64 GetFenceVal();

	size_t GetBackBufferCount() const;

	/// <summary>
	/// 解放処理
	/// </summary>
	void Delete();
private:
	DirectXCommon() = default;
	~DirectXCommon() = default;
	DirectXCommon& operator=(const DirectXCommon&) = delete;
	DirectXCommon(const DirectXCommon&) = delete;
};
