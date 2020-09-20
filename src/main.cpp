#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <iostream>
#include <string>

#include "ibuffer.h"
#include "renderer.h"
#include "shader.h"
#include "texture.h"
#include "varray.h"
#include "vblayout.h"
#include "vbuffer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"

unsigned int SRC_WIDTH = 800;
unsigned int SRC_HEIGHT = 600;

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
  SRC_WIDTH = width;
  SRC_HEIGHT = height;
}

// Define main function
int main() {
  // Initialize GLFW
  if (!glfwInit())
    return -1;

  // Define version and compatibility settings
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);

  // Create OpenGL window and context
  GLFWwindow *window =
      glfwCreateWindow(SRC_WIDTH, SRC_HEIGHT, "yup", NULL, NULL);
  glfwMakeContextCurrent(window);

  // Check for window creation failure
  if (!window) {
    // Terminate GLFW
    glfwTerminate();
    return 0;
  }

  glfwSwapInterval(1);

  // Initialize GLEW
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK)
    return -1;

  // Version
  std::cout << glGetString(GL_VERSION) << std::endl;

  // Nr of vertex attribs supported
  int nrAttributes;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);

  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;

  ImGui::StyleColorsDark();

  const char *glsl_version = "#version 150";
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  {
    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

        -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
        0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

    unsigned int indices[] = {0, 1, 2, 2, 3, 0};

    GlCall(glEnable(GL_DEPTH_TEST));
    GlCall(glEnable(GL_BLEND));
    GlCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    VertexArray va;
    VertexBuffer vb(vertices, sizeof(vertices));

    VertexBufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(2);

    va.AddBuffer(vb, layout);

    IndexBuffer ib(indices, 6);

    std::string vertexShader = Shader::ReadFile("../src/glsl/vertex.glsl");
    std::string fragmentShader = Shader::ReadFile("../src/glsl/fragment.glsl");

    Shader shader = Shader(vertexShader, fragmentShader);
    shader.Bind();

    shader.SetUniform4f("u_Color", 0.2f, 0.5f, 0.8f, 1.0f);

    Texture texture1 = Texture("../res/textures/awesome.png");
    shader.SetUniform1i("u_Texture1", 0);

    Texture texture2 = Texture("../res/textures/travel.png");
    shader.SetUniform1i("u_Texture2", 1);

    ib.Unbind();
    va.Unbind();
    shader.Unbind();

    Renderer renderer;

    texture1.Bind(0);
    texture2.Bind(1);

    glm::vec3 translation = glm::vec3(0.0f, 0.0f, -3.0f);

    bool imgui_window = true;
    // Event loop
    while (!glfwWindowShouldClose(window)) {
      glfwPollEvents();

      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();

      glm::mat4 proj =
          glm::perspective(glm::radians(45.0f),
                           (float)SRC_WIDTH / (float)SRC_HEIGHT, 0.1f, 100.0f);
      glm::mat4 view = glm::translate(glm::mat4(1.0f), translation);

      ImGui::Begin("Another Window", &imgui_window);
      ImGui::SliderFloat3("Projection", &translation.x, -10.0f, 10.0f);
      ImGui::Text("Hello from another window!");

      ImGui::End();

      renderer.Clear();

      va.Bind();
      shader.Bind();

      for (unsigned int i = 0; i < 10; i++) {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), cubePositions[i]);
        model = glm::rotate(
            model,
            glm::radians((float)glfwGetTime() * (float)(20.0f * (i + 1))),
            glm::vec3(1.0f, 0.3f, 0.5f));

        glm::mat4 mvp = proj * view * model;

        shader.SetUniformMat4f("u_MVP", mvp);
        GlCall(glDrawArrays(GL_TRIANGLES, 0, 36));
      }

      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
      processInput(window);
      glfwSwapBuffers(window);
    }
  }

  // Cleanup
  ImGui_ImplGlfw_Shutdown();
  ImGui_ImplOpenGL3_Shutdown();
  ImGui::DestroyContext();
  glfwTerminate();
  return 0;
}
