#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "glew.h"
#include "glfw/glfw3.h"
#include <iostream>

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) 
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Request an OpenGL 4.3 core profile context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a GLFW window
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Example", NULL, NULL);
    if (!window) 
    {
        glfwTerminate();
        std::cerr << "Failed to create GLFW window" << std::endl;
        return -1;
    }
    // Make the window's context current
    glfwMakeContextCurrent(window);
#if 1
    // Initialize GLEW
    if (glewInit() != GLEW_OK) 
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
#endif
    // Setup ImGui context
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 430");

    // Set the clear color (background color)
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // Set up a callback for the Escape key
    glfwSetKeyCallback(window, keyCallback);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Poll for and process events
        glfwPollEvents();

        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Rendering code goes here
        ImGui::Begin("Mouse Position");

        // Get the mouse cursor position
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        // Display mouse position in ImGui window
        ImGui::Text("Mouse X: %.2f", mouseX);
        ImGui::Text("Mouse Y: %.2f", mouseY);

        ImGui::End();

        // ImGui rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap front and back buffers
        glfwSwapBuffers(window);
    }

    // Terminate GLFW
    glfwTerminate();

    return 0;
}
