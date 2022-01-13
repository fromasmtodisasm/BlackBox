#include "BasicConsoleWindow.h"

void BasicConsoleWindow::Print(const char* inszText)
{
	//items << inszText;
	//QString text;
	addItem(inszText);
	update();
}

