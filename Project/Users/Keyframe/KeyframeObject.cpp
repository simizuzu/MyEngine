#include "KeyframeObject.h"

void KeyframeObject::Initialize()
{
	keyframeData = LevelLoader::LoadFile("anim");
	
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
}

void KeyframeObject::IsPlayAnimation()
{
	//開始時間取得(1.0Fからスタート)
	currentTime = startTime;
	//終了時間を取得
	endTime = keyframeData->keyframeObjects.back();
	//キーフレームを再生中にする
	isPlay = true;
}
