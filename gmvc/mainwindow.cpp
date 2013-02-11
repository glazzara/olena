#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QListView *view = new QListView();
    view->setViewMode(QListView::IconMode);
    view->setIconSize(QSize(100, 100));
    view->setMovement(QListView::Static);
    view->setFlow(QListView::TopToBottom);
    //view->setGridSize(QSize(300, 100));
    view->setSpacing(9);

    QList<QPixmap> pixmaps;
    QStringList paths;
    paths << "/lrde/home/stage/froger_a/ppm/mp00032c.ppm" << "/lrde/home/stage/froger_a/ppm/mp00042.ppm" << "/lrde/home/stage/froger_a/ppm/mp00076.ppm";
    QString path;
    Model *model = new Model(this);
    foreach(path, paths)
    {
        QPixmap pixmap(path);
        model->addPixmap(pixmap);
    }

    view->setModel(model);
    //setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

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
