#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    xml.load("/tmp/mp00082c_gui.xml");

    //xmlWidget.load(xml.treeItem());

    view = new QGraphicsView(new QGraphicsScene());

    QGraphicsItem *item;
    foreach(item, xml.graphicsItems())
        view->scene()->addItem(item);

    setCentralWidget(view);
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
