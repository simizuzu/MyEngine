#pragma once
#include "SuppressWarning.h"
MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <memory>
MYENGINE_SUPPRESS_WARNINGS_END

#include "ObjModel.h"
#include "Pipeline.h"
#include "Shader.h"
#include "Light.h"
#include "WorldTransform.h"

#include "Matrix4.h"

/**
 * @class ObjObject3d
 * @brief OBJモデルを描画するクラス
 */

class ObjObject3d
{
public:
	ObjObject3d() = default;

public: // 静的メンバ関数
	static void StaticInitialize(ID3D12Device* device);

	static void CreateGraphicsPipeline();

	static ObjObject3d* Create();

	static void SetLight(Light*light);

private:
	// デバイス
	static ID3D12Device* device_;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList_;
	// パイプラインステート
	static RootsigSetPip pip;
	//ライト
	static Light* light_;

public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	bool Initialize();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(WorldTransform* transform);

	// setter
	void SetModel(ObjModel* model);

	void SetModel(const std::string& filePath, bool smoothing = false);

	static void Finalize();

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
	int8_t pad1[ 4 ];
	// 親オブジェクト
	ObjObject3d* parent = nullptr;
	// モデル
	ObjModel* model_ = nullptr;

	//代入演算子削除
	ObjObject3d& operator=(const ObjObject3d&) = delete;

	//コピーコンストラクタ削除
	ObjObject3d(const ObjObject3d&) = delete;
};

namespace MyMath
{
	Matrix4 MakeWorldMatrix4(ObjObject3d& transform);
}
