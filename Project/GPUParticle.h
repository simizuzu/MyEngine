#pragma once
#include "SuppressWarning.h"
#include "DirectX12Math.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <memory>
#include <d3d12.h>
#include <wrl.h>
MYENGINE_SUPPRESS_WARNINGS_END

class GPUParticle
{
private:

	Microsoft::WRL::ComPtr<ID3D12Resource> drawList_;
	Microsoft::WRL::ComPtr<ID3D12Resource> freeList_;
	Microsoft::WRL::ComPtr<ID3D12Resource> particlePool_;

	//エミッター用定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffEmit_;
	//ビルボード用定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffBill_;

	//デスクヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap = nullptr;

	//フリーリストパイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> freeListPipState_;
	//フリーリストルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> freeListRootSig_;

	//エミッターパイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> emitPipState_;
	//エミッタールートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> emitRootSig_;

	//アップデート用パイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> updatePipState_;
	//アップデート用ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> updateRootSig_;


	D3D12_GPU_DESCRIPTOR_HANDLE freeListUavHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE particlePoolHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE DrawListUavHandle;

	//パーティクルの最大数
	size_t MAX_PARTICLE_;

private:
	struct ParticleData
	{
		//色
		MyMath::Vector4 color = { 1,1,1,1 };
		//座標
		MyMath::Vector3 position = { 0,0,0 };
		//経過時間
		float AGE;
		//速度
		MyMath::Vector3 velocity;
		//生存しているか
		float isAlive;
		//加速度
		MyMath::Vector3 accel;
		//サイズ
		float size;
		//ライフタイム
		float lifeTime;

		//パディング
		MyMath::Vector3 pad;
	};

	struct BillboardData
	{
		MyMath::Matrix4 mat;// 3D変換行列
		MyMath::Matrix4 billMat;
	};

	struct EmitData
	{
		//座標
		MyMath::Vector3 pos = {};
		//寿命
		float lifeTime;
		//加速度
		MyMath::Vector3 accel = {};
		//スケール
		float scale = 1;
		//開始時の色
		MyMath::Vector4 startColor;
		//終了時の色
		MyMath::Vector4 endColor;
		//速度
		MyMath::Vector3 velocity;
		//経過時間
		float deltaTime;
		//パーティクルの最大数
		uint32_t MAX_PARTICLE;
	};

private:
	//エミットデータ
	EmitData emitData;
	//ビルボードデータ
	BillboardData billboradData;

	//エミッターのカウンタ
	float emitTimeCounter = 1000.0f;
	//生成する時間
	float timeBetweenEmit = 1000.0f;
	int32_t pad[3 ];

public:
	void Initialize(size_t MAX_PARTICLE);

	void Update(float deltaTime);

	void Draw();

private:
	//エミッター用定数バッファ生成
	void CreateConstBufferEmit();
	//ビルボード用定数バッファ生成
	void CreateConstBufferBillboard();

	void CreateParticlePoolBuff();

	void ConstMapEmit();
	void ConstMapBillboard();

	//パイプライン
	void CreateFreeListPipeline();
	void CreateEmitPipeline();
	void CreateUpdatePipeline();
};

