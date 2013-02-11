#include "ui_mainwindow.h"
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView->setGeometry(0, 0, ui->groupBox->width(), ui->groupBox->height());
    Scene *scene = new Scene(ui->graphicsView);
    scene->setSceneRect(ui->graphicsView->geometry());
    ui->graphicsView->setScene(scene);
    connect(scene, SIGNAL(sendString(QString&)), this, SLOT(on_action(QString&)));
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
    ui->graphicsView->scene()->addPixmap(pixmap);
}

void MainWindow::on_action(QString& string)
{
    ui->textEdit->setText(string);
}
