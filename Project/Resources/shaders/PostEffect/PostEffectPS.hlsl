#include "PostEffect.hlsli"

Texture2D<float4> tex0 : register(t0);  //0番スロットに設定されたテクスチャ
Texture2D<float4> tex1 : register(t1);  //1番スロットに設定されたテクスチャ
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    float blurLevel = 4.0f;
    float vForRange = 5.0f;
    float uForRange = 5.0f;

    //const float offsetU = blurLevel / 1280;
    //const float offsetV = blurLevel / 720;
    //float4 color = { 0, 0, 0, 0 };
    
    //for (float v = 0; v < vForRange; v++)
    //{
    //    for (float u = 0; u < uForRange; u++)
    //    {
    //        float2 offSet = float2((u - 1) * offsetU, (v - 1) * offsetV);
    //        color += tex0.Sample(smp, saturate(input.uv - offSet));
    //    }
    //}

    //float4 texcolor = color / (uForRange * vForRange);
    //return float4(1.0f - texcolor.rgb, 1);

    float4 colortex0 = tex0.Sample(smp, input.uv);
    float4 colortex1 = tex1.Sample(smp, input.uv);

    float4 color;
    if (fmod(input.uv.y, 0.1f) < 0.05f)
    {
        color.x = colortex0;
        color.y = colortex1;
    }

    return float4(color.rgb, 1);
}