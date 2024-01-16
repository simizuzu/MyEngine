cbuffer cbuff0 : register(b0)
{
	matrix matWorld;
	matrix matBillboard;
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
	float3 pos : POSITION;
	float size : SIZE;
	float4 color : COLOR;
};

struct GSOutput
{
	float4 svpos : SV_POSITION;
	float2 uv : TEXCOORD;
	float4 color : COLOR;
};

// 四角形の頂点数
static const uint vnum = 4;

// センターからのオフセット
static const float4 offset_array[vnum] =
{
	float4(-0.5f, -0.5f, 0, 0), //左下
	float4(-0.5f, +0.5f, 0, 0), //左上
	float4(+0.5f, -0.5f, 0, 0), //右下
	float4(+0.5f, +0.5f, 0, 0) //右上
};

static const float2 uv_array[vnum] =
{
	float2(0, 1), //左下
	float2(0, 0), //左上
	float2(1, 1), //右下
	float2(1, 0) //右上
};