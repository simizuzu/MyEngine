#pragma once

#include "SuppressWarning.h"
MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <string>
MYENGINE_SUPPRESS_WARNINGS_END
#include "BaseScene.h"

/**
 * @class AbstractSceneFactory.h
 * @brief シーンファクトリークラス
 */

/// <summary>
/// シーンファクトリー
/// </summary>
class AbstractSceneFactory
{
public:
	// 仮想デストラクタ
	virtual ~AbstractSceneFactory() = default;
	// シーン生成
	virtual BaseScene* CreateScene(const std::string& sceneName) = 0;
};
