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
    // Cache the background for repaint optimization.
    ui->graphicsView->setCacheMode(QGraphicsView::CacheBackground);
    scene = new Scene(ui->graphicsView);
    scene->setSceneRect(ui->graphicsView->geometry());
    ui->graphicsView->setScene(scene);
    Xml::parseItems("/tmp/mp00082c_gui.xml", scene);
    ToolBar *toolBar = new ToolBar(this);
    toolBar->setGeometry(0, ui->menuBar->height(), ui->menuBar->width(), 30);
    connect(toolBar->openAction(), SIGNAL(triggered()), this, SLOT(on_actionOpen_triggered()));
    connect(toolBar->quitAction(), SIGNAL(triggered()), this, SLOT(close()));
    connect(toolBar->segmentAction(), SIGNAL(triggered()), this, SLOT(onBeginSegmentation()));
    progressDialog = new ProgressDialog(this);
    connect(&runner, SIGNAL(finished()), progressDialog, SLOT(close()));
    connect(&runner, SIGNAL(new_progress_max_value(int)), progressDialog, SLOT(setMaximum(int)));
    connect(&runner, SIGNAL(new_progress_label(QString)), progressDialog, SLOT(setLabelText(QString)));
    connect(&runner, SIGNAL(xml_saved(QString)), this, SLOT(onXmlSaved(QString)));
    connect(&runner, SIGNAL(progress()), progressDialog, SLOT(run()));
}

MainWindow::~MainWindow()
{
    delete scene;
    delete progressDialog;
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
        scene->removeItems();
        QPixmap pixmap(imagePath);
        scene->setSceneRect(pixmap.rect());
        scene->setBackgroundBrush(QBrush(pixmap));
    }
}

void MainWindow::onBeginSegmentation()
{
    if(imagePath != "")
    {
        progressDialog->reset();
        runner.start_demat(imagePath);
    }
}

void MainWindow::onXmlSaved(const QString& filename)
{
    Xml::parseItems(filename, scene);
}

/*void MainWindow::on_action(QString& string)
{
    ui->textEdit->setText(string);
}*/
