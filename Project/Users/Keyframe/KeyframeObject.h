#pragma once
#include "LevelLoader.h"

class KeyframeObject
{
public:
	void Initialize();

	void Update();

private:
	LevelData* keyframeData;
	std::vector<LevelData::AnimData> points;
	
};

