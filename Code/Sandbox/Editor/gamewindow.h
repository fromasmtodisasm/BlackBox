#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class GameWindow; }
QT_END_NAMESPACE

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    GameWindow(QWidget *parent = nullptr);
    ~GameWindow();

	void OnIdleCallback();


private slots:
    void on_checkBox_stateChanged(int arg1);

private slots:
    void on_actionQuit_triggered();

private:
    Ui::GameWindow *ui;
};
#endif // GAMEWINDOW_H
