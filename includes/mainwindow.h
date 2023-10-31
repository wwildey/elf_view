#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListView>
#include <QStringListModel>
#include <QTabWidget>
#include <QTextEdit>
#include <QMenuBar>
#include <QMenu>
#include <QFileDialog>
#include <QFileInfo>

#include "elffile.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:   
    void createActions(void);
    void createMenu(void);

    ELFFILE *pElfFile;

    Ui::MainWindow *ui;

    QMenu *pFileMenu;
    QMenu *pHelpMenu;

    QAction *pOpenAction;
    QAction *pQuitAction;

    QAction *pAboutAction;

    QStringListModel *pSectionListModel;
    QListView *pSectionlistView;
    QMenuBar  *pMyMenuBar;
    QTextEdit *pInfoDisplay;
    QTabWidget *pTabWidget;

public slots:
    void openFile();
    void quitApp();
    void showAboutDialog();

};
#endif // MAINWINDOW_H
