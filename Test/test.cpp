#include "test.h"
#include "ui_test.h"

Test::Test(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Test)
{
    ui->setupUi(this);
    createActions();
    createMenus();
}

Test::~Test()
{
    delete ui;
}

void Test::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void contextMenuEvent(QContextMenuEvent *event)
{
}

void Test::createActions()
{
    openAction = new QAction(tr("&new"), this);
    openAction->setShortcuts(QKeySequence::New);
    openAction->setStatusTip(tr("Create a new file"));
    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));
}

void Test::createMenus()
{
    openMenu = menuBar()->addMenu(tr("&File"));
    openMenu->addAction(openAction);
}
