#include "Scanner.hpp"

void Scanner::make_include(const char* file_name)
{
	//include_stack.push(IncludeData(YY_CURRENT_BUFFER, lineno(), driver.file.c_str()));
    const char *buf = NULL;
    {
        driver.stream.open(file_name);
        if (!driver.stream.is_open())
		{
			gEnv->pLog->LogError("[FX] File %s not found", file_name);
		}
		else
		{
			gEnv->pLog->Log("$3[FX] File %s opened", file_name);
            yypush_buffer_state( yy_create_buffer(  driver.stream,  YY_BUF_SIZE  )  );
		}


    }
    if(driver.stream.is_open())
    {
        // let's keep track of this newly opened file : when poping, we will have to close it
        //IncludeData &incData = include_stack.top();
        //incData.fileToClose = driver.stream;
		yyrestart(driver.stream);
    } else {
        LexerError( "failure in including a file" );
    }	
}

void Scanner::eof()
{
	gEnv->pLog->LogError("$3[FX] File %s ended", driver.file.c_str());
	yypop_buffer_state();
	//driver.file = incData.file_name;
	//driver.location.initialize(&driver.file);

#if 0
	if(incData.fileToClose)
		fclose(incData.fileToClose);
#endif
	include_stack.pop();
}
