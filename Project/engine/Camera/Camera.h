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
/// ビュープロジェクションバッファデータ
/// </summary>
struct ConstBufferDataViewProjection
{
	// ワールド → ビュー変換行列
	MyMath::Matrix4 view;
	// ビュー → プロジェクション変換行列
	MyMath::Matrix4 projection;
	// カメラ座標（ワールド座標）
	MyMath::Vector3 cameraPos;
};

/// <summary>
/// カメラ
/// </summary>
class Camera
{
private:
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;
	// マッピング済みアドレス
	ConstBufferDataViewProjection* constBuffMap = nullptr;

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
	//回転
	MyMath::Vector3 rotation_;
	MyMath::Quaternion quaternion_;
	//平行移動
	MyMath::Vector3 translation_;

	//カメラのワールド行列（スケールは無し）
	MyMath::Matrix4 matTrans,matRot;

public:
#pragma region ビュー行列設定
	// 視点座標
	MyMath::Vector3 eye_;
	// 注視点座標
	MyMath::Vector3 target_;
	// 上方向ベクトル
	MyMath::Vector3 up_;
	//カメラのワールド
	MyMath::Matrix4 matCameraWorld_;
#pragma endregion
	const Camera* parent = nullptr;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="mode">[default, matrix, quaternion]</param>
	void Update(const std::string& mode);

	/// <summary>
	/// マットワールド用注視点
	/// </summary>
	/// <param name="target">注視点座標</param>
	/// <param name="up">上ベクトル</param>
	void LookAtMatrix(const MyMath::Vector3& target, const MyMath::Vector3& up);

private:
	/// <summary>
	/// 定数バッファ生成
	/// </summary>
	void CreateConstBuffer();
	/// <summary>
	/// ビュー行列の更新(LoodAt方式)
	/// </summary>
	void UpdateLookAt();
	/// <summary>
	/// ビュー行列の更新(ワールド座標方式)
	/// </summary>
	void UpdateMatrix();
	/// <summary>
	/// ビュー行列の更新(クォータニオン回転方式)
	/// </summary>
	void UpdateQuaternion();
	/// <summary>
	/// ビュー行列の転送
	/// </summary>
	void TransferMatrix();

public:
	//getter・setter
	const MyMath::Matrix4& GetMatView();
	const MyMath::Matrix4& GetMatViewInverse();
	const MyMath::Matrix4& GetMatProjection();

	const MyMath::Vector3& GetEye();
	const MyMath::Vector3& GetTarget();
	const MyMath::Vector3& GetUp();

	const MyMath::Vector3& GetTranslation();
	const MyMath::Vector3& GetRotation();

	const MyMath::Matrix4& GetMatWorld();

	MyMath::Matrix4& GetMatRot();

	void SetTranslation(MyMath::Vector3 translation);
	void SetRotation(MyMath::Vector3 rotation);
	void SetRotation(MyMath::Quaternion rotation);

	//視点距離
	void SetEye(const MyMath::Vector3& eye);
	//視点方向
	void SetTarget(const MyMath::Vector3& target);
	void SetUp(const MyMath::Vector3& up);
	void SetFarZ(const float& farZ);
	void SetNearZ(const float& nearZ);
	void SetDistance(const float& distance);
	void SetFovAngleY(const float& fovAngle);

	/// <summary>
	/// HAngleの取得関数(水平方向)
	/// </summary>
	/// <param name="eye">始点</param>
	/// <param name="target">注視点</param>
	/// <returns>角度</returns>
	float GetHAngle(const MyMath::Vector3& eye,const MyMath::Vector3& target);

	/// <summary>
	/// VAngleの取得関数(垂直方向)
	/// </summary>
	/// <param name="eye">始点</param>
	/// <param name="target">注視点</param>
	/// <returns></returns>
	float GetVAngle(const MyMath::Vector3& eye,const MyMath::Vector3& target);

	void SetCameraRot(MyMath::Vector3& rotation);

	Camera() = default;
	~Camera() = default;

private:
	//コピーコンストラクタ削除
	Camera& operator=(const Camera&) = delete;
	//代入演算子削除
	Camera(const Camera&) = delete;
};
