#include "gamewindow.h"
#include "./ui_gamewindow.h"

#include <QTimer>
#include <EditorApp.h>

extern QWidget* game_window;

GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GameWindow)
{
    ui->setupUi(this);


    QFrame * frame = nullptr;
    frame = (QFrame *)findChild<QWidget*>("MainFrame");
    game_window = frame;

	QTimer::singleShot(0, this, &GameWindow::OnIdleCallback);
}

GameWindow::~GameWindow()
{
    delete ui;
}

void GameWindow::OnIdleCallback()
{
	//qDebug() << "Idle processing";
    auto res = CEditApp::GetInstance()->IdleProcessing(false);
    (void)res;

	QTimer::singleShot(0, this, &GameWindow::OnIdleCallback);
}

void GameWindow::on_checkBox_stateChanged(int arg1)
{
    gEnv->pConsole->ShowConsole(arg1);
}

void GameWindow::on_action_Quit_triggered()
{
    qApp->quit();
}

