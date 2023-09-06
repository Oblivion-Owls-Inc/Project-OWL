#pragma once

#include "BaseSystem.h"

class GLFWwindow;

class PlatformSystem : public System
{
public:
	PlatformSystem(const char* name, int w_width, int w_height);
	GLFWwindow* GetWindowHandle();

	// Inherited via System
	virtual void OnInit() override;
	virtual void OnUpdate(float dt) override;
	virtual void OnExit() override;

	// Unused virtuals
	virtual void OnFixedUpdate() override {}
	virtual void OnSceneLoad() override {}
	virtual void OnSceneInit() override {}
	virtual void OnSceneExit() override {}

private:
	int _width, _height;
	GLFWwindow* _window;
};
