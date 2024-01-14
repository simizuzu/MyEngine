AppendStructuredBuffer<uint> freeList : register(u0);

cbuffer EmitData : register(b0)
{
	//座標
	float3 pos;
	//寿命
	float lifeTime;
	//加速度
	float3 accel;
	//スケール
	float scale;
	//開始時の色
	float4 startColor;
	//終了時の色
	float4 endColor;
	//経過時間
	float deltaTime;
	//パーティクルの最大数
	uint MAX_PARTICLE;
}

[numthreads(1024, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	if (DTid.x >= MAX_PARTICLE)
	{
		return;
	}

	freeList.Append(DTid.x);
}