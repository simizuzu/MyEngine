#pragma once
#include "Camera.h"
#include <array>
#include <cstdint>
#include <wrl.h>
#include <memory>
#include <string>

class PrimitiveDrawer
{
public:
	
	static const UINT MaxLineCount = 4096;
	
	static const UINT VertexCountLine = 2;
	
	static const UINT IndexCountLine = 0;

	enum class BlendMode {
		None,		
		Normal,		
		Add,		
		Subtract,	
		Multily,	
		Screen,		

		
		CountOfBlendMode,
	};

	
	struct VertexPosColor
	{
		MyMath::Vector3 pos;	
		MyMath::Vector4 color;	
	};


	struct PipelineSet
	{
		
		Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
		
		Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	};

	
	struct Mesh 
	{
		
		Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;
		
		Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff;
		
		D3D12_VERTEX_BUFFER_VIEW vbView{};
		
		D3D12_INDEX_BUFFER_VIEW ibView{};
		
		VertexPosColor* vertMap = nullptr;
		
		uint16_t* indexMap = nullptr;
	};


	static PrimitiveDrawer* GetInstance();


	Microsoft::WRL::ComPtr<ID3D12Resource> CreateCommittedResource(UINT64 size);


	std::unique_ptr<Mesh> CreateMesh(UINT vertexCount, UINT indexCount);


	void Initialize();


	void DrawLine3d(const MyMath::Vector3& p1, const MyMath::Vector3& p2, const MyMath::Vector4& color);


	void Reset();


	void SetViewProjection(const ViewProjection* viewProjection) { viewProjection_ = viewProjection; }

private:
	PrimitiveDrawer() = default;
	~PrimitiveDrawer() = default;
	PrimitiveDrawer(const PrimitiveDrawer&) = delete;
	PrimitiveDrawer& operator=(const PrimitiveDrawer&) = delete;

	std::unique_ptr<PipelineSet>
		CreateGraphicsPipeline(D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType, BlendMode blendMode);


	void CreateGraphicsPipelines();

	void CreateMeshes();


	std::unique_ptr<Mesh> line_;

	uint32_t indexLine_ = 0;

	const ViewProjection* viewProjection_ = nullptr;

	BlendMode blendMode_ = BlendMode::Normal;

	std::array<std::unique_ptr<PipelineSet>, (uint16_t)BlendMode::CountOfBlendMode> pipelineSetLines_;
};

