#include "Paddle.h"

Paddle::Paddle()
{
  Vertex vertices[] = 
  { //CCW dir to avoid culling
    //position                                        //color                         
    {glm::vec3(-0.5f, -0.5f, 0.5f),   glm::vec3(0.0f, 0.0f, 1.0f)}, //closer bottom left
    {glm::vec3(0.5f, -0.5f, 0.5f),    glm::vec3(1.0f, 0.0f, 1.0f)}, //closer bottom right
    {glm::vec3(-0.5f, 0.5f, 0.5f),    glm::vec3(0.0f, 1.0f, 1.0f)}, //closer upper left
    {glm::vec3(0.5f, 0.5f, 0.5f),     glm::vec3(1.0f, 1.0f, 1.0f)}, //closer upper right

    {glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec3(0.0f, 0.0f, 0.0f)}, //further bottom left
    {glm::vec3(0.5f, -0.5f, -0.5f),   glm::vec3(1.0f, 0.0f, 0.0f)}, //further bottom right
    {glm::vec3(-0.5f, 0.5f, -0.5f),   glm::vec3(0.0f, 1.0f, 0.0f)}, //further upper left
    {glm::vec3(0.5f, 0.5f, -0.5f),    glm::vec3(1.0f, 1.0f, 0.0f)}  //further upper right
  };

  GLuint indices[] =
  {
    2, 0, 1,
    1, 3, 2, //front
    6, 4, 0,
    0, 2, 6, //left
    3, 1, 5,
    5, 7, 3, //right
    7, 5, 4,
    4, 6, 7, //back
    6, 2, 3,
    3, 7, 6, //top
    5, 1, 0,
    0, 4, 5  //bottom
  };

  indices_count_ = sizeof(indices) / sizeof(GLuint);

  //Vertex Array Buffer (big box)
  glGenBuffers(1, &vao_); 
  glBindVertexArray(vao_);

  //Vetrex Buffer Object
  glGenBuffers(1, &vbo_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //POSITION
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
  glEnableVertexAttribArray(0);
    //COLOR
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, color)));
  glEnableVertexAttribArray(1);

  //Element Buffer Array
  glGenBuffers(1, &ebo_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glBindVertexArray(0); //unbind - just in case

  model_matrix_ = glm::mat4(1.0f); //identity matrix (no scaling, 0 rotation and position of model at world origin)
}

void Paddle::Draw(Shader* shader)
{
  //bind VAO
  glBindVertexArray(vao_);

  //set shader uniform
  shader->setUniform("model", model_matrix_);

  //draw mesh
  glDrawElements(GL_TRIANGLES, indices_count_, GL_UNSIGNED_INT, 0);

  //unbind VAO
  glBindVertexArray(0);
}

Paddle::~Paddle()
{
  //delete shader_;
}
