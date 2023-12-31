#include "Shader.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <cassert>
#include <string>
#include <d3dcompiler.h>
MYENGINE_SUPPRESS_WARNINGS_END

#pragma comment(lib,"d3dcompiler.lib")

 /**
 * @file Shader.cpp
 * @brief Shaderの処理について書いてあります
 */

void Shader::CreateSpriteShade(Microsoft::WRL::ComPtr<ID3DBlob>& vsBlob, Microsoft::WRL::ComPtr<ID3DBlob>& psBlob)
{
	// エラーオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;

	//　頂点シェーダの読み込みとコンパイル
	HRESULT result = D3DCompileFromFile(
		L"Resources/shaders/Sprite/SpriteVS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,	// インクルード可能にする
		"main", "vs_5_0",	// エントリーポイント名、シェーダモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバック用設定
		0, &vsBlob, &errorBlob
	);
	
	// シェーダのエラー内容を表示
	if (FAILED(result))
	{
		// errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/Sprite/SpritePS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,	// インクルード可能にする
		"main", "ps_5_0",	// エンドリーポイント名、シェーダモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバック用設定
		0, &psBlob, &errorBlob
	);

	// エラーなら
	if (FAILED(result))
	{
		// errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}
}

void Shader::CreateObjShade(Microsoft::WRL::ComPtr<ID3DBlob>& vsBlob, Microsoft::WRL::ComPtr<ID3DBlob>& psBlob)
{
	// エラーオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;

	//　頂点シェーダの読み込みとコンパイル
	HRESULT result = D3DCompileFromFile(
		L"Resources/shaders/OBJ/OBJVS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,	// インクルード可能にする
		"main", "vs_5_0",	// エントリーポイント名、シェーダモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバック用設定
		0, &vsBlob, &errorBlob
	);

	// シェーダのエラー内容を表示
	if (FAILED(result))
	{
		// errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/OBJ/OBJPS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,	// インクルード可能にする
		"main", "ps_5_0",	// エンドリーポイント名、シェーダモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバック用設定
		0, &psBlob, &errorBlob
	);

	// エラーなら
	if (FAILED(result))
	{
		// errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}
}

void Shader::CreateFBXShade(Microsoft::WRL::ComPtr<ID3DBlob>& vsBlob, Microsoft::WRL::ComPtr<ID3DBlob>& psBlob)
{
	// エラーオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;
	HRESULT result = S_FALSE;

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/FBX/FBXVS.hlsl",    // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0",    // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/FBX/FBXPS.hlsl",    // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0",    // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}
}

void Shader::CreateParticleShade(Microsoft::WRL::ComPtr<ID3DBlob>& vsBlob, Microsoft::WRL::ComPtr<ID3DBlob>& psBlob, Microsoft::WRL::ComPtr<ID3DBlob>& gsBlob)
{
	// エラーオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;

	//　頂点シェーダの読み込みとコンパイル
	HRESULT result = D3DCompileFromFile(
		L"Resources/shaders/Particle/ParticleVS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,	// インクルード可能にする
		"main", "vs_5_0",	// エントリーポイント名、シェーダモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバック用設定
		0, &vsBlob, &errorBlob
	);

	// シェーダのエラー内容を表示
	if (FAILED(result))
	{
		// errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}

	// ジオメトリシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/Particle/ParticleGS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "gs_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&gsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/Particle/ParticlePS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,	// インクルード可能にする
		"main", "ps_5_0",	// エンドリーポイント名、シェーダモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバック用設定
		0, &psBlob, &errorBlob
	);

	// エラーなら
	if (FAILED(result))
	{
		// errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}
}

void Shader::CreatePostEffectShade(Microsoft::WRL::ComPtr<ID3DBlob>& vsBlob, Microsoft::WRL::ComPtr<ID3DBlob>& psBlob)
{
	HRESULT result;
	// エラーオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;

	//頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/PostEffect/PostEffectVS.hlsl",	//シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,	//インクルード可能にする
		"main", "vs_5_0",	//エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,	//デバック用設定
		0,
		&vsBlob, &errorBlob);

	if (FAILED(result)) {
		//errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";

		//エラー内容を出力ウインドウに表示
		OutputDebugStringA(errstr.c_str());
		assert(0);
	}

	//ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/PostEffect/PostEffectPS.hlsl",	//シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,	//インクルード可能にする
		"main", "ps_5_0",	//エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,	//デバッグ用設定
		0,
		&psBlob, &errorBlob);

	if (FAILED(result)) {
		//errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";

		//エラー内容を出力ウインドウに表示
		OutputDebugStringA(errstr.c_str());
		assert(0);
	}
}
