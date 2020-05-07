#include <BlackBox/Renderer/BaseShader.hpp>

#include <sstream>

using namespace std;


namespace
{
	bool match(std::string_view input, std::string_view with, size_t& offset)
	{
		if ((offset + with.size()) > input.size())
			return false;
		if (input.substr(offset, with.size()) == with)
		{
			offset += with.size();
			return true;
		}
		return false;
	}

	void skip_spaces(string_view buffer, size_t& begin)
	{
		size_t pos = begin;
		if ((pos = buffer.find_first_of(begin, ' ')) != buffer.npos)
		{
			begin = buffer.find_first_not_of(begin, ' ');
		}
	}

	std::string_view get_attr_name(string_view buffer, string_view attr, size_t begin, size_t end)
	{
		if (match(buffer, attr, begin))
		{
			//begin += attr.length();
			if (match(buffer, ",", begin))
			{
				if ((begin = buffer.find_first_of(' ', begin - 1)) != buffer.npos)
				{
					begin++;
					if ((end = buffer.find_first_of(')', begin)) != buffer.npos)
					{
						string_view attr_name(buffer.substr(begin, end - begin));
						return attr_name;
					}
				}
			}
		}
		return string_view();
	}
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
    ///shader->print();
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

    if (!loadInternal(std::string("res/shaders/") + file, buff)) return false;
    buffer.clear();
    buffer += buff;
  }
	else if ((pos = buffer.find("#pragma")) != buffer.npos)
	{
    size_t begin = pos, end = buffer.size();
		if ((begin = buffer.find_first_of(' ')) != buffer.npos)
		{
			begin++;
			skip_spaces(buffer, begin);
			if (buffer.substr(begin, std::strlen("attribute")) == "attribute")
			{
				begin += sizeof("attribute") - 1;
				if (buffer[begin] == '(')
				{
					begin++;
					stringstream ss;
					bool status = false;
					if (auto name = get_attr_name(buffer, "position", begin, end); name.size() != 0)
					{
						ss << "POSITION_ATTR(" << name << ");";
						buffer = ss.str();
						status = true;
					}
					else if (auto name = get_attr_name(buffer, "tc", begin, end); name.size() != 0)
					{
						ss << "TC_ATTR(" << name << ");";
						buffer = ss.str();
						status = true;
					}
					else if (auto name = get_attr_name(buffer, "normal", begin, end); name.size() != 0)
					{
						ss << "NORMAL_ATTR(" << name << ");";
						buffer = ss.str();
						status = true;
					}
					else if (auto name = get_attr_name(buffer, "color", begin, end); name.size() != 0)
					{
						ss << "COLOR_ATTR(" << name << ");";
						buffer = ss.str();
						status = true;
					}
					return status;
				}
			}
		}
		return false;
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
  auto shader = ShaderRef(new CShader(text, type));
  if (!shader->Create())
    return nullptr;
  shader->Compile();
  shader->print();
  return shader;
}
