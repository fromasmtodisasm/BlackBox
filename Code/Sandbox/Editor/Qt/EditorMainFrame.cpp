#include "EditorMainFrame.h"
#include "./ui_editormainframe.h"

#include <QTimer>
#include <EditorApp.h>

#include <QFileDialog>

extern QWidget* game_window;

CEditorMainFrame::CEditorMainFrame(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CEditorMainFrame)
{
    ui->setupUi(this);


    QFrame * frame = nullptr;
    frame = (QFrame *)findChild<QWidget*>("MainFrame");
    game_window = frame;

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

