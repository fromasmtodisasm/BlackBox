#include <BlackBox/Renderer/BaseShader.hpp>
#include <BlackBox/Renderer/OpenGL/Debug.hpp>
#include <BlackBox/Renderer/Pipeline.hpp>
#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/System/ILog.hpp>
#include <BlackBox/System/ISystem.hpp>

#include <fstream>
#include <string>
#include <cstdarg>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

char* CBaseShaderProgram::buffer = nullptr;

CShader::type str2typ(std::string type)
{
  if (type == "vertex")
    return CShader::type::E_VERTEX;
  else if (type == "fragment")
    return CShader::type::E_FRAGMENT;
  else if (type == "geometry")
    return CShader::type::E_GEOMETRY;
  else if (type == "compute")
    return CShader::type::E_COMPUTE;
  else
    return CShader::type::E_UNKNOWN;
}

ShaderStatus::ShaderStatus(CShader* shader) :
  m_Shader(shader)
{
}

bool ShaderStatus::get(GLenum statusType) {
  GLenum err;
  while((err = glGetError()) != GL_NO_ERROR)
  {
    GetISystem()->GetILog()->Log("current error %d\n", err);
  }
  glCheck(glGetShaderiv(m_Shader->get(), statusType, &m_Status));
  while((err = glGetError()) != GL_NO_ERROR)
  {
    GetISystem()->GetILog()->Log("current error %d\n", err);
  }
  if (m_Status != GL_TRUE)
  {
    GLsizei length = 0;
    glCheck(glGetShaderInfoLog(m_Shader->get(), 512, &length, m_InfoLog));
    if (length > 0)
    {
      GetISystem()->GetILog()->Log("[ERROR] Shader %s \n %s\n", m_Shader->getName(), m_InfoLog);;
    }
    return false;
  }
  return true;
}

ShaderProgramStatus::ShaderProgramStatus(CBaseShaderProgram* program) :
  m_Program(program)
{
}

bool ShaderProgramStatus::get(GLenum statusType) {
  GLsizei size;
  glCheck(glGetProgramiv(m_Program->Get(), statusType, &m_Status));
  if (m_Status != GL_TRUE)
  {
    glCheck(glGetProgramInfoLog(m_Program->Get(), 512, &size, m_InfoLog));
    auto log = GetISystem()->GetILog();
    if (log != nullptr)
    {
      log->Log("[ERROR] Shader::programm: %s\n", m_InfoLog);
      std::vector<char> label(1);
      GLsizei length = 0;
      glCheck(glGetObjectLabel(GL_PROGRAM, this->m_Program->Get(), 1, &length, label.data()));
      if (length > 0)
      {
        label.resize(length);
        glCheck(glGetObjectLabel(GL_PROGRAM, this->m_Program->Get(), length, &length, label.data()));
        log->Log("[INFO] Shader::programm label: %s\n", label.data());
      }
    }
    else
      GetISystem()->Log((std::string("[ERROR] Shader::programm: ") + m_InfoLog).c_str());
    return false;
  }
  return true;
}
CShader::CShader(string text, CShader::type type) :
  m_Text(text), m_Type(type), m_Status(this), m_Empty(true)
{
}

CShader::~CShader() {
  glCheck(glDeleteShader(m_Shader));
}

static int get_gl_enum(IShader::type type)
{
  switch (type)
  {
  case IShader::E_VERTEX:
    return GL_VERTEX_SHADER;
    break;
  case IShader::E_FRAGMENT:
    return GL_FRAGMENT_SHADER;
    break;
  case IShader::E_GEOMETRY:
    return GL_GEOMETRY_SHADER;
    break;
  case IShader::E_COMPUTE:
    return GL_COMPUTE_SHADER;
    break;
  case IShader::E_UNKNOWN:
    return -1;
    break;
  default:
    break;
  }
}

bool CShader::Create() {
  m_Shader = glCreateShader(get_gl_enum(m_Type));
  if (m_Shader != 0) { return true; }
  else { return false; }
  // return m_Status.get(GL_VALIDATE_STATUS);
}

