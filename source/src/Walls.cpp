#include "Walls.h"

Walls::Walls(float front_depth, float back_depth, float half_width, float half_height)
{
  //create vertices
  Vertex vertices[] =
  {
    {glm::vec3(-half_width, half_height, front_depth), glm::vec2(0.0f), glm::vec3(0.0f)},//front left top     0
    {glm::vec3(half_width, half_height, front_depth), glm::vec2(0.0f), glm::vec3(0.0f)},//front right top     1
    {glm::vec3(-half_width, -half_height, front_depth), glm::vec2(0.0f), glm::vec3(0.0f)},//front bottom left 2
    {glm::vec3(half_width, -half_height, front_depth), glm::vec2(0.0f), glm::vec3(0.0f)},//front bottom right 3
    {glm::vec3(-half_width, half_height, back_depth), glm::vec2(0.0f), glm::vec3(0.0f)},//back left top     4
    {glm::vec3(half_width, half_height, back_depth), glm::vec2(0.0f), glm::vec3(0.0f)},//back right top     5
    {glm::vec3(-half_width, -half_height, back_depth), glm::vec2(0.0f), glm::vec3(0.0f)},//back bottom left 6
    {glm::vec3(half_width, -half_height, back_depth), glm::vec2(0.0f), glm::vec3(0.0f)},//back bottom right 7
  };
  /*{
    {glm::vec3(0.0f, 0.5f, -2.0f), glm::vec2(0.0f), glm::vec3(0.0f)},
    {glm::vec3(0.5f, 0.0f, -2.0f), glm::vec2(0.0f), glm::vec3(0.0f)},
    {glm::vec3(-0.5f, 0.0f, -2.0f), glm::vec2(0.0f), glm::vec3(0.0f)}
  };*/
  //GLuint vertices_count = sizeof(vertices) / sizeof(float);

  GLuint indices[] =
  {
    0, 2, 6,
    0, 4, 6, //left
    1, 3, 7,
    1, 5, 7,//right
    1, 0, 4,
    1, 5, 4,//top
    2, 3, 7,
    2, 6, 7//bottom
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
