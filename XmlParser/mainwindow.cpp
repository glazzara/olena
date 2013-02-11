#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    imagePath = QString::null;
    QVBoxLayout *gBLayout = new QVBoxLayout;
    gBLayout->addWidget(ui->graphicsView);
    ui->groupBox->setLayout(gBLayout);
    scene = new Scene(ui->graphicsView);
    scene->setSceneRect(ui->graphicsView->geometry());
    ui->graphicsView->setScene(scene);
    Xml::parseItems("/tmp/mp00082c_gui.xml", scene);
    ToolBar *toolBar = new ToolBar(this);
    toolBar->setGeometry(0, ui->menuBar->height(), ui->menuBar->width(), 30);
    connect(toolBar->openAction(), SIGNAL(triggered()), this, SLOT(on_actionOpen_triggered()));
    connect(toolBar->quitAction(), SIGNAL(triggered()), this, SLOT(close()));
    connect(toolBar->segmentAction(), SIGNAL(triggered()), this, SLOT(onSegmentation()));
    progressDialog = new ProgressDialog(this);
    connect(&runner, SIGNAL(finished(QString*)), this, SLOT(onEndSegmentation(QString*)));
    connect(&runner, SIGNAL(new_progress_max_value(int)), progressDialog, SLOT(setMaximum(int)));
    connect(&runner, SIGNAL(new_progress_label(QString)), progressDialog, SLOT(setLabelText(QString)));
    connect(&runner, SIGNAL(progress()), progressDialog, SLOT(run()));
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
    imagePath = QFileDialog::getOpenFileName(this);
    if(imagePath != "")
    {
        QPixmap pixmap(imagePath);
        QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
        pixmapItem->setFlag(QGraphicsItem::ItemNegativeZStacksBehindParent, true);
        pixmapItem->setZValue(-1);
        pixmapItem->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
        ui->graphicsView->scene()->setSceneRect(pixmap.rect());
        ui->graphicsView->scene()->addItem(pixmapItem);
    }
}

void MainWindow::onSegmentation()
{
    if(imagePath != "")
    {
        progressDialog->reset();
        runner.start_demat(imagePath);
    }
}

void MainWindow::onEndSegmentation(QString *filename)
{
    progressDialog->close();
    Xml::parseItems(*filename, scene);
}

/*void MainWindow::on_action(QString& string)
{
    ui->textEdit->setText(string);
}*/
