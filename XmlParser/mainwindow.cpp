#include "ui_mainwindow.h"
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    selectionItem = new SelectionItem(0, 0, 0, 0);
    graphicsView = new GraphicsView(ui->groupBox, selectionItem);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type())
    {
        case QEvent::LanguageChange:
          ui->retranslateUi(this);
          break;
        default:
          break;
    }
}

void MainWindow::on_actionOpen_triggered()
{
    QPixmap pixmap(QFileDialog::getOpenFileName(this));
    graphicsView->scene()->addPixmap(pixmap);
}
