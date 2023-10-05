#pragma once
#include "SuppressWarning.h"
MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <forward_list>
MYENGINE_SUPPRESS_WARNINGS_END

#include "Pipeline.h"

#include "WorldTransform.h"


class ParticleManager
{
public:
	static void StaticInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList);

private:
	void InitializeDescriptorHeap();
	static void CreateCraphicsPipeline();

public:

	void Initialize();
	
	void Draw(WorldTransform* transform);

	void Update();

	void LoadTexture(const wchar_t* texturename);
	void CreateModel();

	void Add(int life, MyMath::Vector3 position, MyMath::Vector3 velocity, MyMath::Vector3 accel, float start_scale, float end_scale, MyMath::Vector4 s_color, MyMath::Vector4 e_color);

	void RandParticle();

private:
	static const int vertexCount = 1024;	// 頂点数

private:
	// デバイス
	static Microsoft::WRL::ComPtr<ID3D12Device> device_;
	// コマンドリスト
	static Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList_;
	// パイプラインステート
	static RootsigSetPip pip;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;
	Microsoft::WRL::ComPtr<ID3D12Resource> texbuff;
	D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	UINT descriptorHandleIncrementSize;
	// 頂点データ配列
	VertexPos vertices[vertexCount];
	int8_t pad1[4 ];

	D3D12_VERTEX_BUFFER_VIEW vbView;

	MyMath::Matrix4 matView;
	MyMath::Matrix4 matProjection;

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;
	std::forward_list<Particle> particles;

	//代入演算子削除
	ParticleManager& operator=(const ParticleManager&) = delete;

	//コピーコンストラクタ削除
	ParticleManager(const ParticleManager&) = delete;
};

