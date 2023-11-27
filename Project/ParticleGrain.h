#pragma once

#include "Vector3.h"
#include <stdint.h>

/**
 * @file BaseGrain.h
 * @brief パーティクルの一粒
 */

struct ParticleGrain
{
	//座標
	MyMath::Vector3 pos = {};
	//回転
	MyMath::Vector3 rot = {};
	//速度
	MyMath::Vector3 velocity = {};
	//加速度
	MyMath::Vector3 accel = {};
	//現在フレーム
	uint16_t startFrame = 0;
	//終了フレーム
	uint16_t endFrame = 0;
	//スケール
	float scale = 0;
	//初期値
	float startScale = 1.0f;
	//最終値
	float endScale = 0.0f;
};