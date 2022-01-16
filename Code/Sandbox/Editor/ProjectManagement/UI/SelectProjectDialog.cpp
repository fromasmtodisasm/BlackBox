#include "SelectProjectDialog.h"

#include <QTabWidget>
#include <QLayout>
#include <QPushButton>

CSelectProjectDialog::CSelectProjectDialog(QWidget * pParent, bool runOnSandboxInit, Tab tabToShow)
	//: CEditorDialog("SelectProjectDialog", pParent)
{
	setWindowTitle("Project Browser");
	setWindowIcon(QIcon("icons:editor_icon.ico"));

	QTabWidget* pTabs = new QTabWidget(this);

	pTabs->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	pTabs->setTabsClosable(false);

	QVBoxLayout* pMainLayout = new QVBoxLayout;
	pMainLayout->setContentsMargins(0, 0, 0, 0);
	pMainLayout->setSpacing(0);
	#if 0
	pMainLayout->setMargin(0);
	#endif
	QPushButton* pQuitBtn = new QPushButton(runOnSandboxInit ? "Quit" : "Cancel", this);
	connect(pQuitBtn, SIGNAL(clicked()), this, SLOT(reject()));
	pMainLayout->addWidget(pTabs);
	pMainLayout->addWidget(pQuitBtn);
	setLayout(pMainLayout);
}

CSelectProjectDialog::~CSelectProjectDialog()
{
}

CProjectManager& CSelectProjectDialog::GetProjectManager()
{
	return m_projectManager;
}

string CSelectProjectDialog::GetPathToProject() const
{
	return '\"' + m_pathToProject + '\"';
}
