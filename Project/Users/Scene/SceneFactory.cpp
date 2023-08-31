#include "SceneFactory.h"
#include "TitleScene.h"
#include "GameScene.h"

#pragma warning(push)
#pragma warning(disable: 4091)
#pragma warning(pop)

BaseScene* SceneFactory::CreateScene(const std::string& sceneName)
{
    
    BaseScene* newScene = nullptr;

    if (sceneName == "TITLE") {
        newScene = new TitleScene();
    }
    else if (sceneName == "GAME") {
        newScene = new GameScene();
    }

    return newScene;
}
