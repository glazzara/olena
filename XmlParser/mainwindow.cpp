#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(ui->groupBox);
    layout->addWidget(ui->textEdit);
    setLayout(layout);
    QVBoxLayout *gBLayout = new QVBoxLayout;
    gBLayout->addWidget(ui->graphicsView);
    ui->groupBox->setLayout(gBLayout);
    Scene *scene = new Scene(ui->graphicsView);
    scene->setSceneRect(ui->graphicsView->geometry());
    ui->graphicsView->setScene(scene);
    Xml::parseItems("/tmp/mp00082c_gui.xml", scene);
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
    QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
    pixmapItem->setFlag(QGraphicsItem::ItemNegativeZStacksBehindParent, true);
    pixmapItem->setZValue(-1);
    pixmapItem->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    ui->graphicsView->scene()->setSceneRect(pixmap.rect());
    ui->graphicsView->scene()->addItem(pixmapItem);
}

/*void MainWindow::on_action(QString& string)
{
    ui->textEdit->setText(string);
}*/
