#include <stack>

#include "Scanner.hpp"

std::stack<std::shared_ptr<std::ifstream>> includes;
bool Scanner::MakeInclude(const char* file_name)
{
	bool result = false;
	CryLog("include <%s>", file_name);
	//include_stack.push(IncludeData(YY_CURRENT_BUFFER, lineno(), driver.file.c_str()));
	auto includeFile = std::make_shared<std::ifstream>(std::ifstream((std::string("res/shaders/fx/") + file_name)));
	const char* buf	 = nullptr;
	{
		//driver.stream = std::ifstream(file_name);
		if (!includeFile->is_open())
		{
			gEnv->pLog->LogError("[FX] File %s not found", file_name);
			LexerError("failure in including a file");
			return result;
		}
		includes.emplace(includeFile);
		gEnv->pLog->Log("$3[FX] File %s opened", file_name);
		yypush_buffer_state(yy_create_buffer(includes.top().get(), YY_BUF_SIZE));
		//yyrestart(includeFile);
		result = true;
	}
	return result;
}

void Scanner::eof()
{
	yypop_buffer_state();
	gEnv->pLog->LogError("$3[FX] File %s ended", driver.file.c_str());
	//yypop_buffer_state();
	//driver.file = incData.file_name;
	//driver.location.initialize(&driver.file);

#if 0
	if(incData.fileToClose)
		fclose(incData.fileToClose);
#endif
	//include_stack.pop();
}
