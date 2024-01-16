#include "GPUParticle.hlsli"

RWStructuredBuffer<ParticleData> ParticlePool : register(u0);
ConsumeStructuredBuffer<uint> DrawList : register(u1);

VSOutput main(uint id : SV_POSITION)
{
	VSOutput output; // ピクセルシェーダーに渡す値
	output.pos = pos;
	return output;
}