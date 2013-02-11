#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    /* Central window to adjust all widgets's size inside it. */
    QWidget *window = new QWidget();
    QHBoxLayout *mainLayout = new QHBoxLayout(window);
    mainLayout->setSpacing(8);
    setCentralWidget(window);
    //

    initGraphicsRegion(window, mainLayout);
    initTextRegion(window, mainLayout);
}

MainWindow::~MainWindow()
{
    delete scene;
    delete graphicsView;
    delete textEdit;
    delete ui;
}

void MainWindow::initGraphicsRegion(QWidget *parent, QLayout *layout)
{
    QGroupBox *graphicsBox = new QGroupBox(tr("Image"), parent);
    graphicsBox->setFlat(true);
    graphicsBox->setAlignment(Qt::AlignHCenter);

    graphicsView = new QGraphicsView();
    // Cache backgroud to speed up its repaint.
    graphicsView->setCacheMode(QGraphicsView::CacheBackground);
    graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);

    scene = new Scene();
    // Disable scroll bar extend with a non-null rectangle.
    scene->setSceneRect(0, 0, 0, 1);
    graphicsView->setScene(scene);

    QHBoxLayout *graphicsLayout = new QHBoxLayout(graphicsBox);
    graphicsLayout->addWidget(graphicsView);
    layout->addWidget(graphicsBox);
}

void MainWindow::initTextRegion(QWidget *parent, QLayout *layout)
{
    QGroupBox *textBox = new QGroupBox(tr("Previsualization"), parent);
    textBox->setFlat(true);
    textBox->setAlignment(Qt::AlignHCenter);

    textEdit = new QPlainTextEdit(textBox);

    QHBoxLayout *textLayout = new QHBoxLayout(textBox);
    textLayout->addWidget(textEdit);
    layout->addWidget(textBox);
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
