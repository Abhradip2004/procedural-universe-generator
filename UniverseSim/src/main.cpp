#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <iostream>
#include <vector>

#include "Universe.h"


GLuint compileShader(GLenum type, const char* src)
{
    GLuint s = glCreateShader(type);
    glShaderSource(s, 1, &src, NULL);
    glCompileShader(s);

    int ok;
    glGetShaderiv(s, GL_COMPILE_STATUS, &ok);

    if (!ok)
    {
        char log[512];
        glGetShaderInfoLog(s, 512, NULL, log);
        std::cout << log << std::endl;
    }

    return s;
}


GLuint createProgram(const char* vs, const char* fs)
{
    GLuint v = compileShader(GL_VERTEX_SHADER, vs);
    GLuint f = compileShader(GL_FRAGMENT_SHADER, fs);

    GLuint p = glCreateProgram();

    glAttachShader(p, v);
    glAttachShader(p, f);

    glLinkProgram(p);

    glDeleteShader(v);
    glDeleteShader(f);

    return p;
}


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

    glEnable(GL_PROGRAM_POINT_SIZE);    

    // ---------- IMGUI ----------

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 450");


    // ---------- SHADERS ----------

    const char* vs = R"(

    #version 450

    layout (location = 0) in vec2 pos;

    void main()
    {
        gl_Position = vec4(pos, 0.0, 1.0);
        gl_PointSize = 20.0;
    }

    )";


    const char* fs = R"(

    #version 450

    out vec4 color;

    void main()
    {
        color = vec4(1.0, 1.0, 1.0, 1.0);
    }

    )";


    GLuint program = createProgram(vs, fs);


    // ---------- VAO / VBO ----------

    GLuint vao;
    GLuint vbo;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(
        GL_ARRAY_BUFFER,
        1000 * sizeof(float) * 2,
        NULL,
        GL_DYNAMIC_DRAW
    );

    glVertexAttribPointer(
        0,
        2,
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(float),
        (void*)0
    );

    glEnableVertexAttribArray(0);


    // ---------- UNIVERSE ----------

    Universe universe;

    Body a;
    a.x = -0.3f;
    a.y = 0.0f;
    a.vx = 0.0f;
    a.vy = 0.5f;
    a.mass = 10.0f;
    a.radius = 3.0f;

    Body b;
    b.x = 0.3f;
    b.y = 0.0f;
    b.vx = 0.0f;
    b.vy = -0.5f;
    b.mass = 10.0f;
    b.radius = 3.0f;

    universe.bodies.push_back(a);
    universe.bodies.push_back(b);


    // ---------- LOOP ----------

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        float dt = 0.016f;

        universe.update(dt);


        // ---------- IMGUI ----------

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Universe");

        ImGui::Text("Bodies: %d",
            (int)universe.bodies.size());

        ImGui::Text("FPS: %.1f",
            io.Framerate);

        ImGui::End();

        ImGui::Render();


        // ---------- RENDER ----------

        glClearColor(0.02f, 0.02f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        std::vector<float> verts;

        for (auto& b : universe.bodies)
        {
            verts.push_back(b.x);
            verts.push_back(b.y);
        }


        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glBufferSubData(
            GL_ARRAY_BUFFER,
            0,
            verts.size() * sizeof(float),
            verts.data()
        );


        glUseProgram(program);
        glBindVertexArray(vao);

        glDrawArrays(
            GL_POINTS,
            0,
            universe.bodies.size()
        );


        ImGui_ImplOpenGL3_RenderDrawData(
            ImGui::GetDrawData()
        );

        glfwSwapBuffers(window);
    }


    // ---------- SHUTDOWN ----------

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
}