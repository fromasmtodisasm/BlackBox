#include <BlackBox/Render/CBaseShader.hpp>
#include <BlackBox/IEngine.hpp>
#include <BlackBox/ILog.hpp>
#include <BlackBox/Render/OpenglDebug.hpp>
#include <BlackBox/Render/Pipeline.hpp>

#include <fstream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

char* CBaseShaderProgram::buffer = nullptr;

ShaderStatus::ShaderStatus(CShader *shader) :
  m_Shader(shader)
{

}

bool ShaderStatus::get(GLenum statusType) {
  glCheck(glCheck(glGetShaderiv(m_Shader->get(), statusType, &m_Status)));
  if(m_Status != GL_TRUE)
  {
    glCheck(glGetShaderInfoLog(m_Shader->get(), 512, NULL, infoLog));
    GetIEngine()->getILog()->AddLog("[ERROR] Shader %s \n %s\n", m_Shader->getName().c_str(), infoLog);;
    return false;
  }
  return true;
 
}

ShaderProgramStatus::ShaderProgramStatus(CBaseShaderProgram *program) :
  m_Program(program)
{

}

bool ShaderProgramStatus::get(GLenum statusType) {
  GLsizei size;
  glCheck(glGetProgramiv(m_Program->get(), statusType, &m_Status));
  if(m_Status != GL_TRUE)
  {
    glCheck(glGetProgramInfoLog(m_Program->get(), 512, &size, infoLog));
    GetIEngine()->getILog()->AddLog("[ERROR] Shader::programm: %s\n", infoLog);
    return false;
  }
  return true;
 
}
CShader::CShader(string text, CShader::type type) :
  m_Text(text), m_Type(type), m_Status(this) 
{
  
}

CShader::~CShader() {
  glCheck(glDeleteShader(m_Shader));
}

bool CShader::create() {
  m_Shader = glCreateShader(m_Type);
  if (m_Shader != 0) { return true; }
  else { return false; }
  // return m_Status.get(GL_VALIDATE_STATUS);
}


std::shared_ptr<CShader> CShader::load(string path, CShader::type type) {
  string text;

	if (!loadInternal(path, text)) return nullptr;

  auto shader = std::make_shared<CShader>(text, type);
  if (!shader->create())
    return nullptr;
  shader->compile();
  shader->print();
  return shader;
}

bool CShader::parseLine(std::ifstream &fin, std::string& buffer)
{
	if (!getline(fin, buffer))
		return false;
	size_t pos = 0;
	if ((pos = buffer.find("#include")) != buffer.npos)
	{
		size_t begin, end;
		if ((begin = buffer.find_first_of('<')) != buffer.npos)
			end = buffer.find_first_of('>');
		else if ((begin = buffer.find_first_of('"')) != buffer.npos)
			end = buffer.find('"', begin + 1);
		else
			return false;

		std::string file(buffer.substr(begin + 1, end - begin - 1));
		std::string buff;
		if (!loadInternal("res/shaders/" + file, buff)) return false;
		buffer.clear();
		buffer += buff;
	}
		
	return true;
}

bool CShader::loadInternal(std::string &path, std::string& buffer)
{
  ifstream fin(path);
  string buff;

  if (!fin.is_open()) return false;
  
  while (parseLine(fin, buff)) {
    buffer += buff;
    buffer += '\n';
  }
	fin.close();
	return true;
}

std::shared_ptr<CShader> CShader::loadFromMemory(std::string text, CShader::type type)
{
  auto shader = std::make_shared<CShader>(text, type);
  if (!shader->create())
    return nullptr;
  shader->compile();
  shader->print();
  return shader;
}

bool CShader::compile() {
  const char *text = m_Text.c_str();
  glCheck(glShaderSource(m_Shader, 1, &text, nullptr));
  glCheck(glCompileShader(m_Shader));
  return m_Status.get(GL_COMPILE_STATUS);
}

bool CShader::bind() {
 return true; 
}

void CShader::print() {
  //cout << m_Text << endl;
}

string CShader::typeToStr()
{
  switch (m_Type) {
  case E_VERTEX:
    return "vertex";
  case E_FRAGMENT:
    return "fragment";
  }
}

string CShader::getName() {
  return m_Path;
}

GLuint CShader::get() {
  return m_Shader;
}

CBaseShaderProgram::CBaseShaderProgram() : m_Status(this)
{
  m_Program = glCreateProgram();
  created = true;
  buffer = new char[BUFFER_SIZE];
}

CBaseShaderProgram::CBaseShaderProgram(
  std::shared_ptr<CShader> vs, std::shared_ptr<CShader> fs) :
  m_Status(this)
{
  m_Vertex = vs;
  m_Fragment = fs;
  buffer = new char[BUFFER_SIZE];
}

CBaseShaderProgram::~CBaseShaderProgram() {
  if (buffer != nullptr)
    delete[] buffer;
}

bool CBaseShaderProgram::create() {
  if (!created) m_Program = glCreateProgram();
  if (!attached)
  {
    attach(m_Vertex);
    attach(m_Fragment);
  }
  link();
	return m_Status.get(GL_LINK_STATUS);
}