CShader* CShader::load(ShaderDesc  const& desc) {
  string text;

  auto path = ShaderDesc::root + desc.name;
  if (!loadInternal(path, text)) return nullptr;

  if (desc.macro.size() > 0)
  {
    auto pos = text.find("#version");
    if (pos != std::string::npos)
    {
      auto end = text.find_first_of('\n', pos + 1);
      std::string defines;
      for (auto& define : desc.macro)
      {
        defines = "#define " + define.first + " " + define.second + "\n";
      }
      text.insert(end + 1, defines);
      std::cout << text << endl;
    }
  }

  auto shader = new CShader(text, str2typ(desc.type));
  if (shader->Create())
  {
    shader->Compile();
    shader->print();
    shader->m_Empty = false;
    debuger::shader_label(shader->get(), path);
  }
  return shader;
}

bool CShader::parseLine(std::ifstream& fin, std::string& buffer)
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

    if (!loadInternal(std::string("res/shaders/"), buff)) return false;
    buffer.clear();
    buffer += buff;
  }

  return true;
}

bool CShader::loadInternal(std::string const& path, std::string& buffer)
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

ShaderRef CShader::loadFromMemory(std::string text, IShader::type type)
{
  auto shader = _smart_ptr<CShader>(new CShader(text, type));
  if (!shader->Create())
    return nullptr;
  shader->Compile();
  shader->print();
  return shader;
}

bool CShader::Compile() {
  const char* text = m_Text.c_str();
  glCheck(glShaderSource(m_Shader, 1, &text, nullptr));
  glCheck(glCompileShader(m_Shader));
  //glCompileShaderIncludeARB(m_Shader, )
  return m_Status.get(GL_COMPILE_STATUS);
}

bool CShader::Bind() {
  return true;
}

bool CShader::Empty()
{
  return m_Empty;
}

void CShader::print() {
  //cout << m_Text << endl;
}

const char* CShader::typeToStr()
{
  switch (m_Type) {
  case E_VERTEX:
    return "vertex";
  case E_FRAGMENT:
    return "fragment";
  case E_GEOMETRY:
    return "geometry";
  case E_COMPUTE:
    return "compute";
  default:
    return "unknown";
  }
}

const char* CShader::getName() {
  return m_Path.c_str();
}

GLuint CShader::get() {
  return m_Shader;
}

IShader::type CShader::GetType()
{
  return m_Type;
}

void CShader::AddRef()
{
  m_Refs++;
}

int CShader::Release()
{
  m_Refs--;
  if (m_Refs == 0)
  {
    delete this;
  }
  return m_Refs;
}


CBaseShaderProgram::CBaseShaderProgram() : m_Status(this)
{
  m_Program = glCreateProgram();
  created = true;
  buffer = new char[BUFFER_SIZE];
}

CBaseShaderProgram::CBaseShaderProgram(
  ShaderRef vs, ShaderRef fs)
  :
  CBaseShaderProgram()
{
  m_Vertex.shader = vs;
  m_Fragment.shader = fs;
}

CBaseShaderProgram::CBaseShaderProgram(ShaderInfo& vs, ShaderInfo& fs)
  :
  CBaseShaderProgram()
{
  m_Vertex = vs;
  m_Fragment = fs;
}

CBaseShaderProgram::CBaseShaderProgram(ShaderInfo& vs, ShaderInfo& fs, ShaderInfo& gs)
  :
  CBaseShaderProgram()
{
  m_Vertex = vs;
  m_Fragment = fs;
  m_Geometry = gs;
}

CBaseShaderProgram::CBaseShaderProgram(ShaderInfo& vs, ShaderInfo& fs, ShaderInfo& gs, ShaderInfo& cs)
  :
  CBaseShaderProgram()
{
  m_Vertex = vs;
  m_Fragment = fs;
  m_Geometry = gs;
  m_Compute = cs;
}

CBaseShaderProgram::~CBaseShaderProgram() {
  glDeleteProgram(m_Program);
}

bool CBaseShaderProgram::Create(const char* label) {
  if (!created) {
    created = true;
    m_Program = glCreateProgram();
  }
  debuger::program_label(m_Program, label);
  Attach(m_Vertex);
  Attach(m_Fragment);
  Attach(m_Geometry);
  Attach(m_Compute);
  return Link();
}

void CBaseShaderProgram::Attach(ShaderInfo& info) {
  ShaderInfo& attached = info;

  if (!info.used) return;
  switch (info.shader->GetType()) {
  case CShader::type::E_VERTEX:
    attached = attachInternal(info, m_Vertex);
    break;
  case CShader::type::E_FRAGMENT:
    attached = attachInternal(info, m_Fragment);
    break;
  case CShader::type::E_GEOMETRY:
    attached = attachInternal(info, m_Geometry);
    break;
  case CShader::type::E_COMPUTE:
    attached = attachInternal(info, m_Compute);
    break;
  }
  attached.attached = true;
  glCheck(glAttachShader(m_Program, info.shader->get()));
}

