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

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
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

  // Create OpenGL window and context
  GLFWwindow *window = glfwCreateWindow(800, 600, "yup", NULL, NULL);
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
  std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes
            << std::endl;

  // TODO: glViewport not working properly
  // glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
  {
    float vertices[] = {
        // positions
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, -0.5f, 0.5f,  0.0f, 0.0f, 1.0f,
        0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,
    };

    unsigned int indices[] = {0, 1, 2, 2, 3, 0};

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

    Texture texture = Texture("../res/textures/awesome.png");
    texture.Bind();
    shader.SetUniform1i("u_Texture", 0);

    ib.Unbind();
    va.Unbind();
    shader.Unbind();

    Renderer renderer;

    // Event loop
    while (!glfwWindowShouldClose(window)) {
      processInput(window);
      renderer.Clear();

      renderer.Draw(va, ib, shader);

      glfwSwapBuffers(window);
      glfwPollEvents();
    }
  }
  // Terminate GLFW

  glfwTerminate();
  return 0;
}
