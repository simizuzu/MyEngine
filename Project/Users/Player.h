#pragma once
#include "PlayerBullet.h"

class Player
{
public:
	void Initialize();

	void Update();

	void Draw();

	void Attack();

private:
	PlayerBullet* bullet = nullptr;
};