CBaseShaderProgram::ShaderInfo& CBaseShaderProgram::attachInternal(ShaderInfo& src, ShaderInfo& dst)
{
  if (dst.shader == nullptr)
    dst.shader = src.shader;
  if (dst.attached)
    Detach(src);
  return dst;
}

void CBaseShaderProgram::Detach(ShaderInfo& info)
{
  if (info.attached)
    glCheck(glDetachShader(m_Program, info.shader->get()));
}

bool CBaseShaderProgram::Dispatch(int x, int y, int z, GLbitfield barriers)
{
  if (m_Compute.attached)
  {
    glCheck(glDispatchCompute(x, y, z));
    if (barriers) glCheck(glMemoryBarrier(barriers));
    return true;
  }
  return false;
}

bool CBaseShaderProgram::DispatchInderect()
{
  return false;
}

bool CBaseShaderProgram::Link() {
  glCheck(glLinkProgram(m_Program));
  return m_Status.get(GL_LINK_STATUS);
}

void CBaseShaderProgram::Use() {
  glCheck(glUseProgram(m_Program));
}

void CBaseShaderProgram::Unuse()
{
  glCheck(glUseProgram(0));
}

void CBaseShaderProgram::DeleteProgram()
{
  glDeleteProgram(m_Program);
  created = false;
}

GLint CBaseShaderProgram::GetUniformLocation(const char* format, ...)
{
  va_list ptr;
  va_start(ptr, format);
  auto name = buildName(format, ptr);
  va_end(ptr);

  GLint loc = -1;
  if (use_cache->GetIVal())
  {
    auto it = m_Cache.find(name);
    if (it != m_Cache.end())
      loc = it->second;
    else
    {
      loc = glGetUniformLocation(m_Program, name);
      m_Cache[name] = loc;
    }
  }
  else
  {
    loc = glGetUniformLocation(m_Program, name);
    m_Cache[name] = loc;
  }
  return loc;
}

GLint CBaseShaderProgram::GetUniformLocation(std::string& name)
{
  return GetUniformLocation(name.c_str());
}

UniformValue CBaseShaderProgram::GetUniformValue(const char* name)
{
  UniformValue result;
  auto location = GetUniformLocation(name);

  result.location = location;
  result.program = m_Program;
  return result;
}

void CBaseShaderProgram::Uniform(int value, const char* format, ...)
{
  va_list ptr;
  va_start(ptr, format);
  auto name = buildName(format, ptr);
  va_end(ptr);

  GLint loc = GetUniformLocation(name);
  if (loc != -1) {
    glCheck(glUniform1i(loc, value));
  }
}

void CBaseShaderProgram::Uniform(unsigned int value, const char* format, ...)
{
  va_list ptr;
  va_start(ptr, format);
  auto name = buildName(format, ptr);
  va_end(ptr);

  GLint loc = GetUniformLocation(name);
  if (loc != -1) {
    glCheck(glUniform1ui(loc, value));
  }
}

void CBaseShaderProgram::Uniform(float value, const char* format, ...)
{
  va_list ptr;
  va_start(ptr, format);
  auto name = buildName(format, ptr);
  va_end(ptr);

  GLint loc = GetUniformLocation(name);
  if (loc != -1) {
    glCheck(glUniform1f(loc, value));
  }
}

void CBaseShaderProgram::Uniform(Vec1 value, const char* format, ...)
{
  va_list ptr;
  va_start(ptr, format);
  auto name = buildName(format, ptr);
  va_end(ptr);

  GLint loc = GetUniformLocation(name);
  if (loc != -1) {
    glCheck(glUniform1f(loc, value[0]));
  }
}

void CBaseShaderProgram::Uniform(Vec2 value, const char* format, ...)
{
  va_list ptr;
  va_start(ptr, format);
  auto name = buildName(format, ptr);
  va_end(ptr);

  GLint loc = GetUniformLocation(name);
  if (loc != -1) {
    glCheck(glUniform2fv(loc, 1, glm::value_ptr(value)));
  }
}

