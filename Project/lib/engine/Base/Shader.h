#pragma once
#include <DirectXTex/d3dx12.h>
#include <wrl.h>

class Shader
{
public:

	static void CreateSpriteShade(Microsoft::WRL::ComPtr<ID3DBlob>& vsBlob, Microsoft::WRL::ComPtr<ID3DBlob>& psBlob);

	static void CreateObjShade(Microsoft::WRL::ComPtr<ID3DBlob>& vsBlob, Microsoft::WRL::ComPtr<ID3DBlob>& psBlob);

	static void CreateFBXShade(Microsoft::WRL::ComPtr<ID3DBlob>& vsBlob, Microsoft::WRL::ComPtr<ID3DBlob>& psBlob);

	static void CreateParticleShade(Microsoft::WRL::ComPtr<ID3DBlob>& vsBlob, Microsoft::WRL::ComPtr<ID3DBlob>& psBlob, Microsoft::WRL::ComPtr<ID3DBlob>& gsBlob);

	static void CreatePostEffectShade(Microsoft::WRL::ComPtr<ID3DBlob>& vsBlob,Microsoft::WRL::ComPtr<ID3DBlob>& psBlob);
};