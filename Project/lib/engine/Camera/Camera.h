#pragma once
#include "DirectX12Math.h"
#include "EngineUtility.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <DirectXMath.h>
MYENGINE_SUPPRESS_WARNINGS_END

/**
 * @class Camera.h
 * @brief カメラを作成する(ビュープロジェクション)クラス
 */

/// <summary>
/// ビュープロジェクション頂点データ
/// </summary>
struct ConstBufferDataViewProjection
{
	// ワールド行列
	MyMath::Matrix4 world;
	// ワールド座標
	MyMath::Matrix4 matWorld;

	// ワールド → ビュー変換行列
	MyMath::Matrix4 view;
	// ビュー → プロジェクション変換行列
	MyMath::Matrix4 projection;
	// カメラ座標（ワールド座標）
	MyMath::Vector3 cameraPos;

	ConstBufferDataViewProjection() = default;

	//代入演算子削除
	ConstBufferDataViewProjection& operator=(const ConstBufferDataViewProjection&) = delete;
	//コピーコンストラクタ削除
	ConstBufferDataViewProjection(const ConstBufferDataViewProjection&) = delete;
};

/// <summary>
/// ビュープロジェクション
/// </summary>
struct ViewProjection
{
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;
	// マッピング済みアドレス
	ConstBufferDataViewProjection* constBuffMap = nullptr;

	ViewProjection() = default;

//代入演算子削除
	ViewProjection& operator=(const ViewProjection&) = delete;
	//コピーコンストラクタ削除
	ViewProjection(const ViewProjection&) = delete;
};

/// <summary>
/// カメラ
/// </summary>
class Camera
{
public:
#pragma region ビュー行列設定
	// 視点座標
	MyMath::Vector3 eye_;
	// 注視点座標
	MyMath::Vector3 target_;
	// 上方向ベクトル
	MyMath::Vector3 up_;
#pragma endregion

	// ビュー行列
	MyMath::Matrix4 matView_;
	// ビュー逆行列
	MyMath::Matrix4 matViewInverse_;
	// 射影行列
	MyMath::Matrix4 matProjection_;;
	// カメラ距離
	float distance_ = 50.0f;

private:
#pragma region 射影行列設定
	// カメラ画角
	float fovAngleY = MyMathUtility::degree2Radius * 45.0f;
	// 手前
	float nearZ_ = 0.1f;
	// 奥行き
	float farZ_ = 1000.0f;
	// アスペクト比(画面横幅/画面縦幅)
	float aspect = 0.0f;
#pragma endregion

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 定数バッファ生成
	/// </summary>
	void CreateConstBuffer();
	/// <summary>
	/// マッピング
	/// </summary>
	void Map();
	/// <summary>
	/// ビュー行列の更新
	/// </summary>
	void UpdateMatrix();
	/// <summary>
	/// ビュー行列の転送
	/// </summary>
	void TransferMatrix();

public:
	const MyMath::Matrix4& GetMatView();
	const MyMath::Matrix4& GetMatViewInverse();
	const MyMath::Matrix4& GetMatProjection();

	const MyMath::Vector3& GetEye();
	const MyMath::Vector3& GetTarget();
	const MyMath::Vector3& GetUp();

	//視点距離
	void SetEye(const MyMath::Vector3& eye);
	//視点方向
	void SetTarget(const MyMath::Vector3& target);
	void SetUp(const MyMath::Vector3& up);
	void SetFarZ(const float& farZ);
	void SetNearZ(const float& nearZ);
	void SetDistance(const float& distance);
	void SetFovAngleY(const float& fovAngle);

	//コピーコンストラクタ削除
	Camera& operator=(const Camera&) = delete;
	//代入演算子削除
	Camera(const Camera&) = delete;
	Camera() = default;
	~Camera() = default;
};
