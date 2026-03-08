#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <iostream>

int main()
{
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window =
        glfwCreateWindow(1280, 720, "UniverseSim", NULL, NULL);

    if (!window)
        return -1;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGL())
    {
        std::cout << "GLAD failed\n";
        return -1;
    }

    // ---------------- IMGUI INIT ----------------

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 450");

    // ---------------- LOOP ----------------

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // start imgui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // UI
        ImGui::Begin("Universe Debug");

        ImGui::Text("Procedural Universe Generator");
        ImGui::Text("FPS: %.1f", io.Framerate);

        static float value = 0.0f;
        ImGui::SliderFloat("test", &value, 0.0f, 1.0f);

        ImGui::End();

        // render
        ImGui::Render();

        glClearColor(0.02f, 0.02f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(
            ImGui::GetDrawData()
        );

        glfwSwapBuffers(window);
    }

    // ---------------- SHUTDOWN ----------------

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
}