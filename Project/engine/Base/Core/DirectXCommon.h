#pragma once
#include "SuppressWarning.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <d3d12.h>
#include <vector>
#include <dxgi1_6.h>
MYENGINE_SUPPRESS_WARNINGS_END

#include "WinApp.h"

/**
 * @class DirectXCommon.h
 * @brief DirectX12の基盤クラス
 */

/// <summary>
/// DirectX12基盤
/// </summary>
class DirectXCommon
{
private:
	// DirectX 初期化処理
	HRESULT result;
	int8_t pad1[ 4 ];

	//デバイス
	Microsoft::WRL::ComPtr<ID3D12Device> device;
	Microsoft::WRL::ComPtr<ID3D12DebugDevice> debugDevice;
	//DXGIファクトリー
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory;
	//スワップチェーン
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain;
	//コマンドアロケータ
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> cmdAllocator;
	//コマンドリスト
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;
	//コマンドキュー
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue;
	//レンダーターゲットビューヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap;
	//フェンス
	Microsoft::WRL::ComPtr<ID3D12Fence> fence;
	//深度バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff;
	//深度ビュー
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap;
	//バックバッファ
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

	static DirectXCommon* dxcommon;

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

	//WinAppポインタ
	WinApp* winApp_ = nullptr;

public:
	static DirectXCommon* GetInstance();

	//初期化
	void Initialize();
	//描画開始
	void PreDraw(WinApp* winApp);
	//描画終了
	void PostDraw();
	//コマンド処理を実行
	void ExecuteCommand(bool flip = true);
	void ResetCommand();

	void Finalize();

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

private:
	DirectXCommon() = default;
	~DirectXCommon() = default;
	DirectXCommon& operator=(const DirectXCommon&) = delete;
	DirectXCommon(const DirectXCommon&) = delete;
};
