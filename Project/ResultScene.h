#pragma once
#include <memory>
#include "BaseScene.h"
#include "SceneManager.h"

#include "Input.h"
#include "Light.h"
#include "Sprite.h"
#include "Camera.h"

class ResultScene : public BaseScene
{
public:
	ResultScene() = default;
	~ResultScene() = default;

	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;

	/// <summary>
	/// �I������
	/// </summary>
	void Finalize() override;

private:
	Input* input_ = nullptr;
	std::unique_ptr<Camera> camera;
	std::unique_ptr<Light> light;
	SceneManager* sceneManager_ = nullptr;

private:
	
};

