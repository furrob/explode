#include "Walls.h"

Walls::Walls(float front_depth, float back_depth, float half_width, float half_height)
{
  //create vertices
  Vertex vertices[] =
  {
    {glm::vec3(-half_width, half_height, front_depth), glm::vec2(0.0f), glm::vec3(0.0f, -1.0f, 0.0f)},//front left top top    0
    {glm::vec3(-half_width, half_height, front_depth), glm::vec2(0.0f), glm::vec3(1.0f, 0.0f, 0.0f)},//front left top left    1
    {glm::vec3(half_width, half_height, front_depth), glm::vec2(0.0f), glm::vec3(0.0f, -1.0f, 0.0f)},//front right top top    2
    {glm::vec3(half_width, half_height, front_depth), glm::vec2(0.0f), glm::vec3(-1.0f, 0.0f, 0.0f)},//front right top right   3
    {glm::vec3(-half_width, -half_height, front_depth), glm::vec2(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)},//front bottom left bottom 4
    {glm::vec3(-half_width, -half_height, front_depth), glm::vec2(0.0f), glm::vec3(1.0f, 0.0f, 0.0f)},//front bottom left left 5
    {glm::vec3(half_width, -half_height, front_depth), glm::vec2(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)},//front bottom right bottom 6
    {glm::vec3(half_width, -half_height, front_depth), glm::vec2(0.0f), glm::vec3(-1.0f, 0.0f, 0.0f)},//front bottom right right 7
    {glm::vec3(-half_width, half_height, back_depth), glm::vec2(0.0f), glm::vec3(0.0f, -1.0f, 0.0f)},//back left top top     8
    {glm::vec3(-half_width, half_height, back_depth), glm::vec2(0.0f), glm::vec3(1.0f, 0.0f, 0.0f)},//back left top left     9
    {glm::vec3(half_width, half_height, back_depth), glm::vec2(0.0f), glm::vec3(0.0f, -1.0f, 0.0f)},//back right top top     10
    {glm::vec3(half_width, half_height, back_depth), glm::vec2(0.0f), glm::vec3(-1.0f, 0.0f, 0.0f)},//back right top right   11
    {glm::vec3(-half_width, -half_height, back_depth), glm::vec2(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)},//back bottom left bottom 12
    {glm::vec3(-half_width, -half_height, back_depth), glm::vec2(0.0f), glm::vec3(1.0f, 0.0f, 0.0f)},//back bottom left left 13
    {glm::vec3(half_width, -half_height, back_depth), glm::vec2(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)},//back bottom right bottom 14
    {glm::vec3(half_width, -half_height, back_depth), glm::vec2(0.0f), glm::vec3(-1.0f, 0.0f, 0.0f)},//back bottom right right 15
  };
  //GLuint vertices_count = sizeof(vertices) / sizeof(float);

  GLuint indices[] =
  {
    1, 5, 13,
    1 ,13, 9,//left
    3, 11, 15,
    3, 15, 7,//right
    0, 8, 2,
    2, 8, 10,//top
    6, 14, 12,
    6, 12, 4//bottom
  };
  indices_count_ = sizeof(indices) / sizeof(GLuint);

  glGenVertexArrays(1, &vao_);
  glBindVertexArray(vao_);

  glGenBuffers(1, &vbo_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  //position
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
  //texcoords
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texcoord)));
  //normal
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));

  glGenBuffers(1, &ebo_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glBindVertexArray(0);

  model_matrix_ = glm::mat4(1.0f);
}

void Walls::Draw(Shader* shader)
{
  //set shader uniform
  
  shader->setUniform("model", model_matrix_);

  glBindVertexArray(vao_);

  glDrawElements(GL_TRIANGLES, indices_count_, GL_UNSIGNED_INT, 0);

  //unbind VAO
  glBindVertexArray(0);
}
