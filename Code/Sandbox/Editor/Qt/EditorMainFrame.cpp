#include "EditorMainFrame.h"
#include "./ui_editormainframe.h"

#include <QTimer>
#include <QWindow>
#include <QFrame>
#include <QSizePolicy>
#include <EditorApp.h>

#include <QFileDialog>
#include <QRegularExpression>
#include "Version.h"
#include <EditorImpl.h>

#include <BlackBox/System/IProjectManager.hpp>

extern QWidget* game_window;
CEditorMainFrame* CEditorMainFrame::m_Instance;

CEditorMainFrame::CEditorMainFrame(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CEditorMainFrame)
{
    ui->setupUi(this);
	setWindowState(Qt::WindowMaximized);
	setEnabled(true);
	setFocusPolicy(Qt::StrongFocus);
	setAttribute(Qt::WA_NativeWindow, true);
	setFocus();
	setMouseTracking(true);


	m_Instance = this;


    QFrame * frame = nullptr;
    frame = (QFrame *)findChild<QWidget*>("MainFrame");
    game_window = frame;

    ui->toolBar->setStyleSheet("QToolBar::separator { background-color: #ffffff; width: 1px; height: 1px; }");
    //ui->toolBar->setStyleSheet("QMenuBar::separator { background-color: #454545; }");
    //ui->toolBar->setStyleSheet("QMenuBar { background-color: #454545; }");

	//UpdateWindowTitle();

	setWindowIcon(QIcon("icons:editor_icon.ico"));
	qApp->setWindowIcon(windowIcon());
	//qApp->setOverrideCursor(QCursor(Qt::BlankCursor));


	QTimer::singleShot(16, this, &CEditorMainFrame::OnIdleCallback);
}

CEditorMainFrame::~CEditorMainFrame()
{
    delete ui;
}

void CEditorMainFrame::PostLoad()
{
	#if 0
	ui->listView->PostLoad();
	#endif
    this->show();
}

void CEditorMainFrame::UpdateWindowTitle(const QString& levelPath /*= "" */)
{
	const Version& v = GetIEditorImpl()->GetFileVersion();

	// Show active game project as "ProjectDir/GameDll".
	const QString game	= QString(gEnv->pSystem->GetIProjectManager()->GetCurrentProjectName());
	QString title = QString("CRYENGINE Sandbox - Build %2 - Project '%3'").arg(QString::number(v[0])).arg(game);

	if (!levelPath.isEmpty())
		title.prepend(levelPath.mid(levelPath.lastIndexOf(QRegularExpression("[/\\\\]")) + 1) + " - ");

	setWindowTitle(title);
}


void CEditorMainFrame::OnIdleCallback()
{
	//qDebug() << "Idle processing";
    auto res = CEditApp::GetInstance()->IdleProcessing(false);
    (void)res;

    QTimer::singleShot(16, this, &CEditorMainFrame::OnIdleCallback);
}

void CEditorMainFrame::on_action_Quit_triggered()
{
    qApp->quit();
}

void CEditorMainFrame::on_action_Open_File_triggered()
{
    QString str = QFileDialog::getOpenFileName(0, "Open Dialog", "", "*.*");
}

void CEditorMainFrame::on_gamewindow_created(WId hwnd)
{
	#if 1
	//auto wnd_proc_after = (WNDPROC)GetWindowLongPtr((HWND)startupParams.hWnd, GWLP_WNDPROC);
	#endif
	QWindow* window = QWindow::fromWinId((WId)hwnd);
	QWidget* widget = QWidget::createWindowContainer(window);

	auto parent = GetDockContent();
	widget->setFocusPolicy(Qt::StrongFocus);
	widget->setAttribute(Qt::WA_NativeWindow, true);
	widget->setParent(parent);
	widget->setWindowState(Qt::WindowMaximized);
	widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	ui->horizontalLayout->addWidget(widget);

	//RECT rcClip;	// new area for ClipCursor
	RECT rcOldClip; // previous area for ClipCursor
	auto pos	   = widget->pos();
	auto size	   = widget->size();
	RECT rcClip1{
		50, 100, 1800, 900};

	// Record the area in which the cursor can move.

	GetClipCursor(&rcOldClip);
	//GetWindowRect((HWND)hwnd, &rcClip1);

	// Get the dimensions of the application's window.

	//GetWindowRect((HWND)hwnd, &rcClip);
	//rcClip.left = rcClip1.left;
	//rcClip.right = rcClip1.right;

	// Confine the cursor to the application's window.

	ClipCursor(&rcClip1); 
	
}


void CEditorMainFrame::on_action_Toggle_Console_triggered()
{

}

QWidget* CEditorMainFrame::GetDockContent()
{
	return nullptr;
	//return ui->dockWidgetContents;
}

void CEditorMainFrame::on_action_Toggle_Console_toggled(bool arg1)
{
    gEnv->pConsole->ShowConsole(arg1);
}


void CEditorMainFrame::on_action_Clear_Console_triggered()
{
   gEnv->pConsole->Clear();
}

