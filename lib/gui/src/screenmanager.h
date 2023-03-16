#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include <QMainWindow>
#include <QMap>

class IScreenLoader;
typedef QMap<QString, IScreenLoader *> TScreens;

class CL2_EXPORT ScreenManager : public QMainWindow
{
    Q_OBJECT
	TScreens _screens;
	IScreenLoader *_curScr;
public:
    ScreenManager();
	void addScreen( QString scrName, IScreenLoader *scrLoader );

public slots:
    void openScreen( QString );
    void openModalDialog( QString );
//    void about();
//    void aboutQt();

};

#endif
