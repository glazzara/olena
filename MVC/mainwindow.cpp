#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QStringList list;
    list << "/lrde/home/stage/froger_a/ppm/mp00032c.ppm" << "/lrde/home/stage/froger_a/ppm/mp00042.ppm" << "/lrde/home/stage/froger_a/ppm/mp00076.ppm";

    QAbstractItemModel *model = new ListModel(list);

    ItemDelegate *delegate = new ItemDelegate();

    QListView *view = new QListView;
    view->setModel(model);
    view->setItemDelegate(delegate);
    setCentralWidget(view);
    view->show();
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
