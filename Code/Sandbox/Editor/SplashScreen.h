// Copyright 2001-2019 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once
#include <QPixmap>
#include <QSplashScreen>

struct Version;

class SplashScreen : public QSplashScreen
{
public:
	SplashScreen(const QPixmap& pixmap = QPixmap(), Qt::WindowFlags f = Qt::Widget);
	~SplashScreen();

	static SplashScreen* GetSplashScreen() { return s_pLogoWindow; }

	static void          SetVersion(const Version& v);
	static void          SetText(const char* szText);

private:
	virtual void drawContents(QPainter* painter) override;
	void         SetInfo(const char* text);

	QString              version;
	QString              m_text;

	static SplashScreen* s_pLogoWindow;
};
