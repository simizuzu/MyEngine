#pragma once
#include "DirectX12Math.h"
#include "WorldTransform.h"
#include "Camera.h"

/// <summary>
/// 3Dレティクルクラス
/// </summary>
class Reticle3D
{
public:
	
	void Initialize();

	void Update(Camera* camera,WorldTransform& transform);


private:

private:
	MyMath::Vector3 positionReticle;
};