void CBaseShaderProgram::Uniform(Vec3 value, const char* format, ...)
{
  va_list ptr;
  va_start(ptr, format);
  auto name = buildName(format, ptr);
  va_end(ptr);

  GLint loc = GetUniformLocation(name);
  if (loc != -1) {
    glCheck(glUniform3fv(loc, 1, glm::value_ptr(value)));
  }
}

void CBaseShaderProgram::Uniform(Vec4 value, const char* format, ...)
{
  va_list ptr;
  va_start(ptr, format);
  auto name = buildName(format, ptr);
  va_end(ptr);

  GLint loc = GetUniformLocation(name);
  if (loc != -1) {
    glCheck(glUniform4fv(loc, 1, glm::value_ptr(value)));
  }
}

void CBaseShaderProgram::Uniform(glm::ivec4 value, const char* format, ...)
{
  va_list ptr;
  va_start(ptr, format);
  auto name = buildName(format, ptr);
  va_end(ptr);

  GLint loc = GetUniformLocation(name);
  if (loc != -1) {
    glCheck(glUniform4iv(loc, 1, glm::value_ptr(value)));
  }
}

void CBaseShaderProgram::Uniform(Mat2 value, const char* format, ...)
{
  va_list ptr;
  va_start(ptr, format);
  auto name = buildName(format, ptr);
  va_end(ptr);

  GLint loc = GetUniformLocation(name);
  if (loc != -1) {
    glCheck(glUniformMatrix2fv(loc, 1, GL_FALSE, glm::value_ptr(value)));
  }
}

void CBaseShaderProgram::Uniform(Mat3 value, const char* format, ...)
{
  va_list ptr;
  va_start(ptr, format);
  auto name = buildName(format, ptr);
  va_end(ptr);

  GLint loc = GetUniformLocation(name);
  if (loc != -1) {
    glCheck(glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(value)));
  }
}

void CBaseShaderProgram::Uniform(Mat4 value, const char* format, ...)
{
  va_list ptr;
  va_start(ptr, format);
  auto name = buildName(format, ptr);
  va_end(ptr);

  GLint loc = GetUniformLocation(name);
  if (loc != -1) {
    glCheck(glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value)));
  }
}

void CBaseShaderProgram::Reload(ShaderRef v, ShaderRef f, ShaderRef g, ShaderRef c, const char* label)
{
  Detach(m_Vertex);
  //reset(m_Vertex);
  Detach(m_Fragment);
  //reset(m_Fragment);
  Detach(m_Geometry);
  //reset(m_Geometry);
  Detach(m_Compute);
  //reset(m_Compute);
  DeleteProgram();

  m_Vertex.shader = v;
  m_Fragment.shader = f;
  if (!g)
    m_Geometry.shader = g;
  if (!c)
    m_Compute.shader = c;
  Create(label);
}

void CBaseShaderProgram::BindTexture2D(GLuint texture, GLint unit, const char* sampler)
{
  BindTextureUnit2D(texture, unit);
  Uniform(unit, sampler);
}

void CBaseShaderProgram::BindTextureUnit2D(GLuint texture, GLint unit)
{
  gl::ActiveTexture(GL_TEXTURE0 + unit);
  gl::BindTexture2D(texture);
}

GLuint CBaseShaderProgram::Get() {
  return m_Program;
}

void CBaseShaderProgram::Dump()
{
  int len = 0;
  GLenum bFormat;
  glGetProgramiv(m_Program, GL_PROGRAM_BINARY_LENGTH, &len);

  std::unique_ptr<uint8_t> binary(new uint8_t[len]);
  glGetProgramBinary(m_Program, len, &len, &bFormat, binary.get());

  FILE* shader = fopen("dump.bin", "wb");
  if (!shader)
  {
    //GetISystem()->Log("Cannot open file for dump");
    return;
  }

  fwrite(binary.get(), 1, len, shader);
  fclose(shader);
}

void CBaseShaderProgram::reset(ShaderInfo src)
{
  if (src.shader != nullptr)
    //src.shader.reset();
    src.shader = nullptr;
}

const char* CBaseShaderProgram::buildName(const char* format, va_list args)
{
  vsprintf(buffer, format, args);
  return buffer;
}

void CBaseShaderProgram::AddRef()
{
  m_Refs++;
}

int CBaseShaderProgram::Release()
{
  m_Refs--;
  if (m_Refs == 0)
  {
    delete this;
  }
  return m_Refs;
}
