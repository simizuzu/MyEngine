#include "FbxModel.h"

void FbxModel::CreateBuffers(ID3D12Device* device)
{
	HRESULT result;

	//頂点データ全体のサイズ
	UINT sizeVB = static_cast<UINT>(sizeof(FBXVertexPosNormalUv) * vertices.size());

	//頂点バッファ生成
	CD3DX12_HEAP_PROPERTIES VBHEAP_PROPERTIES = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC VBRESOURCE_DESC = CD3DX12_RESOURCE_DESC::Buffer(sizeVB);

	result = device->CreateCommittedResource(
		&VBHEAP_PROPERTIES,
		D3D12_HEAP_FLAG_NONE,
		&VBRESOURCE_DESC,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));

	//頂点バッファへのデータ転送
	FBXVertexPosNormalUv* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		std::copy(vertices.begin(), vertices.end(), vertMap);
		vertBuff->Unmap(0, nullptr);
	}

	//頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(vertices[0]);

	//頂点インデックス全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());

	//インデックスバッファ生成
	CD3DX12_HEAP_PROPERTIES IBHEAP_PROPERTIES = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC IBRESOURCE_DESC = CD3DX12_RESOURCE_DESC::Buffer(sizeIB);

	result = device->CreateCommittedResource(
		&VBHEAP_PROPERTIES,
		D3D12_HEAP_FLAG_NONE,
		&VBRESOURCE_DESC,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));

	unsigned short* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(result)) {
		std::copy(indices.begin(), indices.end(), indexMap);
		indexBuff->Unmap(0, nullptr);
	}

	//インデックスバッファビューの作成
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

	//テクスチャ画像データ
	const DirectX::Image* img = scratchImg.GetImage(0, 0, 0);
	assert(img);

	//リソース設定
	CD3DX12_RESOURCE_DESC texersDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels);

	//テクスチャ用バッファの生成
	CD3DX12_HEAP_PROPERTIES TEXHEAP_PROPERTIES = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,D3D12_MEMORY_POOL_L0);
	result = device->CreateCommittedResource(
		&TEXHEAP_PROPERTIES,
		D3D12_HEAP_FLAG_NONE,
		&texersDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff));

	//テクスチャバッファにデータ転送
	result = texBuff->WriteToSubresource(
		0,
		nullptr,				//全領域へコピー
		img->pixels,			//元データアドレス
		(UINT)img->rowPitch,	//1ラインサイズ
		(UINT)img->slicePitch	//1枚サイズ
	);

	//SRV用デスクリプタヒープを生成
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = 1;
	result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeapSRV));

	//シェーダーリソースビューの作成
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; //設定構造体
	D3D12_RESOURCE_DESC resDesc = texBuff->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; //2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView(
		texBuff.Get(),	//ビューと関連付けるバッファ
		&srvDesc,		//テクスチャ設定情報
		descHeapSRV->GetCPUDescriptorHandleForHeapStart()); //ヒープの先頭アドレス
}

void FbxModel::Draw(ID3D12GraphicsCommandList* cmdList)
{
	//貯点バッファをセット(VBV)
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	//インデックスヴぁっふぁをせっと(IBV)
	cmdList->IASetIndexBuffer(&ibView);

	//デスクリプタヒープのセット
	ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//シェーダリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(1, descHeapSRV->GetGPUDescriptorHandleForHeapStart());

	//描画コマンド
	cmdList->DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0);
}

const MyMath::Matrix4& FbxModel::GetModelTransform()
{
	return meshNode->globalTransform;
}
