#pragma once
#include "BaseBullet.h"
#include "ObjObject3d.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <string>
MYENGINE_SUPPRESS_WARNINGS_END

class BulletManager
{
public:
	//インスタンス
	static BulletManager* Create(ObjObject3d* obj, const std::string filePath);

	void Initialize(const std::string& modelName,Camera* camera);

	void Update();

	void Draw();

private:
	Camera* camera_ = nullptr;
	std::string modelName_;
};

