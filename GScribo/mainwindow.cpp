 #include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle(tr("GScribo"));
    statusBar()->hide();

    initGraphicsRegion();
    initTextRegion();
    initXmlWidget();
    initRegionWidget();
    initPageWidget();
    initToolBar();
    initMenuBar();

    // Told the left dock widget (here the pages widget and the region widget) to fill the bottom left corner.
    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    // Merge the region and pages widget in one tab (pages widget over the region widget).
    tabifyDockWidget(&dockRegion_, &dockPages_);

    connectWidgets();
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

void MainWindow::initGraphicsRegion()
{
    graphicsView_.setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    graphicsView_.setScene(&scene_);

    setCentralWidget(&graphicsView_);
}

void MainWindow::initPageWidget()
{
    dockPages_.setWindowTitle("Pages");
    dockPages_.setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);
    dockPages_.setWidget(&pagesWidget_);
    dockPages_.setMinimumWidth(190);

    addDockWidget(Qt::LeftDockWidgetArea, &dockPages_);
}

void MainWindow::initRegionWidget()
{
    dockRegion_.setWindowTitle("Regions");
    dockRegion_.setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);
    dockRegion_.setWidget(&regionWidget_);

    addDockWidget(Qt::LeftDockWidgetArea, &dockRegion_);
}

void MainWindow::initXmlWidget()
{
    dockXml_.setWindowTitle("Xml");
    dockXml_.setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);
    dockXml_.setWidget(&xmlWidget_);

    addDockWidget(Qt::BottomDockWidgetArea, &dockXml_);
    dockXml_.setVisible(false);
}

void MainWindow::initTextRegion()
{
    dockText_.setWindowTitle("Text");
    dockText_.setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
    dockText_.setWidget(&textEdit_);

    addDockWidget(Qt::RightDockWidgetArea, &dockText_);
    dockText_.hide();
}

void MainWindow::initToolBar()
{
    QAction *open = ui->mainToolBar->addAction(tr("Open"));
    connect(open, SIGNAL(triggered()), this, SLOT(onOpen()));

    QAction *segment = ui->mainToolBar->addAction(tr("Segment"));
    connect(segment, SIGNAL(triggered()), this, SLOT(onSegment()));
}

void MainWindow::initMenuBar()
{
    QMenu *menuFile = ui->menuBar->addMenu(tr("File"));

    QAction *open = menuFile->addAction(tr("Open"));
    connect(open, SIGNAL(triggered()), SLOT(onOpen()));

    QAction *segment = menuFile->addAction(tr("Segment"));
    connect(segment, SIGNAL(triggered()), this, SLOT(onSegment()));

    QMenu *menuAreas = ui->menuBar->addMenu(tr("Areas"));

    QAction *page = menuAreas->addAction(tr("Page"));
    page->setCheckable(true);
    page->setChecked(true);
    connect(page, SIGNAL(triggered()), &dockPages_, SLOT(switchVisibility()));

    QAction *region = menuAreas->addAction(tr("Region"));
    region->setCheckable(true);
    region->setChecked(true);
    connect(region, SIGNAL(triggered()), &dockRegion_, SLOT(switchVisibility()));

    QAction *xml = menuAreas->addAction(tr("Xml"));
    xml->setCheckable(true);
    xml->setChecked(false);
    connect(xml, SIGNAL(triggered()), &dockXml_, SLOT(switchVisibility()));

    QAction *preferences = ui->menuBar->addAction(tr("Preferences"));
    connect(preferences, SIGNAL(triggered()), SLOT(onPreferences()));

    QAction *about = ui->menuBar->addAction(tr("About"));
    connect(about, SIGNAL(triggered()), SLOT(onAbout()));
}

void MainWindow::connectWidgets()
{
    // Each time the scene rect change (when a new picture is loaded), we fit the scene background rectangle in the view.
    connect(&scene_, SIGNAL(sceneRectChanged(QRectF)), &graphicsView_, SLOT(fitInView(QRectF)));

    // If double click on a picture of the page widget -> draw it on background scene.
    connect(&pagesWidget_, SIGNAL(imageSelectionned(QString)), this, SLOT(onFileChanged(QString)));

    // Connect the scene to the xml widget and vice versa.
    connect(&scene_, SIGNAL(newSelection(QList<RegionItem*>)), this, SLOT(onRegionSelection(QList<RegionItem*>)));
    connect(xmlWidget_.view(), SIGNAL(select(QList<XmlItem*>)), this, SLOT(onXmlSelect(QList<XmlItem*>)));
    connect(xmlWidget_.view(), SIGNAL(unselect(QList<XmlItem*>)), this, SLOT(onXmlUnselect(QList<XmlItem*>)));
    connect(xmlWidget_.view(), SIGNAL(emptySelection()), &scene_, SLOT(selectBase()));
    connect(xmlWidget_.view(), SIGNAL(resetSelection()), &scene_, SLOT(clearSelection()));

    // Connect the scene with the region widget.
    connect(&regionWidget_, SIGNAL(checkStateChanged(GraphicsRegion::Id,bool)), &scene_, SLOT(setVisible(GraphicsRegion::Id,bool)));

    // Connect the xml widget with the region widget.
    connect(&regionWidget_, SIGNAL(checkStateChanged(QString)), xmlWidget_.view(), SLOT(setFilterString(QString)));

    /*connect(&runner, SIGNAL(progress()), &progressDialog, SLOT(run()));
    connect(&runner, SIGNAL(new_progress_max_value(int)), &progressDialog, SLOT(setMaximum(int)));
    connect(&runner, SIGNAL(new_progress_label(QString)), &progressDialog, SLOT(setLabelText(QString)));
    connect(&runner, SIGNAL(finished()), &progressDialog, SLOT(close()));
    connect(&runner, SIGNAL(xml_saved(QString)), this, SLOT(onXmlSaved(QString)));*/
}

