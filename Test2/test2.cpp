#include <QFileDialog>
#include <QScrollBar>
#include <QShortcut>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include "test2.h"
#include "picture.h"
#include "ui_test2.h"
#include "customgraphicsview.h"
#include "xml.h"

Test2::Test2(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Test2)
{
    ui->setupUi(this);
    scaleFactor = 1.0;
    graphicsView = new QGraphicsView();
    graphicsView->setParent(ui->scrollArea);
    QGraphicsScene *scene = new QGraphicsScene(ui->scrollArea);
    graphicsView->setScene(scene);
    graphicsView->setGeometry(0, 0, ui->scrollArea->width(), ui->scrollArea->height());
    picture = new Picture();
    graphicsView->scene()->addItem(picture);
    QVector<QGraphicsItem *> items = Xml::graphicsItems("/tmp/mp00082c_gui.xml", ui->textEdit);
    //for(int i = 0; i < items.count(); i++)
        //graphicsView->scene()->addItem(items[i]);
    selectionItem = new SelectionItem(0, 0, 0, 0);
    shortcutZoomIn = new QShortcut(QKeySequence("Ctrl++"), this);
    shortcutZoomOut = new QShortcut(QKeySequence("Ctrl+-"), this);
    connect(shortcutZoomOut, SIGNAL(activated()), this, SLOT(on_actionZoomOut_triggered()));
    connect(shortcutZoomIn, SIGNAL(activated()), this, SLOT(on_actionZoomIn_triggered()));
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
    graphicsView->scale(factor, factor);
    adjustScrollBar(ui->scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(ui->scrollArea->verticalScrollBar(), factor);
}

void Test2::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this);
    ui->textEdit->setText("FilePath : " + filename);
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
