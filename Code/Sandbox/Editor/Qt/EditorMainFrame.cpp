#include "EditorMainFrame.h"
#include "./ui_editormainframe.h"

#include <QTimer>
#include <EditorApp.h>

#include <QFileDialog>
#include <QRegularExpression>
#include "Version.h"
#include <EditorImpl.h>

#include <BlackBox/System/IProjectManager.hpp>

extern QWidget* game_window;

CEditorMainFrame::CEditorMainFrame(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CEditorMainFrame)
{
    ui->setupUi(this);
	setEnabled(true);
	setFocusPolicy(Qt::StrongFocus);
	setFocus();


    QFrame * frame = nullptr;
    frame = (QFrame *)findChild<QWidget*>("MainFrame");
    game_window = frame;

    ui->toolBar->setStyleSheet("QToolBar::separator { background-color: #ffffff; width: 1px; height: 1px; }");
    //ui->toolBar->setStyleSheet("QMenuBar::separator { background-color: #454545; }");
    //ui->toolBar->setStyleSheet("QMenuBar { background-color: #454545; }");

	//UpdateWindowTitle();

	setWindowIcon(QIcon("icons:editor_icon.ico"));
	qApp->setWindowIcon(windowIcon());


	QTimer::singleShot(0, this, &CEditorMainFrame::OnIdleCallback);
}

CEditorMainFrame::~CEditorMainFrame()
{
    delete ui;
}

void CEditorMainFrame::PostLoad()
{
	ui->listView->PostLoad();
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

    QTimer::singleShot(8, this, &CEditorMainFrame::OnIdleCallback);
}

void CEditorMainFrame::on_action_Quit_triggered()
{
    qApp->quit();
}

void CEditorMainFrame::on_action_Open_File_triggered()
{
    QString str = QFileDialog::getOpenFileName(0, "Open Dialog", "", "*.*");
}


void CEditorMainFrame::on_action_Toggle_Console_triggered()
{

}

void CEditorMainFrame::on_action_Toggle_Console_toggled(bool arg1)
{
    gEnv->pConsole->ShowConsole(arg1);
}


void CEditorMainFrame::on_action_Clear_Console_triggered()
{
   gEnv->pConsole->Clear();
}

