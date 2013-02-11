#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initGraphicsRegion();
    initTextRegion();
    initTablePage();
    initToolBar();
}

MainWindow::~MainWindow()
{
    delete dockText;
    delete dockPages;
    delete scene;
    delete graphicsView;
    delete textEdit;
    delete ui;
}

void MainWindow::initGraphicsRegion()
{
    graphicsView = new QGraphicsView;
    // Cache backgroud to speed up its repaint.
    graphicsView->setCacheMode(QGraphicsView::CacheBackground);
    graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);

    scene = new Scene();
    // Disable scroll bar extend with a non-null rectangle.
    scene->setSceneRect(0, 0, 0, 1);
    graphicsView->setScene(scene);

    setCentralWidget(graphicsView);
}

void MainWindow::initTablePage()
{
    dockPages = new QDockWidget(tr("Pages"));
    dockPages->setAllowedAreas(Qt::AllDockWidgetAreas);
    dockPages->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);

    pagesWidget = new PagesWidget;
    dockPages->setWidget(pagesWidget);

    addDockWidget(Qt::LeftDockWidgetArea, dockPages);
}

void MainWindow::initTextRegion()
{
    dockText = new QDockWidget(tr("Previsualization"));
    dockText->setAllowedAreas(Qt::AllDockWidgetAreas);
    dockText->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);

    textEdit = new QPlainTextEdit;
    dockText->setWidget(textEdit);

    addDockWidget(Qt::RightDockWidgetArea, dockText);
    //dockText->hide();
}

void MainWindow::initToolBar()
{
   QAction *open = ui->mainToolBar->addAction(tr("Open"));
   connect(open, SIGNAL(triggered()), this, SLOT(onOpen()));
}

void MainWindow::onOpen()
{
    QString path = QFileDialog::getOpenFileName(this);
    if(path != "")
    {
        QPixmap pixmap(path);

        scene->setSceneRect(pixmap.rect());
        scene->setBackgroundBrush(QBrush(pixmap));

        pagesWidget->addPicture(path, pixmap);
    }
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
