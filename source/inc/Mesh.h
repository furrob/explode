#pragma once

#include <vector>

#include <GL/glew.h>

#include "vertex.h"
#include "Shader.h"

class Mesh
{
private:
  GLuint vao_, vbo_;
  GLsizei vertices_count_;

  void SetupMesh();

public:
  Mesh(std::vector<Vertex> vertices);

  Mesh();

  void Draw(Shader &shader);

  std::vector<Vertex> vertices_;
};

