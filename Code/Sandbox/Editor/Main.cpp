#include <BlackBox/Core/Platform/Platform.hpp>
//////////////////////////////////////////////////////////////////////////
#include "EditorApp.h"
//////////////////////////////////////////////////////////////////////////

#include <QApplication>
#include <QFrame>
#include <QStyleFactory>

extern CEditApp theApp;

QWidget* game_window;
HWND	 main_hwnd() { return (HWND)game_window->winId(); }

void SetStyle()
{
	qApp->setStyle(QStyleFactory::create("Fusion"));
	QPalette darkPalette;
	QColor	 darkColor	   = QColor(45, 45, 45);
	QColor	 disabledColor = QColor(127, 127, 127);
	darkPalette.setColor(QPalette::Window, darkColor);
	darkPalette.setColor(QPalette::WindowText, Qt::white);
	darkPalette.setColor(QPalette::Base, QColor(18, 18, 18));
	darkPalette.setColor(QPalette::AlternateBase, darkColor);
	darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
	darkPalette.setColor(QPalette::ToolTipText, Qt::white);
	darkPalette.setColor(QPalette::Text, Qt::white);
	darkPalette.setColor(QPalette::Disabled, QPalette::Text, disabledColor);
	darkPalette.setColor(QPalette::Button, darkColor);
	darkPalette.setColor(QPalette::ButtonText, Qt::white);
	darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, disabledColor);
	darkPalette.setColor(QPalette::BrightText, Qt::red);
	darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));

	darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
	darkPalette.setColor(QPalette::HighlightedText, Qt::black);
	darkPalette.setColor(QPalette::Disabled, QPalette::HighlightedText, disabledColor);

	qApp->setPalette(darkPalette);

	qApp->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");
}

int main(int argc, char* argv[])
{
	auto result = EXIT_FAILURE;
#if defined(DEBUG) | defined(_DEBUG) && 0
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtMemState s1, s2, s3;
	_CrtMemCheckpoint(&s1);
#endif
	QApplication a(argc, argv);
	SetStyle();

	if (theApp.InitInstance())
	{
		theApp.PostInit();
		//theApp.Run();
		result = a.exec();
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
