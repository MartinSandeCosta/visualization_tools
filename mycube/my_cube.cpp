#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>


// keep track of window size for things like the viewport and the mouse cursor
int g_gl_width = 640;
int g_gl_height = 480;

// a call-back function
void glfw_window_size_callback(GLFWwindow* window, int width, int height) {
  g_gl_width = width;
  g_gl_height = height;
}

int main() {
  // start GL context and O/S window using the GLFW helper library
  if (!glfwInit()) {
    fprintf(stderr, "ERROR: could not start GLFW3\n");
    return 1;
  }

  //  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  //  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  //  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  //  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(640, 480, "My spinning cube", NULL, NULL);
  if (!window) {
    fprintf(stderr, "ERROR: could not open window with GLFW3\n");
    glfwTerminate();
    return 1;
  }
  glfwSetWindowSizeCallback(window, glfw_window_size_callback);
  glfwMakeContextCurrent(window);

  // start GLEW extension handler
  // glewExperimental = GL_TRUE;
  glewInit();

  // get version info
  const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
  const GLubyte* glversion = glGetString(GL_VERSION); // version as a string
  const GLubyte* glslversion = glGetString(GL_SHADING_LANGUAGE_VERSION); // version as a string
  printf("Renderer: %s\n", renderer);
  printf("OpenGL version supported %s\n", glversion);
  printf("GLSL version supported %s\n", glslversion);

  // tell GL to only draw onto a pixel if the shape is closer to the viewer
  glEnable(GL_DEPTH_TEST); // enable depth-testing
  glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

  // Vertex Shader
  const char* vertex_shader =
    {
      "#version 410 core                                                  \n"
      "                                                                   \n"
      "layout(location = 0) in vec4 position;                             \n"
      "layout(location = 1) in vec3 color;                                \n"
      "                                                                   \n"
      "out VS_OUT                                                         \n"
      "{                                                                  \n"
      "    vec4 color;                                                    \n"
      "} vs_out;                                                          \n"
      "                                                                   \n"
      "uniform mat4 mv_matrix;                                            \n"
      "uniform mat4 proj_matrix;                                          \n"
      "                                                                   \n"
      "void main(void)                                                    \n"
      "{                                                                  \n"
      "    gl_Position = proj_matrix * mv_matrix * position;              \n"
      "    vs_out.color = vec4(color, 1.0);                               \n"
      "}                                                                  \n"
    };

  // Fragment Shader
  const char* fragment_shader =
    {            
      "#version 410 core                                                  \n"
      "                                                                   \n"
      "out vec4 color_out;                                                \n"
      "                                                                   \n"
      "in VS_OUT                                                          \n"
      "{                                                                  \n"
      "    vec4 color;                                                    \n"
      "} fs_in;                                                           \n"
      "                                                                   \n"
      "void main(void)                                                    \n"
      "{                                                                  \n"
      "    color_out = fs_in.color;                                       \n"
      "}                                                                  \n"
    };

  // Shaders compilation
  GLuint vs = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vs, 1, &vertex_shader, NULL);
  glCompileShader(vs);
  GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fs, 1, &fragment_shader, NULL);
  glCompileShader(fs);

  // Create program, attach shaders to it and link it
  GLuint shader_programme = glCreateProgram();
  glAttachShader(shader_programme, fs);
  glAttachShader(shader_programme, vs);
  glLinkProgram(shader_programme);

  // Vertex Array Object
  GLuint vao = 0;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Cube to be rendered
  const GLfloat vertex_positions[] = {
    -0.25f,  0.25f, -0.25f,
    -0.25f, -0.25f, -0.25f,
     0.25f, -0.25f, -0.25f,

     0.25f, -0.25f, -0.25f,
     0.25f,  0.25f, -0.25f,
    -0.25f,  0.25f, -0.25f,

     0.25f, -0.25f, -0.25f,
     0.25f, -0.25f,  0.25f,
     0.25f,  0.25f, -0.25f,

     0.25f, -0.25f,  0.25f,
     0.25f,  0.25f,  0.25f,
     0.25f,  0.25f, -0.25f,

     0.25f, -0.25f,  0.25f,
    -0.25f, -0.25f,  0.25f,
     0.25f,  0.25f,  0.25f,

    -0.25f, -0.25f,  0.25f,
    -0.25f,  0.25f,  0.25f,
     0.25f,  0.25f,  0.25f,

    -0.25f, -0.25f,  0.25f,
    -0.25f, -0.25f, -0.25f,
    -0.25f,  0.25f,  0.25f,

    -0.25f, -0.25f, -0.25f,
    -0.25f,  0.25f, -0.25f,
    -0.25f,  0.25f,  0.25f,

    -0.25f, -0.25f,  0.25f,
     0.25f, -0.25f,  0.25f,
     0.25f, -0.25f, -0.25f,

     0.25f, -0.25f, -0.25f,
    -0.25f, -0.25f, -0.25f,
    -0.25f, -0.25f,  0.25f,

    -0.25f,  0.25f, -0.25f,
     0.25f,  0.25f, -0.25f,
     0.25f,  0.25f,  0.25f,

     0.25f,  0.25f,  0.25f,
    -0.25f,  0.25f,  0.25f,
    -0.25f,  0.25f, -0.25f
  };

  // Vertex Buffer Object(s)
  GLuint vbo = 0;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER,
                sizeof(vertex_positions),
                vertex_positions,
                GL_STATIC_DRAW);

  // Vertex attributes
  // 0: vertex position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  glEnableVertexAttribArray(0);

