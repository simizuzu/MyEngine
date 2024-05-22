#include "KeyframeObject.h"

void KeyframeObject::Initialize()
{
	keyframeData = LevelLoader::LoadKeyframe("anim",animationTime);
}

void KeyframeObject::Update(Camera* camera)
{
	animationTime += 1.0f / 60.0f;
	animationTime = std::fmod(animationTime,keyframeData->meshKeyframe[ "Box" ].duration);

	translate = MyMathUtility::CalculateValueSlerp(keyframeData->meshKeyframe[ "Box" ].scale,animationTime);

	if ( camera )
	{

	}
}

void KeyframeObject::IsPlayAnimation()
{
	////開始時間取得(1.0Fからスタート)
	//currentTime = startTime;
	////終了時間を取得
	//endTime = keyframeData->keyframeObjects.back();
	////キーフレームを再生中にする
	//isPlay = true;
}
