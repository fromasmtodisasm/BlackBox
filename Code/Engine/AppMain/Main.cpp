#ifdef _WIN32
#	include <windows.h>
extern int main(int arg, char** argv);

int
	WINAPI
	WinMain(
		_In_ HINSTANCE	   hInstance,
		_In_opt_ HINSTANCE hPrevInstance,
		_In_ LPSTR		   lpCmdLine,
		_In_ int		   nShowCmd)
{
	int			 argc;
	int			 retVal;
	char**		 argv;
	unsigned int i;
	int			 j;

	// parse a few of the command line arguments
	// a space delimites an argument except when it is inside a quote

	argc	= 1;
	int pos = 0;
	for (i = 0; i < strlen(lpCmdLine); i++)
	{
		while (lpCmdLine[i] == ' ' && i < strlen(lpCmdLine))
		{
			i++;
		}
		if (lpCmdLine[i] == '\"')
		{
			i++;
			while (lpCmdLine[i] != '\"' && i < strlen(lpCmdLine))
			{
				i++;
				pos++;
			}
			argc++;
			pos = 0;
		}
		else
		{
			while (lpCmdLine[i] != ' ' && i < strlen(lpCmdLine))
			{
				i++;
				pos++;
			}
			argc++;
			pos = 0;
		}
	}

	argv = (char**)malloc(sizeof(char*) * (argc + 1));

	argv[0] = (char*)malloc(1024);
	::GetModuleFileName(0, argv[0], 1024);

	for (j = 1; j < argc; j++)
	{
		argv[j] = (char*)malloc(strlen(lpCmdLine) + 10);
	}
	argv[argc] = 0;

	argc = 1;
	pos	 = 0;
	for (i = 0; i < strlen(lpCmdLine); i++)
	{
		while (lpCmdLine[i] == ' ' && i < strlen(lpCmdLine))
		{
			i++;
		}
		if (lpCmdLine[i] == '\"')
		{
			i++;
			while (lpCmdLine[i] != '\"' && i < strlen(lpCmdLine))
			{
				argv[argc][pos] = lpCmdLine[i];
				i++;
				pos++;
			}
			argv[argc][pos] = '\0';
			argc++;
			pos = 0;
		}
		else
		{
			while (lpCmdLine[i] != ' ' && i < strlen(lpCmdLine))
			{
				argv[argc][pos] = lpCmdLine[i];
				i++;
				pos++;
			}
			argv[argc][pos] = '\0';
			argc++;
			pos = 0;
		}
	}
	argv[argc] = 0;

	// Initialize the processes and start the application.
	retVal = main(argc, argv);

	// Delete arguments
	for (j = 0; j < argc; j++)
	{
		free(argv[j]);
	}
	free(argv);

	return retVal;
}
#endif