void CBaseShaderProgram::attach(std::shared_ptr<CShader> shader) {
  switch (shader->m_Type) {
  case CShader::type::E_VERTEX:
    if (m_Vertex == nullptr)
      m_Vertex = shader;
    break;
  case CShader::type::E_FRAGMENT:
    if (m_Fragment == nullptr)
      m_Fragment = shader;
    break;
  }
  glCheck(glAttachShader(m_Program, shader->get()));
  attached = true;
}

bool CBaseShaderProgram::link() {
  glCheck(glLinkProgram(m_Program));
  //delete m_Vertex;
  //delete m_Fragment;
	return m_Status.get(GL_LINK_STATUS);
}

void CBaseShaderProgram::use() {
    glCheck(glUseProgram(m_Program));
}

void CBaseShaderProgram::unuse()
{
    glCheck(glUseProgram(0));
}

GLint CBaseShaderProgram::getUniformLocation(const char* name)
{
  GLint loc = -1;
  auto it = m_Cache.find(name);
  if (it != m_Cache.end())
    loc = it->second;
  else
    loc = glGetUniformLocation(m_Program, name);
  return loc;
}

GLint CBaseShaderProgram::getUniformLocation(std::string& name)
{
  return getUniformLocation(name.c_str());
}

UniformValue CBaseShaderProgram::getUniformValue(const char* name)
{
  UniformValue result;
  auto location = getUniformLocation(name);

  result.location = location;
  return result;
}

void CBaseShaderProgram::setUniformValue(int value, const char * format, ...)
{
  va_list ptr;
  va_start(ptr, format);
  auto name = buildName(format, ptr);
  va_end(ptr);

  GLint loc = getUniformLocation(name);
  if (loc != -1){
      glCheck(glUniform1i(loc, value));
  }
}

void CBaseShaderProgram::setUniformValue(float value, const char * format, ...)
{
  va_list ptr;
  va_start(ptr, format);
  auto name = buildName(format, ptr);
  va_end(ptr);

  GLint loc = getUniformLocation(name);
  if (loc != -1){
        glCheck(glUniform1f(loc, value));
  }
}

void CBaseShaderProgram::setUniformValue(glm::vec1 value, const char * format, ...)
{
  va_list ptr;
  va_start(ptr, format);
  auto name = buildName(format, ptr);
  va_end(ptr);

  GLint loc = getUniformLocation(name);
  if (loc != -1){
        glCheck(glUniform1f(loc, value[0]));
  }
}

void CBaseShaderProgram::setUniformValue(glm::vec2 value, const char * format, ...)
{
  va_list ptr;
  va_start(ptr, format);
  auto name = buildName(format, ptr);
  va_end(ptr);

  GLint loc = getUniformLocation(name);
  if (loc != -1){
        glCheck(glUniform2fv(loc, 1, glm::value_ptr(value)));
  }
}

void CBaseShaderProgram::setUniformValue(glm::vec3 value, const char * format, ...)
{
  va_list ptr;
  va_start(ptr, format);
  auto name = buildName(format, ptr);
  va_end(ptr);

  GLint loc = getUniformLocation(name);
  if (loc != -1){
      glCheck(glUniform3fv(loc, 1, glm::value_ptr(value)));
  }
}

void CBaseShaderProgram::setUniformValue(glm::vec4 value, const char * format, ...)
{
  va_list ptr;
  va_start(ptr, format);
  auto name = buildName(format, ptr);
  va_end(ptr);

  GLint loc = getUniformLocation(name);
  if (loc != -1){
        glCheck(glUniform4fv(loc, 1, glm::value_ptr(value)));
  }
}

void CBaseShaderProgram::setUniformValue(glm::mat2 value, const char * format, ...)
{
  va_list ptr;
  va_start(ptr, format);
  auto name = buildName(format, ptr);
  va_end(ptr);

  GLint loc = getUniformLocation(name);
  if (loc != -1){
    glCheck(glUniformMatrix2fv(loc, 1, GL_FALSE, glm::value_ptr(value)));
  }
}

void CBaseShaderProgram::setUniformValue(glm::mat3 value, const char * format, ...)
{
  va_list ptr;
  va_start(ptr, format);
  auto name = buildName(format, ptr);
  va_end(ptr);

  GLint loc = getUniformLocation(name);
  if (loc != -1){
    glCheck(glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(value)));
  }
}

void CBaseShaderProgram::setUniformValue(glm::mat4 value, const char * format, ...)
{
  va_list ptr;
  va_start(ptr, format);
  auto name = buildName(format, ptr);
  va_end(ptr);

  GLint loc = getUniformLocation(name);
  if (loc != -1){
    glCheck(glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value)));
  }
}

GLuint CBaseShaderProgram::get() {
  return m_Program;
}

const char* CBaseShaderProgram::buildName(const char* format, va_list args)
{
  vsprintf(buffer, format, args);
  return buffer;
}

