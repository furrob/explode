#include "Shader.h"

using namespace std;

Shader::Shader(const char* vertex_path, const char* fragment_path)
{
  char infoLog[512];
  string vertexCode;
  string fragmentCode;
  ifstream vShaderFile;
  ifstream fShaderFile;
  try
  {
    //opening shader source files
    vShaderFile.open(vertex_path);
    fShaderFile.open(fragment_path);
    stringstream vShaderStream, fShaderStream;
    //reading source into streams
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();
    //source file closing
    vShaderFile.close();
    fShaderFile.close();

    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();
  }
  catch(ifstream::failure e)
  {
    MessageBoxW(NULL, L"ERROR::SHADER::VERTEX::FILE_READ_FAILED", L"Error", MB_OK | MB_ICONERROR);
  }
  int success;
  const char* vShaderCode = vertexCode.c_str();
  const char* fShaderCode = fragmentCode.c_str();


  unsigned int vertex, fragment;

      // vertex shader
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderCode, NULL);
  glCompileShader(vertex);
  glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
  if(!success)
  {
    glGetShaderInfoLog(vertex, 512, NULL, infoLog);
    MessageBoxW(NULL, L"ERROR::SHADER::VERTEX::COMPILATION_FAILED", L"ERROR", MB_OK | MB_ICONERROR);
  }

      //fragment shader
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderCode, NULL);
  glCompileShader(fragment);
  glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
  if(!success)
  {
    glGetShaderInfoLog(fragment, 512, NULL, infoLog);
    MessageBoxW(NULL, L"ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n", L"ERROR", MB_OK | MB_ICONERROR);
  }
  ID = glCreateProgram();
  glAttachShader(ID, vertex);
  glAttachShader(ID, fragment);
  glLinkProgram(ID);
  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if(!success)
  {
    glGetProgramInfoLog(ID, 512, NULL, infoLog);
    MessageBoxW(NULL, L"ERROR::SHADER::PROGRAM::LINKING_FAILED\n", L"ERROR", MB_OK | MB_ICONERROR);
  }
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

Shader::~Shader()
{
  glDeleteProgram(ID);
}