void MainWindow::onOpen()
{
    QStringList paths = QFileDialog::getOpenFileNames(this, "Open Image(s)", QDir::homePath(), "Images (*.png *.jpg *.ppm *.bmp)");

    if(paths.count() > 0)
    {
        QStringList filenames = pagesWidget_.filenames();
        QString path;

        int counter = 0;
        bool isContained;
        // Check for an image not already added to the page widget.
        do
        {
            path = paths[counter];
            counter++;
            isContained = filenames.contains(path);
        } while(isContained && counter < paths.count());

        if(!isContained)
        {
            pagesWidget_.addPicture(path, QPixmap(path));

            // If more than one file, we store it in the page widget.
            for(int i = counter; i < paths.count(); i++)
            {
                // Check if the page widget contains the image.
                if(!filenames.contains(paths[i]))
                {
                    path = paths[i];
                    pagesWidget_.addPicture(path, QPixmap(path));
                }
            }

            pagesWidget_.setCurrentRow(0);
            pagesWidget_.scrollToTop();
        }

        // Change current scene.
        onFileChanged(path);
    }
}

void MainWindow::onSegment()
{
    if(scene_.backgroundPath() != "")
    {
        QStringList filenames;

        if(!pagesWidget_.isVisible())
            filenames << scene_.backgroundPath();
        else
            filenames = pagesWidget_.filenames();

        // Run segmentation of page(s).
        //progressDialog.reset();
        //runner.start_demat(filenames);
    }
}

void MainWindow::onPreferences()
{
    PreferencesDialog *preferenceDialog = new PreferencesDialog(this);
    preferenceDialog->show();
}

void MainWindow::onAbout()
{
    AboutDialog *about = new AboutDialog;
    about->show();
}

void MainWindow::onXmlSaved(const QString& filename)
{
    xml_.load(filename);
    xmlWidget_.changeView(xml_.xmlItem());
    scene_.setRoot(xml_.graphicsItem());
}

void MainWindow::onFileChanged(const QString& filename)
{
    // If it's not the current scene.
    if(scene_.backgroundPath() != filename)
    {
        QString xmlPath = Xml::getPath(filename);
        // Check if the xml file already exists.
        if(!QFile(xmlPath).exists())
            xmlPath = QString();

        xml_.load(xmlPath);
        scene_.changeScene(filename, xml_.graphicsItem());
        xmlWidget_.changeView(xml_.xmlItem());
    }
}

QList<RegionItem *> MainWindow::toRegionItems(QList<XmlItem *> xmlItems) const
{
    QList<RegionItem *> regionItems;
    XmlItem *child;
    foreach(child, xmlItems)
    {
        if(child->regionItem())
            regionItems << child->regionItem();
    }

    return regionItems;
}

QList<XmlItem *> MainWindow::toXmlItems(QList<RegionItem *> regionItems) const
{
    QList<XmlItem *> xmlItems;
    RegionItem *child;
    foreach(child, regionItems)
        xmlItems << child->xmlItem();

    return xmlItems;
}

void MainWindow::onRegionSelection(QList<RegionItem *> regionItems)
{
    if(!regionItems.isEmpty())
        xmlWidget_.view()->displayOnly(toXmlItems(regionItems));
    else
    {
        xmlWidget_.view()->reset();
        xmlWidget_.view()->displayAll();
        xmlWidget_.view()->expandAll();
    }
}

void MainWindow::onXmlChangeSelection(QList<XmlItem *> xmlItems, bool select)
{
    QList<RegionItem *> regionItems = toRegionItems(xmlItems);

    if(!regionItems.isEmpty())
    {
        if(select)
            scene_.select(regionItems);
        else
            scene_.unselect(regionItems);
    }
}
