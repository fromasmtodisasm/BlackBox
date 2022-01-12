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
    void on_action_Clear_Console_triggered();

private slots:
    void on_action_Toggle_Console_toggled(bool arg1);

private slots:
    void on_action_Toggle_Console_triggered();

private slots:
    void on_action_Quit_triggered();
    void on_action_Open_File_triggered();

private:
    Ui::GameWindow *ui;
};
#endif // GAMEWINDOW_H
