#pragma once
#include "System.h"
#include "glm/vec2.hpp"

struct GLFWwindow;	// forward reference for GetWindowHandle()

class PlatformSystem : public System
{
public:
	GLFWwindow* GetWindowHandle() const;
	bool WindowClosing() const;

	// Returns width and height as a vec2
	glm::vec2 GetWindowDimensions() const;



private:
	int _width, _height;
	GLFWwindow* _window;

	// Inherited via System
	virtual void OnInit() override;
	virtual void OnUpdate(float dt) override;
	virtual void OnExit() override;

	// Unused virtuals
	virtual void OnFixedUpdate() override {}
	virtual void OnSceneLoad() override {}
	virtual void OnSceneInit() override {}
	virtual void OnSceneExit() override {}


    PlatformSystem(const char* w_name = "Prototype", int w_width = 800, int w_height = 600);

public:

    /**
     * @brief gets the PlatformSystem instance, creating one if it doesn't exist
     * @return a pointer to the PlatformSystem instance
    */
    static PlatformSystem * getInstance();

    /**
     * @brief gets the PlatformSystem instance, creating one if it doesn't exist
     * @param w_name the name of the window to create
     * @param w_width the width of the window to create
     * @param w_height the height of the window to create
     * @return a pointer to the PlatformSystem instance
    */
    static PlatformSystem * getInstance( const char* w_name, int w_width, int w_height );
    
    // Prevent copying (later, gotta prevent multiple instances in general)
    PlatformSystem(PlatformSystem& other) = delete;
    void operator=(const PlatformSystem&) = delete;
};
