#include "FbxObject3d.h"

#include "DirectXCommon.h"
#include "Shader.h"

 /**
 * @file FbxObject3d.cpp
 * @brief FbxObject3dの処理について書いてあります
 */

//静的メンバ変数の実態
Microsoft::WRL::ComPtr<ID3D12Device> FbxObject3d::device_;
Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> FbxObject3d::cmdList_;
RootsigSetPip FbxObject3d::pip;

void FbxObject3d::StaticInitialize(ID3D12Device* device)
{
	assert(device);

	FbxObject3d::device_ = device;

	//グラフィックスパイプラインの生成
	CrateGrapicsPipeline();
}


FbxObject3d* FbxObject3d::Create()
{
	// 3Dオブジェクトのインスタンスを生成
	FbxObject3d* object3d = new FbxObject3d();
	if (object3d == nullptr)
	{
		return nullptr;
	}

	// 初期化
	if (!object3d->Initialize())
	{
		delete object3d;
		assert(0);
		return nullptr;
	}

	return object3d;
}

bool FbxObject3d::Initialize()
{
	HRESULT result;

	CD3DX12_HEAP_PROPERTIES HEAP_PROP = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC RESOURCE_DESC = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataTransformFbx) + 0xff) & ~0xff);

	result = device_->CreateCommittedResource(
		&HEAP_PROP,
		D3D12_HEAP_FLAG_NONE,
		&RESOURCE_DESC,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform)
	);

	CD3DX12_HEAP_PROPERTIES HEAP_PROP_SKIN = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC RESOURCE_DESC_SKIN = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataSkin) + 0xff) & ~0xff);

	result = device_->CreateCommittedResource(
		&HEAP_PROP_SKIN,
		D3D12_HEAP_FLAG_NONE,
		&RESOURCE_DESC_SKIN,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffSkin)
	);

	//1フレーム分の時間を60FPSで設定
	frameTime.SetTime(0,0,0,1,0,FbxTime::EMode::eFrames60);

	return true;
}

void FbxObject3d::Update(Camera* camera)
{
	HRESULT result;
	MyMath::Matrix4 matScale, matRot, matTrans;

	// スケール、回転、平行移動行列の計算
	matScale = MyMathUtility::MakeScaling(scale_);
	matRot = MyMathUtility::MakeIdentity();
	matRot = MyMathUtility::MakeRotation(rotation_);
	matTrans = MyMathUtility::MakeTranslation(translation_);

	// ワールド行列の合成
	matWorld = MyMathUtility::MakeIdentity();
	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrans;

	const MyMath::Matrix4 matView = camera->GetMatView();
	const MyMath::Matrix4 matProjection = camera->GetMatProjection();
	const MyMath::Vector3& cameraPos = camera->GetEye();
	const MyMath::Matrix4& modelTransform = model_->GetModelTransform();

	// 定数バッファへデータ転送
	ConstBufferDataTransformFbx* constMap = nullptr;
	result = constBuffTransform->Map(0, nullptr, (void**)&constMap);
	assert(SUCCEEDED(result));
	constMap->viewproj = matView * matProjection;
	constMap->world = modelTransform * matWorld;
	constMap->cameraPos = cameraPos;
	constBuffTransform->Unmap(0, nullptr);

	//アニメーション
	if (isPlay)
	{
		//1フレーム、宇進める
		currentTime += frameTime;
		//最後まで再生したら先頭に戻す
		if (currentTime > endTime)
		{
			currentTime = startTime;
		}
	}

	//ボーン配列
	std::vector<FbxModel::Bone>& bones = model_->GetBones();

	//定数バッファへデータ転送
	ConstBufferDataSkin* constMapSkin = nullptr;
	result = constBuffSkin->Map(0, nullptr, (void**)&constMapSkin);
	for (int i = 0; i > MAX_BONES; i++)
	{
		constMapSkin->bones[i] = MyMathUtility::MakeIdentity();
	}

	for (size_t i = 0; i < bones.size(); i++)
	{
		//今の姿勢行列
		MyMath::Matrix4 matCurrentPose;
		//指定時間(currentTime)の姿勢行列を取得
		FbxAMatrix fbxCurrengPose = 
		bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime);
		//Matrix4に変換
		FbxLoader::ConvertMatrixFromFbx(&matCurrentPose, fbxCurrengPose);
		//合成してスキニング行列に
		constMapSkin->bones[i] = bones[i].invInitialPose * matCurrentPose;
	}
	constBuffSkin->Unmap(0, nullptr);
}

void FbxObject3d::Draw()
{
	cmdList_ = DirectXCommon::GetInstance()->GetCommandList();

	//モデルの割り当てがなければ描画しない
	if (model_ == nullptr)
	{
		return;
	}

	//パイプラインステートの設定
	cmdList_->SetPipelineState(pip.pipelineState.Get());
	//ルートシグネチャの設定
	cmdList_->SetGraphicsRootSignature(pip.rootSignature.Get());
	//プリミティブ形状を設定
	cmdList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//定数バッファビューをセット
	cmdList_->SetGraphicsRootConstantBufferView(0, constBuffTransform->GetGPUVirtualAddress());
	//定数バッファビューをセット
	cmdList_->SetGraphicsRootConstantBufferView(2, constBuffSkin->GetGPUVirtualAddress());

	//モデル描画
	model_->Draw(cmdList_.Get());
}

void FbxObject3d::SetModel(FbxModel* model)
{
	model_ = model;
}

void FbxObject3d::PlayAnimation()
{
	FbxScene* fbxScene = model_->GetFbxScene();
	//0番のアニメーション取得
	FbxAnimStack* animstack = fbxScene->GetSrcObject<FbxAnimStack>(0);
	//アニメーションの名前取得
	const char* animstackname = animstack->GetName();
	//アニメーションの時間情報
	FbxTakeInfo* takeinfo = fbxScene->GetTakeInfo(animstackname);

	//開始時間取得
	startTime = takeinfo->mLocalTimeSpan.GetStart();
	//終了時間取得
	endTime = takeinfo->mLocalTimeSpan.GetStop();
	//開始時間に合わせる
	currentTime = startTime;
	//再生中状態にする
	isPlay = true;
}

void FbxObject3d::SetScale(MyMath::Vector3 scale)
{
	scale_ = scale;
}

void FbxObject3d::SetRotation(MyMath::Vector3 rotation)
{
	rotation_ = rotation;
}

void FbxObject3d::SetTranslation(const MyMath::Vector3 translation)
{
	translation_ = translation;
}

MyMath::Vector3 FbxObject3d::GetScale()
{
	return scale_;
}

MyMath::Vector3 FbxObject3d::GetRotation()
{
	return rotation_;
}

MyMath::Vector3 FbxObject3d::GetTranslation()
{
	return translation_;
}

void FbxObject3d::CrateGrapicsPipeline()
{
	Microsoft::WRL::ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> psBlob;	// ピクセルシェーダオブジェクト

	Shader::CreateFBXShade(vsBlob,psBlob);

	Pipeline::CreateFBXPipeline(vsBlob.Get(),psBlob.Get(),device_.Get(), pip);
}