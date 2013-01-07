#include <QFileDialog>
#include <QTextEdit>
#include "test2.h"
#include "ui_test2.h"

Test2::Test2(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Test2)
{
    ui->setupUi(this);
    ui->label->setScaledContents(true);
    scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(ui->label);
    setCentralWidget(scrollArea);
}

Test2::~Test2()
{
    delete ui;
}

void Test2::changeEvent(QEvent *e)
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

void Test2::on_actionOpen_triggered()
{
    QString file = QFileDialog::getOpenFileName(this);
    ui->label->setPixmap(QPixmap(file));
    ui->textEdit->setText(file);
}
