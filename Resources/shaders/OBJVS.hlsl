#include "Obj.hlsli"

VSOutput main(float4 pos : POSITION, float2 uv : TEXCOORD, float3 normal : NORMAL)
{
	float4 wnormal = normalize(mul(world, float4(normal, 0)));
	float4 wpos = mul(world, pos);

	float3 ambient = m_ambient;

	float3 diffuse = dot(lightVec,wnormal.xyz) * m_diffuse;


	const float shininess = 4.0f;

	float3 eyedir = normalize(cameraPos - wpos.xyz);

	float3 reflect = normalize(-lightVec + 2 * dot(lightVec, wnormal.xyz) * wnormal.xyz);

	float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

	VSOutput output;
	output.svpos = mul(mul(viewproj,world),pos);

	output.color.rgb = dot(-lightVec, normal) * m_diffuse * lightColor;

	output.color.rgb = (ambient + diffuse + specular) * lightColor;
	output.color.a = m_alpha;
	output.uv = uv;

	return output;
}