// ------ ------ ------ ------ ------ ------ ------ ------ ------
  // 1: vertex color
  const GLfloat vertex_colors[] = {
    1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,

    1.0f, 0.7f, 0.8f,
    1.0f, 0.7f, 0.8f,
    1.0f, 0.7f, 0.8f,

    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,

    1.0f, 0.6f, 0.0f,
    1.0f, 0.6f, 0.0f,
    1.0f, 0.6f, 0.0f,

    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,

    0.5f, 1.0f, 0.0f,
    0.5f, 1.0f, 0.0f,
    0.5f, 1.0f, 0.0f,

    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,

    0.5f, 0.0f, 1.0f,
    0.5f, 0.0f, 1.0f,
    0.5f, 0.0f, 1.0f,

    0.0f, 1.0f, 0.5f,
    0.0f, 1.0f, 0.5f,
    0.0f, 1.0f, 0.5f,

    0.0f, 0.5f, 1.0f,
    0.0f, 0.5f, 1.0f,
    0.0f, 0.5f, 1.0f,

    0.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f,

    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
  };
  // Color Buffer Object(s)
  GLuint cbo = 0;
  glGenBuffers(1, &cbo);
  glBindBuffer(GL_ARRAY_BUFFER, cbo);
  glBufferData(GL_ARRAY_BUFFER,
                sizeof(vertex_colors),
                vertex_colors,
                GL_STATIC_DRAW);

  // Color attributes
  // 1: vertex color
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  glEnableVertexAttribArray(1);

// ------ ------ ------ ------ ------ ------ ------ ------ ------ ------

  // Uniforms
  // - Model-View matrix
  // - Projection matrix
  GLint mv_location, proj_location;
  mv_location = glGetUniformLocation(shader_programme, "mv_matrix");
  proj_location = glGetUniformLocation(shader_programme, "proj_matrix");

  // Render loop
  while(!glfwWindowShouldClose(window)) {

    double currentTime = glfwGetTime();
    float f = (float)currentTime * 0.3f;
    float aspect;

    // wipe the drawing surface clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, g_gl_width, g_gl_height);
    glUseProgram(shader_programme);
    glBindVertexArray(vao);
    
    // To manage the model-view and projection transformations:
    // - use routines and structures from GLM

    glm::mat4 mv_matrix, proj_matrix;

    mv_matrix = glm::translate(glm::mat4(1.f), glm::vec3(0.0f, 0.0f, -8.0f)) *
                glm::translate(glm::mat4(1.f), glm::vec3((sinf(2.1f * f) * 0.5f,
                                                          cosf(1.7f * f) * 0.5f,
                                                          sinf(1.3f * f) * cosf(1.5f * f) * 2.0f))) *
                glm::rotate(glm::mat4(1.f), (float)currentTime * glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f)) *
                glm::rotate(glm::mat4(1.f), (float)currentTime * glm::radians(81.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    
    // transfer mv_matrix uniform to shaders:
    glUniformMatrix4fv(mv_location, 1, GL_FALSE, glm::value_ptr(mv_matrix));

    proj_matrix = glm::perspective(50.0f, aspect, 0.1f, 1000.0f);
    aspect = (float)g_gl_width / (float)g_gl_height;

    // transfer proj_matrix uniform to shaders:
    glUniformMatrix4fv(proj_location, 1, GL_FALSE, glm::value_ptr(proj_matrix));

    glDrawArrays(GL_TRIANGLES, 0, 36);

    // update other events like input handling
    glfwPollEvents();
    // put the stuff we've been drawing onto the display
    glfwSwapBuffers(window);
  }

  // close GL context and any other GLFW resources
  glfwTerminate();

  return 0;
}