#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class EditorMainFrame; }
QT_END_NAMESPACE

class CEditorMainFrame : public QMainWindow
{
    Q_OBJECT

public:
    CEditorMainFrame(QWidget *parent = nullptr);
    ~CEditorMainFrame();
	void PostLoad();

	void OnIdleCallback();

private slots:
    void on_action_Clear_Console_triggered();
    void on_action_Toggle_Console_toggled(bool arg1);
    void on_action_Toggle_Console_triggered();
    void on_action_Quit_triggered();
    void on_action_Open_File_triggered();

private:
    Ui::EditorMainFrame *ui;
};
#endif // GAMEWINDOW_H
