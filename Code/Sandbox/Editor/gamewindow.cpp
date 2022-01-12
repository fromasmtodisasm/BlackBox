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
    frame = (QFrame *)findChild<QWidget*>("frame");
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
	QTimer::singleShot(0, this, &GameWindow::OnIdleCallback);
}


void GameWindow::on_actionQuit_triggered()
{
    qApp->quit();
}


void GameWindow::on_checkBox_stateChanged(int arg1)
{
    gEnv->pConsole->ShowConsole(arg1);
}

