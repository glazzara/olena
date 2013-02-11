#include <QFileDialog>
#include <QScrollBar>
#include <QTextEdit>
#include <QShortcut>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QMouseEvent>
#include "test2.h"
#include "picture.h"
#include "ui_test2.h"

Test2::Test2(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Test2)
{
    ui->setupUi(this);
    scaleFactor = 1.0;
    QGraphicsScene *scene = new QGraphicsScene(ui->scrollArea);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setGeometry(0, 0, ui->scrollArea->width(), ui->scrollArea->height());
    shortcutZoomIn = new QShortcut(QKeySequence("Ctrl++"), this);
    shortcutZoomOut = new QShortcut(QKeySequence("Ctrl+-"), this);
    connect(shortcutZoomOut, SIGNAL(activated()), this, SLOT(on_actionZoomOut_triggered()));
    connect(shortcutZoomIn, SIGNAL(activated()), this, SLOT(on_actionZoomIn_triggered()));
    picture = new Picture(ui->textEdit);
    ui->graphicsView->scene()->addItem(picture);
}

Test2::~Test2()
{
    delete ui;
}

void Test2::changeEvent(QEvent *e)
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

void Test2::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value() + ((factor-1) * scrollBar->pageStep()/2)));
}

void Test2::scaleImage(double factor)
{
    scaleFactor *= factor;
    ui->textEdit->clear();
    ui->graphicsView->scale(factor, factor);
    adjustScrollBar(ui->scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(ui->scrollArea->verticalScrollBar(), factor);
}

void Test2::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this);
    ui->textEdit->setText(filename);
    picture->load(filename);
}

void Test2::on_actionZoomIn_triggered()
{
    scaleImage(1.25);
}

void Test2::on_actionZoomOut_triggered()
{
    scaleImage(0.75);
}
