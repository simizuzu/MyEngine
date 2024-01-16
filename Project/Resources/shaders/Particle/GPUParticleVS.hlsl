#include "GPUParticle.hlsli"

RWStructuredBuffer<ParticleData> ParticlePool : register(u0);
ConsumeStructuredBuffer<uint> DrawList : register(u1);

VSOutput main(uint id : SV_InstanceID)
{
	VSOutput output; // ピクセルシェーダーに渡す値
	
	uint drawIndex = DrawList.Consume();
	ParticleData particle = ParticlePool.Load(drawIndex);
	
	output.pos = particle.position;
	output.size = particle.size;
	output.color = particle.color;
	
	return output;
}