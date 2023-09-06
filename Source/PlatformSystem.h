#pragma once
#include "System.h"
#include "glm/vec2.hpp"

struct GLFWwindow;	// forward reference for GetWindowHandle()

class PlatformSystem : public System
{
public:
	PlatformSystem(const char* w_name = "Prototype", int w_width = 800, int w_height = 600);
	GLFWwindow* GetWindowHandle() const;
	bool WindowClosing() const;

	// Returns width and height as a vec2
	glm::vec2 GetWindowDimensions() const;

	// Prevent copying (later, gotta prevent multiple instances in general)
	PlatformSystem(PlatformSystem& other) = delete;
	void operator=(const PlatformSystem&) = delete;


private:
	int _width, _height;
	GLFWwindow* _window;
	bool _initialized;

	// Inherited via System
	virtual void OnInit() override;
	virtual void OnUpdate(float dt) override;
	virtual void OnExit() override;

	// Unused virtuals
	virtual void OnFixedUpdate() override {}
	virtual void OnSceneLoad() override {}
	virtual void OnSceneInit() override {}
	virtual void OnSceneExit() override {}
};
