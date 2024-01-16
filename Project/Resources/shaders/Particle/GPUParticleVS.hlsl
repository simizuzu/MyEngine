#include "GPUParticle.hlsli"

RWStructuredBuffer<ParticleData> ParticlePool : register(u0);
RWStructuredBuffer<uint> DrawList : register(u1);

VSOutput main(uint id : SV_InstanceID)
{
	VSOutput output; // ピクセルシェーダーに渡す値
	
	uint drawIndex = DrawList[id];
	ParticleData particle = ParticlePool[id];
	
	output.pos = particle.position;
	output.size = particle.size;
	output.color = particle.color;
	
	if(particle.isAlive <= 0)
	{
		output.size = 0.0f;
	}
	
	return output;
}