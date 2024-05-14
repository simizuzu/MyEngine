#include "KeyframeObject.h"

void KeyframeObject::Initialize()
{
	keyframeData = LevelLoader::LoadKeyframe("anim");
}

void KeyframeObject::Update(LevelData* animeData)
{
	if ( isPlay )
	{
		//1フレーム進める
		currentTime += frameTime;
		//最後まで再生したら最後に固定する
		if ( currentTime > endTime )
		{
			currentTime = endTime;
		}
	}

	if ( animeData )
	{

	}

	//for ( LevelData::AnimData anim : animeData->anims )
	//{
	//	nodeAnimation = 
	//}

	//translation_ = MyMathUtility::CalculateValue(nodeAnimation.translate,currentTime);
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
