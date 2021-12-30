// Copyright 2001-2019 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once
struct QPixmap
{
};
struct Version;
class QPainter;

using QString = string;

struct QSplashScreen
{
	virtual void drawContents(QPainter* painter)  = 0;
};

class SplashScreen : public QSplashScreen
{
public:
	SplashScreen(const QPixmap& pixmap = QPixmap());
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
