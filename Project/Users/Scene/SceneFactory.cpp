#include "SceneFactory.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "ResultScene.h"
#include "TransitionScene.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName)
{
    
    BaseScene* newScene = nullptr;

    if (sceneName == "TITLE") {
        newScene = new TitleScene();
    }
    else if (sceneName == "GAME") {
        newScene = new GameScene();
    }
    else if (sceneName == "RESULT") {
        newScene = new ResultScene();
    }
	else if ( sceneName == "TRANSITION" )
	{
		newScene = new TransitionScene();
	}

    return newScene;
}
