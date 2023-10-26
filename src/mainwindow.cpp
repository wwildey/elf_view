#include "../includes/mainwindow.h"
#include "../gui/ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pMyMenuBar = ui->menubar;
    pInfoDisplay = ui->textEdit;
    pTabWidget = ui->tabWidget;

    // example of adding text to QTextEdit
    pInfoDisplay->setText("This is a test");
    pInfoDisplay->append("This is also a test");

    // Create actions
    createActions();

    // Create menu
    createMenu();

    // Make data for test
    // **********************************
    // Should a SectionlistView class be created?
    pSectionlistView = ui->SectionlistView;
    pSectionListModel = new QStringListModel(this);
    QStringList List;
    List << "Item 1" << "Item2" << "Item 3";
    pSectionListModel->setStringList(List);
    pSectionlistView->setModel(pSectionListModel);
}

MainWindow::~MainWindow()
{
    delete ui;
    // TODO: Cleanup everything here
    qDebug("Exiting the application from ~MainWindow\n");

}

void MainWindow::createActions(void)
{
    pOpenAction = new QAction(tr("&Open"), this);
    pOpenAction->setShortcuts(QKeySequence::Open);
    pOpenAction->setStatusTip(tr("Open a file"));
    connect(pOpenAction, &QAction::triggered, this, &MainWindow::openFile);
    pQuitAction = new QAction(tr("&Quit"), this);
    pQuitAction->setShortcuts(QKeySequence::Quit);
    pQuitAction->setStatusTip(tr("Exit the application"));
    connect(pQuitAction, &QAction::triggered, this, &MainWindow::quitApp);

    pAboutAction = new QAction(tr("About"), this);
    pAboutAction->setStatusTip(tr("Display About dialog"));
    connect(pAboutAction, &QAction::triggered, this, &MainWindow::showAboutDialog);


//    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
//    connect(aboutQtAct, &QAction::triggered, qApp, &QApplication::aboutQt);
//    connect(aboutQtAct, &QAction::triggered, this, &MainWindow::aboutQt);

}


void MainWindow::createMenu(void)
{
    pFileMenu = pMyMenuBar->addMenu(tr("&File"));
    pFileMenu->addAction(pOpenAction);
    pFileMenu->addSeparator();
    pFileMenu->addAction(pQuitAction);

    pHelpMenu = pMyMenuBar->addMenu(tr("&Help"));
    pHelpMenu->addAction(pAboutAction);

}

//********************************
// Actions
//********************************
void MainWindow::openFile(void)
{
    qDebug("Open a file\n");
}

void MainWindow::quitApp(void)
{
    qDebug("Exiting the application\n");
    QApplication::quit();
}

void MainWindow::showAboutDialog()
{
    qDebug("Display about dialog\n");
}
