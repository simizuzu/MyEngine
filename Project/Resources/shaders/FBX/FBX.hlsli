//3D変換行列
cbuffer cbuff0 : register(b0)
{
    matrix viewproj;
    matrix world;
    float3 camerapos;
};

//ボーンの最大数
static const int MAX_BONES = 64;

//スキニング
cbuffer skinning : register(b3)
{
	matrix matSkinning[MAX_BONES];
};

struct VSInput
{
    float4 pos  : POSITION; //位置
    float3 normal : NORMAL; //頂点法線
    float2 uv   : TEXCOORD; //テクスチャー座標
	uint4 boneIndices : BONEINDICES; //ボーンの番号
	float4 boneWeights : BONEWEIGHTS; //ボーンのスキンウェイト
};

struct VSOutput
{
    float4 svpos : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};
