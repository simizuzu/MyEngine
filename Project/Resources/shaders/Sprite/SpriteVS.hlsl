#include "Sprite.hlsli"

VSOutput main(float4 pos : POSITION, float2 uv : TEXCOORD, float4 col : COLOR)
{
    VSOutput output; // ピクセルシェーダーに渡す値
    output.svpos = mul(mat, pos);
    output.uv = uv;
	output.color = col;
    return output;
}