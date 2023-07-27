﻿#include "ObjModel.h"

#include <DirectXTex.h>
#include <fstream>
#include <sstream>
#include <cassert>

/// <summary>
/// 静的メンバ変数の実態
/// </summary>
Microsoft::WRL::ComPtr<ID3D12Device> ObjModel::device;
Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> ObjModel::descHeap;

ObjModel* ObjModel::LoadFromObj(const std::string& modelname, bool smoothing)
{
	// 新たなModel型のインスタンスをnewする
	ObjModel* model_ = new ObjModel();
	// デバイスを代入
	device = DirectXCommon::GetInstance()->GetDevice();
	// デスクリプタヒープの生成
	model_->InitializeDescriptorHeap();
	// 読み込み
	model_->LoadFromOBJInternal(modelname,smoothing);
	// バッファ生成
	model_->CreateBuffers();

	return model_;
}

void ObjModel::LoadMaterial(const std::string& directoryPath, const std::string& filename)
{
	// ファイルストリーム
	std::ifstream file;
	// マテリアルファイルを開く
	file.open(directoryPath + filename);
	// ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0);
	}

	// 1行ずつ読み込む
	std::string line;
	while (getline(file, line))
	{
		// 1行分の文字列をストリームに変換
		std::istringstream line_stream(line);

		// 半角スペース区切りで行の先頭文字列を取得
		std::string key;
		getline(line_stream, key, ' ');

		// 先頭の文字は無視する
		if (key[0] == '\t')
		{
			key.erase(key.begin()); // 先頭の文字を削除
		}

		// 先頭文字列がnewmtlならマテリアル名
		if (key == "newmtl")
		{
			// マテリアル名読み込み
			line_stream >> material.name;
		}
		// 先頭文字列がKaならアンビエント色
		if (key == "Ka")
		{
			line_stream >> material.ambient.x;
			line_stream >> material.ambient.y;
			line_stream >> material.ambient.z;
		}
		// 先頭文字列がKdならアンビエント色
		if (key == "Kd")
		{
			line_stream >> material.diffuse.x;
			line_stream >> material.diffuse.y;
			line_stream >> material.diffuse.z;
		}
		// 先頭文字列がKsならアンビエント色
		if (key == "Ks")
		{
			line_stream >> material.specular.x;
			line_stream >> material.specular.y;
			line_stream >> material.specular.z;
		}
		// 先頭文字列がmap_kdならテクスチャファイル名
		if (key == "map_Kd")
		{
			// テクスチャファイル名読み込み
			line_stream >> material.textureFilename;
			// テクスチャ読み込み
			LoadTexture(directoryPath, material.textureFilename);
		}
	}
	// ファイルを閉じる
	file.close();
}

void ObjModel::LoadTexture(const std::string& directoryPath, const std::string& filename)
{
	// ファイルパスを結合
	std::string filepath = directoryPath + "/" + filename;

	textureData = TextureManager::GetInstance()->LoadTexture(filepath);
}

void ObjModel::Draw(ID3D12GraphicsCommandList* cmdList)
{
	// 頂点バッファビューの設定
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	// インデックスバッファの設定
	cmdList->IASetIndexBuffer(&ibView);

	// 定数バッファビューをセット（マテリアル）
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffB1->GetGPUVirtualAddress());

	// デスクリプタヒープの配列
	ID3D12DescriptorHeap* ppHeaps[] = { textureData.srvHeap.Get()};
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	if (material.textureFilename.size() > 0)
	{
		// シェーダリソースビューをセット
		cmdList->SetGraphicsRootDescriptorTable(1, textureData.srvGpuHandle);
	}
	// 描画コマンド
	cmdList->DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0);
}

void ObjModel::SetDevice(ID3D12Device* device)
{
	ObjModel::device = device;
}

