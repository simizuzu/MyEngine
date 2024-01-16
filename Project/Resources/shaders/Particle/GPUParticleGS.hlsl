#include "GPUParticle.hlsli"

[maxvertexcount(4)]
void main(point VSOutput input[1], inout TriangleStream< GSOutput > outStream)
{
	GSOutput output;
	
	[unroll]
	for (int i = 0; i < 4; i++)
	{
		//中心からのオフセットをスケーリング
		float4 offset = offset_array[i] * input[0].size;
		//中心からのオフセットをビルボード回転
		offset = mul(matBillboard, offset);
		output.svpos = float4(input[0].pos, 1.0f) + offset;
		output.svpos = mul(matWorld, output.svpos);
		output.color = input[0].color;
		output.uv = uv_array[i];
		
		outStream.Append(output);
	}

}