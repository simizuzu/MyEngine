#pragma once
#include <memory>

#include "ObjModel.h"
#include "Pipeline.h"
#include "Shader.h"
#include "Light.h"
#include "WorldTransform.h"

#include "Matrix4.h"

class ObjObject3d
{
public:


public: // 静的メンバ関数
	static void StaticInitialize(ID3D12Device* device);

	static void CreateGraphicsPipeline();

	static ObjObject3d* Create();

	static void SetLight(Light*light);

private:
	// デバイス
	static Microsoft::WRL::ComPtr<ID3D12Device> device_;
	// コマンドリスト
	static Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList_;
	// パイプラインステート
	static RootsigSetPip pip;
	//ライト
	static Light* light;

public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	bool Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	//void Update(Camera* camera);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(WorldTransform* transform);

	// setter
	void SetModel(ObjModel* model);

public:
	// 定数バッファ
	Microsoft::WRL::ComPtr <ID3D12Resource> constBuffB0;
	// 色
	MyMath::Vector4 color = { 1.0f,1.0f,1.0f,1.0f };
	// ローカルスケール
	MyMath::Vector3 scale = { 1.0f,1.0f,1.0f };
	// X,Y,Z軸回りのローカル回転角
	MyMath::Vector3 rotation = { 0.0f,0.0f,0.0f };
	// ローカル座標
	MyMath::Vector3 position = { 0.0f,0.0f,0.0f };
	// ワールド変換行列
	MyMath::Matrix4 matWorld;
	// 親オブジェクト
	ObjObject3d* parent = nullptr;
	// モデル
	ObjModel* model = nullptr;
};

namespace MyMath
{
	Vector3 GetWorldPosition(ObjObject3d& transform);
	Matrix4 MakeWorldMatrix4(ObjObject3d& transform);
}
