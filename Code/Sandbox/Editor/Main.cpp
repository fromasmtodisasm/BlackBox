#include <BlackBox/Core/Platform/Platform.hpp>
//////////////////////////////////////////////////////////////////////////
#include "EditorApp.h"
//////////////////////////////////////////////////////////////////////////
#include "gamewindow.h"

#include <QApplication>


extern CEditApp theApp;

QWidget* game_window;
HWND main_hwnd() { return (HWND)game_window->winId(); }


int main(int argc, char* argv[])
{
	if (argc > 1)
	{
		auto result = EXIT_FAILURE;
#if defined(DEBUG) | defined(_DEBUG) && 0
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		_CrtMemState s1, s2, s3;
		_CrtMemCheckpoint(&s1);
#endif
		if (theApp.InitInstance())
		{
			theApp.PostInit();
			theApp.Run();
			result = EXIT_SUCCESS;
		}
		theApp.ExitInstance();

#if defined(DEBUG) | defined(_DEBUG) && 0
		_CrtMemCheckpoint(&s2);
		if (_CrtMemDifference(&s3, &s1, &s2))
		{
			OutputDebugString("Memory Leak detected\n");
			_CrtMemDumpStatistics(&s3);
			_CrtDumpMemoryLeaks();
		}
#endif
		return result;
	}
    else
    {
        QApplication a(argc, argv);
        GameWindow	 w;
        w.show();
        return a.exec();
    }
}
