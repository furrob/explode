#include "Mesh.h"

void Mesh::SetupMesh()
{
  //create buffers/arrays
  glGenVertexArrays(1, &vao_);
  glGenBuffers(1, &vbo_);
  //glGenBuffers(1, &ebo_);
  //bind vao
  glBindVertexArray(vao_);
  //position, texcoords, normals
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), &vertices_[0], GL_STATIC_DRAW);
  //indices of vertices
  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
  //glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(GLuint), &indices_[0], GL_STATIC_DRAW);

  //position
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
  //texcoords
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texcoord)));
  //normal
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));

  //:)
  glBindVertexArray(0);

}

Mesh::Mesh(std::vector<Vertex> vertices)
{
  vertices_ = vertices;

  vertices_count_ = vertices_.size();

  SetupMesh();
}

Mesh::Mesh()
{
}

void Mesh::Draw(Shader& shader)
{
  glBindVertexArray(vao_);

  glDrawArrays(GL_TRIANGLES, 0, vertices_count_);

  glBindVertexArray(0);
}
