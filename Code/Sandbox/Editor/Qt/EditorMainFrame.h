#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class CEditorMainFrame; }
QT_END_NAMESPACE


class CEditorMainFrame : public QMainWindow
{
    Q_OBJECT

public:
    CEditorMainFrame(QWidget *parent = nullptr);
    ~CEditorMainFrame();
	void PostLoad();

	void UpdateWindowTitle(const QString& levelPath = "");

	void OnIdleCallback();

    //void keyPressEvent(QKeyEvent* pEvent)
	//{
	//	CryLog("Key [%s] pressed", pEvent->text().toLocal8Bit());
    //}

    //void mousePressEvent(QMouseEvent* pEvent)
	//{
	//	CryLog("Mouse pressed");
    //}
	static CEditorMainFrame* instance() { return m_Instance; }


	static CEditorMainFrame* m_Instance;
	QWidget*				 GetDockContent();
  private slots:
    void on_action_Clear_Console_triggered();
    void on_action_Toggle_Console_toggled(bool arg1);
    void on_action_Toggle_Console_triggered();
    void on_action_Quit_triggered();
    void on_action_Open_File_triggered();

    //std::thread m_UpdateThread;

public:
    Ui::CEditorMainFrame *ui;
};
#endif // GAMEWINDOW_H
