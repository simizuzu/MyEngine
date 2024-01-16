cbuffer cbuff0 : register(b0)
{
	matrix matWorld;
	matrix billMatWorld;
}

struct ParticleData
{
	//色
	float4 color;
	//座標
	float3 position;
	//経過時間
	float AGE;
	//速度
	float3 velocity;
	//生存しているか
	float isAlive;
	//加速度
	float3 accel;
	//サイズ
	float size;
	//ライフタイム
	float lifeTime;

	//パディング
	float3 pad;
};

struct VSOutput
{
	float4 pos : POSITION;
	float size : SIZE;
	float4 color : COLOR;
};

struct GSOutput
{
	float4 svpos : SV_POSITION;
	float2 uv : TEXCOORD;
	float4 color : COLOR;
};