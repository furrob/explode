#include "OBJLoader.h"

std::vector<Vertex> OBJLoader::loadOBJ(const char* file_name)
{
  //vertices
  std::vector<glm::fvec3> vertex_positions;
  std::vector<glm::fvec3> vertex_normals;
  std::vector<glm::fvec2> vertex_texcoords;

  //faces
  std::vector<GLuint> vertex_position_indices;
  std::vector<GLuint> vertex_texcoord_indices;
  std::vector<GLuint> vertex_normal_indices;

  //Vertex
  std::vector<Vertex> vertices;

  std::stringstream ss;
  std::ifstream in_file(file_name);
  std::string line = "";
  std::string prefix = "";

  glm::vec3 temp_vec3;
  glm::vec2 temp_vec2;
  GLuint temp_gluint = 0;

  //error check
  if(!in_file.is_open())
  {
    MessageBoxW(NULL, L"ERROR::OBJLOADER::Could not open *.obj file", L"Error", MB_OK | MB_ICONERROR);
    throw std::exception("ERROR::OBJLOADER::Could not open *.obj file");
    //something else?
  }

  //one line at a time
  while(std::getline(in_file, line))
  {
    ss.clear();
    ss.str(line);
    ss >> prefix; //first word


    if(prefix == "v") //vertex position
    {
      ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
      vertex_positions.push_back(temp_vec3);
    }
    else if(prefix == "vt")
    {
      ss >> temp_vec2.x >> temp_vec2.y;
      vertex_texcoords.push_back(temp_vec2);
    }
    else if(prefix == "vn")
    {
      ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
      vertex_normals.push_back(temp_vec3);
    }
    else if(prefix == "f")
    {
      int counter = 0;
      while(ss >> temp_gluint)
      {
        //pushing into corresponding arrays
        switch(counter)
        {
          case 0:
            vertex_position_indices.push_back(temp_gluint);
            break;
          case 1:
            vertex_texcoord_indices.push_back(temp_gluint);
            break;
          case 2:
            vertex_normal_indices.push_back(temp_gluint);
            break;
        }

        if(ss.peek() == '/')
        {
          ++counter;
          ss.ignore(1, '/');
        }
        else if(ss.peek() == ' ')
        {
          ++counter;
          ss.ignore(1, ' ');
        }

        //reset the counter
        counter = (counter > 2) ? 0 : counter;

      }
    }
    else
    {
      //everything that is not vertex data
    }

  }

  //finall array
  vertices.resize(vertex_position_indices.size(), Vertex());

  //assemble everything
  for(int i = 0; i < vertices.size(); ++i)
  {
    vertices[i].position = vertex_positions[vertex_position_indices[i] - 1];
    vertices[i].texcoord = vertex_texcoords[vertex_texcoord_indices[i] - 1];
    vertices[i].normal = vertex_normals[vertex_normal_indices[i] - 1];
  }


  in_file.close();

  //success
  return vertices;
}