void ObjModel::LoadFromOBJInternal(const std::string& modelname,bool smoothing)
{
	// ファイルストリーム
	std::ifstream file;

	const std::string filename = modelname + ".obj";	// "modelname.obj"
	const std::string directoryPath = "Resources/" + modelname + "/";	// "Resouces/modelname/"
	file.open(directoryPath + filename);	// "Resources/modelname/modelname.obj"

	// ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0);
	}

	std::vector<MyMath::Vector3> positions;		// 頂点座標
	std::vector<MyMath::Vector3> normals;		// 法線ベクトル
	std::vector<MyMath::Vector2> texcoords;		// テクスチャUV
	// 1行ずつ読み込む
	std::string line;
	while (getline(file, line))
	{
		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		// 半角スペース区切りで行の先頭文字列を取得
		std::string key;
		getline(line_stream, key, ' ');

		if (key == "v")
		{
			//X,Y,Z座標読み込み
			MyMath::Vector3 position;
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			// 座標データに追加
			positions.emplace_back(position);
		}
		// 先頭文字列がfならポリゴン（三角形）
		if (key == "f")
		{
			// 半角スペース区切りで行の続きを読み込む
			std::string index_string;
			while (getline(line_stream, index_string, ' '))
			{
				// 頂点インデックス1個分の文字列をストリームに変換して解析しやすくする
				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				index_stream >> indexPosition;
				index_stream.seekg(1, std::ios_base::cur);// スラッシュを飛ばす
				index_stream >> indexTexcoord;
				index_stream.seekg(1, std::ios_base::cur);// スラッシュを飛ばす
				index_stream >> indexNormal;
				// 頂点データの追加
				VertexPosNormalUv vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				vertices.emplace_back(vertex);
				//エッジ平滑化用のデータを追加
				if (smoothing)
				{
					AddSmoothData(indexPosition, (unsigned short)GetVertexCount() - 1);
				}
				// インデックスデータの追加
				indices.emplace_back((unsigned short)indices.size());
			}
		}
		// 先頭文字列がvtならテクスチャ
		if (key == "vt")
		{
			//U,V成分読み込み
			MyMath::Vector2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			// V方向反転
			texcoord.y = 1.0f - texcoord.y;
			// テクスチャ座標データに追加
			texcoords.emplace_back(texcoord);
		}
		// 先頭文字列がvnなら法線ベクトル
		if (key == "vn")
		{
			//X,Y,Z成分読み込み
			MyMath::Vector3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			// 法線ベクトルデータに追加
			normals.emplace_back(normal);
		}
		// 先頭文字列がmtllibならマテリアル
		if (key == "mtllib")
		{
			// マテリアルのファイル名読み込み
			std::string filename;
			line_stream >> filename;
			// マテリアル読み込み
			LoadMaterial(directoryPath, filename);
		}
	}
	// ファイルを閉じる
	file.close();

	//頂点法線の平均によるエッジの平滑化
	if (smoothing)
	{
		CalculateSmoothedVertexNormals();
	}

	CreateVBSize();
	CreateIBSize();
}

void ObjModel::InitializeDescriptorHeap()
{
	HRESULT result;

	// デスクリプタヒープを生成	
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	descHeapDesc.NumDescriptors = 1; // シェーダーリソースビュー1つ
	result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap));//生成
	assert(SUCCEEDED(result));

	descriptorHandleIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void ObjModel::CreateBuffers()
{
	HRESULT result;

	//定数バッファの生成
	D3D12_HEAP_PROPERTIES heapProp{};//ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;//GPUへの転送用

	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = (sizeof(ConstBufferDataB1) + 0xff) & ~0xff;//頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	result = device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffB1));
	assert(SUCCEEDED(result));

	// 定数バッファへデータ転送
	ConstBufferDataB1* constMap1 = nullptr;
	result = constBuffB1->Map(0, nullptr, (void**)&constMap1);
	if (SUCCEEDED(result))
	{
		constMap1->ambient = material.ambient;
		constMap1->diffuse = material.diffuse;
		constMap1->specular = material.specular;
		constMap1->alpha = material.alpha;
		constBuffB1->Unmap(0, nullptr);
	}
}

void ObjModel::CreateVBSize()
{
	HRESULT result;
	//頂点データ全体のサイズ
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * vertices.size());
	
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // 頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 頂点バッファの設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用

	// 頂点バッファの生成
	result = device->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	VertexPosNormalUv* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	for (int i = 0; i < vertices.size(); i++) {
		vertMap[i] = vertices[i]; // 座標をコピー
	}
	// 繋がりを解除
	vertBuff->Unmap(0, nullptr);

	// 頂点バッファビューの作成
	// GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	vbView.SizeInBytes = sizeVB;
	// 頂点1つ分のデータサイズ
	vbView.StrideInBytes = sizeof(vertices[0]);
}

void ObjModel::CreateIBSize()
{
	HRESULT result;
	//インデックスデータ全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(indices[0]) * indices.size());

	//リソース設定
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB;//インデックス情報が入る分のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//インデックスバッファの生成
	result = device->CreateCommittedResource(
		&heapProp,//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc,//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));

	//インデックスバッファをマッピング
	uint16_t* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	//全インデックスに対して
	for (int i = 0; i < indices.size(); i++) {
		indexMap[i] = indices[i];//インデックスをコピー
	}
	//マッピング解除
	indexBuff->Unmap(0, nullptr);

	//インデックスバッファビューの作成
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;
}

const std::vector<VertexPosNormalUv> ObjModel::GetVertices()
{
	return vertices;
}

#pragma region スムージング
inline size_t ObjModel::GetVertexCount()
{
	return vertices.size();
}

void ObjModel::AddSmoothData(unsigned short indexPosition, unsigned short indexVertex)
{
	smoothData[indexPosition].emplace_back(indexVertex);
}

void ObjModel::CalculateSmoothedVertexNormals()
{
	auto itr = smoothData.begin();
	for (; itr != smoothData.end(); ++itr)
	{
		//各面用の共通頂点コレクション
		std::vector<unsigned short>& v = itr->second;
		//全長店の法線を平均する
		MyMath::Vector4 normal = {};
		for (unsigned short index : v)
		{
			normal.x += vertices[index].normal.x;
			normal.y += vertices[index].normal.y;
			normal.z += vertices[index].normal.z;
			normal.w += 0.0f;
		}
		normal = normal / (float)v.size();
		normal = normal.normalization();
		//共通法線を使用する全ての頂点データに書き込む
		for (unsigned short index : v)
		{
			vertices[index].normal.x = normal.x;
			vertices[index].normal.y = normal.y;
			vertices[index].normal.z = normal.z;
		}
	}
}
#pragma endregion
