#pragma once

#include "DirectX12Math.h"
#include "TextureManager.h"
#include "Shader.h"
#include "Pipeline.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <DirectXMath.h>
#include <memory>
MYENGINE_SUPPRESS_WARNINGS_END

// 頂点データ構造体
struct PosUvColor
{
	MyMath::Vector3 pos;
	MyMath::Vector2 uv;
	MyMath::Vector4 color;
};

class Sprite
{
public:
	// 頂点番号
	enum VertexNumber {
		LB,	// 左下
		LT,	// 左上
		RB,	// 右下
		RT,	// 右上
	};

public: // エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private: // 静的メンバ変数
	// デバイス
	static ComPtr<ID3D12Device> device_;
	// デスクリプタサイズ
	static UINT descriptorSize_;
	// コマンドリスト
	static ComPtr<ID3D12GraphicsCommandList> commandList_;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootSignature_;
	// プロジェクション行列
	static MyMath::Matrix4 matProjection_;
	// パイプラインステート
	static std::array<RootsigSetPip, 6> pipelineState;

private: // メンバ変数
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	//頂点マップ
	PosUvColor* vertMap;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//インデックスバッファの生成
	Microsoft::WRL::ComPtr<ID3D12Resource>indexBuff;
	//インデックスバッファをマッピング
	uint16_t* indexMap;
	//インデックスバッファビューの作成
	D3D12_INDEX_BUFFER_VIEW ibView{};
	// エラーオブジェクト
	ComPtr<ID3DBlob> errorBlob;
	// ルートシグネチャのシリアライズ
	ComPtr<ID3DBlob> rootSigBlob;
	// インプットレイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout{};

	// 座標
	MyMath::Vector2 position_ = { 0.0f,0.0f };
	// 色
	MyMath::Vector4 color_ = { 1, 1, 1, 1 };

	//定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource>constBuff;
	MyMath::Matrix4* constBuffMap = nullptr;

	// 角度
	float rotation_ = 0.0f;
	// 表示サイズ(単位はpixel)
	MyMath::Vector2 size_ = { 100.0f,100.0f };
	// テクスチャ番号
	uint32_t textureIndex_ = 0;
	// ブレンドモード
	uint8_t blendMode = (int)BlendMode::Alpha;
	int8_t pad1[ 7 ];

public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update(MyMath::Vector2 pos, MyMath::Vector2 scale, float rot);

	/// <summary>
	/// 描画
	/// </summary>
	void DrawCommandList(TextureData textureData);

	/// <summary>
	/// スプライト描画
	/// </summary>
	/// <param name="textureData">テクスチャデータ</param>
	/// <param name="pos">座標</param>
	/// <param name="rot">回転</param>
	/// <param name="scale">拡大率</param>
	void Draw(
		TextureData& textureData, MyMath::Vector2 position, MyMath::Vector2 scale = { 1.0f,1.0f }, float rotation = 0.0f,
		MyMath::Vector2 anchorpoint = { 0.0f,0.0f }, bool flipX = false, bool flipY = false);

	/*/// <summary>
	/// スプライトのクリップ描画
	/// </summary>
	/// <param name="textureData">テクスチャデータ</param>
	/// <param name="clipCenter">切り取るの中心座標</param>
	/// <param name="clipRadius">切り取る半径</param>
	/// <param name="position">座標</param>
	/// <param name="scale">大きさ</param>
	/// <param name="rotation">回転</param>
	/// <param name="flipX">X軸反転</param>
	/// <param name="flipY">Y軸反転</param>
	void DrawSpriteClip(
		TextureData& textureData, Mathematics::Vector2 clipCenter, Mathematics::Vector2 clipRadius,
		Mathematics::Vector2 position, Mathematics::Vector2 scale = { 1.0f,1.0f }, float rotation = 0.0f,
		bool flipX = false, bool flipY = false);*/

private:
	/// <summary>
	/// 頂点バッファ関連の初期化
	/// </summary>
	void CreateVertexIndexBuff();

	/// <summary>
	/// 定数バッファ作成
	/// </summary>
	void CreateConstBuff();

public: // 静的メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	static void StaticInitialize();

public: // setter,getter
	/// <summary>
	/// 座標をセット
	/// </summary>
	/// <param name="position">: 座標{ x,y }</param>
	void SetPosiotion(const MyMath::Vector2& position);

	/// <summary>
	/// 回転角をセット
	/// </summary>
	/// <param name="rotation">: 角度</param>
	void SetRotation(float rotation);

	/// <summary>
	/// スプライトの大きさをセット
	/// </summary>
	/// <param name="rotation">サイズ</param>
	void SetSize(const MyMath::Vector2& size);

	/// <summary>
	/// スプライトの色をセット
	/// </summary>
	/// <param name="color">: 色{ red,blue,green,alpha }</param>
	void SetColor(const MyMath::Vector4& color);

	/// <summary>
	/// テクスチャ番号をセット
	/// </summary>
	/// <param name="textureIndex">: テクスチャ番号</param>
	void SetTextureIndex(uint32_t textureIndex);

	/// <summary>
	/// ブレンドを設定
	/// </summary>
	void SetBlendMode(BlendMode mode);

	// 座標
	const MyMath::Vector2& GetPosition() const;
	// 角度
	float GetRotation() const;
	// 色
	const MyMath::Vector4& GetColor() const;
	// サイズ
	const MyMath::Vector2& GetSize() const;
	// テクスチャ番号
	uint32_t GetTextureIndex();

private: // クラス呼び出し
	TextureManager* textureManager_ = nullptr;
	DirectXCommon* dxCommon_ = nullptr;
	static WinApp* winApp_;
};
