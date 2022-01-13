#include <BlackBox/Core/Platform/Platform.hpp>
//////////////////////////////////////////////////////////////////////////
#include "EditorApp.h"
//////////////////////////////////////////////////////////////////////////
#include <QWidget>
#include <QStyleFactory>
#include <QFile>
#include <QDir>
#include <QMainWindow>
#include <QLabel>
#include <QSettings>
#include <QFileSystemWatcher>
#include <QMenuBar>
#include <QProxyStyle>
//////////////////////////////////////////////////////////////////////////

#include <QApplication>
#include <QFrame>
#include <QStyleFactory>

#include "Qt/EditorMainFrame.h"
#include "SplashScreen.h"

#include "EditorImpl.h"
#include <BlackBox/Core/Path.hpp>

extern CEditApp theApp;

QWidget* game_window;
HWND	 main_hwnd() { return (HWND)game_window->winId(); }
QFileSystemWatcher* watcher;

static void QtLogToDebug(QtMsgType Type, const QMessageLogContext& Context, const QString& message)
{
	OutputDebugStringW(L"Qt: ");
	OutputDebugStringW(reinterpret_cast<const wchar_t*>(message.utf16()));
	OutputDebugStringW(L"\n");
}

void OnFileChanged(const QString& path)
{
	QByteArray ba = path.toLocal8Bit();
	CryLog("File [%s] changed", ba.data());
}
void OnDirectoryChanged(const QString& path)
{
	QByteArray ba = path.toLocal8Bit();
	CryLog("Directory [%s] changed", ba.data());
	QStringList fileList = watcher->directories();
	Q_FOREACH (QString file, fileList)
		qDebug() << "file name" << file << "\n";
}

void SetStyle()
{
	qApp->setStyle(QStyleFactory::create("Fusion"));
	QPalette darkPalette;
	QColor	 darkColor	   = QColor(45, 45, 45);
	QColor	 disabledColor = QColor(127, 127, 127);
	darkPalette.setColor(QPalette::Window, darkColor);
	darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(35, 35, 35));
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

    darkPalette.setColor(QPalette::Highlight, QColor(42, 200, 218));
	darkPalette.setColor(QPalette::HighlightedText, Qt::black);
	darkPalette.setColor(QPalette::Disabled, QPalette::HighlightedText, disabledColor);

	qApp->setPalette(darkPalette);

	qApp->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");
    qApp->setStyleSheet("QMenuBar::separator { background-color: #ffffff; width: 1px; height: 1px; }");
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

	QPixmap pixmap(":/splash.png");
	SplashScreen splash(pixmap);
	splash.show();
	qApp->processEvents();

	qInstallMessageHandler(QtLogToDebug);

	CEditorMainFrame* mainFrame = new CEditorMainFrame(nullptr);

	watcher = new QFileSystemWatcher();
	if (!watcher->addPath(R"(Data/scripts/)"))
	{
		delete watcher;
		watcher = nullptr;
		CryWarning(VALIDATOR_MODULE_EDITOR, VALIDATOR_ERROR, "Failed to add qss stylesheet to watcher");
	}
	else
	{
		QObject::connect(watcher, &QFileSystemWatcher::fileChanged, OnFileChanged);
		QObject::connect(watcher, &QFileSystemWatcher::directoryChanged, OnDirectoryChanged);
	}

	if (!theApp.InitInstance())
	{
		theApp.ExitInstance();
		return -1;
	}

	mainFrame->PostLoad();
	splash.close();

    theApp.PostInit();
	result = a.exec();

	theApp.ExitInstance();

    delete watcher;
	delete mainFrame;

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
