#pragma once
#include "DirectX12Math.h"

/// <summary>
/// 球
/// </summary>
struct Sphere
{
	//中心座標
	MyMath::Vector3 center = {0,0,1};
	//半径
	float radius = 1.0f;
};

/// <summary>
/// レイ（半直線）
/// </summary>
struct Ray
{
	//視点座標
	MyMath::Vector3 start = { 0,0,1 };
	//方向
	MyMath::Vector3 dir = { 1,0,0 };
};