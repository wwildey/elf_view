#include "../includes/mainwindow.h"
#include "../gui/ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pSectionlistView = ui->SectionlistView;
    pSectionListModel = new QStringListModel(this);

    // Make data for test
    // **********************************
    // Should a SectionlistView class be created?
    QStringList List;
    List << "Item 1" << "Item2" << "Item 3";
    pSectionListModel->setStringList(List);
    pSectionlistView->setModel(pSectionListModel);
}

MainWindow::~MainWindow()
{
    delete ui;
}

