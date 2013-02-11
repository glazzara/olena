#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView->setGeometry(0, 0, ui->groupBox->width(), ui->groupBox->height());
    Scene *scene = new Scene(ui->graphicsView);
    scene->setSceneRect(ui->graphicsView->geometry());
    ui->graphicsView->setScene(scene);
    Xml::parseItems("/lrde/home/stage/froger_a/olena/_build/scribo/src/out.xml", scene);
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
    QBrush brush(pixmap);
    ui->graphicsView->scene()->setBackgroundBrush(brush);
    ui->graphicsView->scene()->setSceneRect(pixmap.rect());;
}

/*void MainWindow::on_action(QString& string)
{
    ui->textEdit->setText(string);
}*/
