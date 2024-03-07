//マテリアル情報
cbuffer ConstBufferDataMaterial : register(b0)
{
    //float4 color; // 色(RGBA)
    float3 m_ambient : packoffset(c0); // アンビエント係数
    float3 m_diffuse : packoffset(c1); // ディフューズ係数
    float3 m_specular : packoffset(c2); // スペキュラー係数
    float m_alpha : packoffset(c2.w); // アルファ
};

// 3D変換行列
cbuffer ConstBufferDataTransform : register(b1)
{
    //matrix mat; // 3D変換行列
    matrix viewproj; //ビュープロジェクション行列
    matrix world; //ワールド行列
    float3 cameraPos; //カメラ座標（ワールド座標）
}

cbuffer cbuff2 : register(b2)
{
    float3 lightVec;
    float3 lightColor;
}

// 頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
    float4 svpos : SV_POSITION; // システム用頂点座標
    float3 normal : NORMAL; // 法線ベクトル
    //float4 color : COLOR;
    float2 uv : TEXCOORD; // uv値
	float4 worldpos : POSITION;
};
