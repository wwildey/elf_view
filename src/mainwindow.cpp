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

//    pElfFile = new ELFFILE();
}

MainWindow::~MainWindow()
{
    delete ui;

    if ( pElfFile )
        delete pElfFile;
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
    QString fileName;

    qDebug("Open a file\n");
    fileName = QFileDialog::getOpenFileName(this,
        tr("Open File"), "", tr("Files (*)"));

    pInfoDisplay->append("Opening file: " + fileName);
    QFileInfo fileInfo(fileName);
    if ( fileInfo.isExecutable() ) {
        qDebug("fileName %s is an executable file\n", fileName.toStdString().c_str());
        pInfoDisplay->append("fileName: " + fileName + " is an executable file");
        pElfFile = new ELFFILE(&fileName);

        // send to ELFFILE
    }else {
        qDebug("fileName %s is not an executable file\n", fileName.toStdString().c_str());
        pInfoDisplay->append("fileName: " + fileName + " is not an executable file");
    }

    // Check if file is a binary file (executable)
    qDebug("fileName %s\n", fileName.toStdString().c_str());

    // TODO: Make a function that takes a qstring and calls pInfoDisplay->append
    pInfoDisplay->append("\nOpening file: " + fileName);
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
