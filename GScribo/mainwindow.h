#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QPrintPreviewDialog>
#include <QGraphicsView>
#include <QPrintDialog>
#include <QMainWindow>
#include <QHBoxLayout>
#include <QDockWidget>
#include <QFileDialog>
#include <QGroupBox>
#include <QPrinter>

#include "Preferences/preferencesdialog.h"
#include "Processing/progressdialog.h"
#include "PagesWidget/pageswidget.h"
#include "Rendering/graphicsview.h"
#include "XmlWidget/xmlwidget.h"
#include "Processing/runner.h"
#include "Rendering/scene.h"
#include "regionwidget.h"
#include "aboutdialog.h"
#include "dockwidget.h"
#include "xml.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow:
        public QMainWindow
{
        Q_OBJECT

    public:
        MainWindow(QWidget *parent = 0);
        ~MainWindow();

    protected:
        void changeEvent(QEvent *e);

    private:
        void initGraphicsRegion();
        void initDialogsWidget();
        void initPageWidget();
        void initRegionWidget();
        void initXmlWidget();
        void initMenuBar();
        void connectWidgets();

        void setActionsEnabled(bool isSegmented);
        void disableActions();

        void printItems(QPainter *painter, const QList<QGraphicsItem *>& items, QStyleOptionGraphicsItem *options);

        Xml processTmpXml(const QString& filename) const;
        QList<RegionItem *> toRegionItems(QList<XmlItem *> regionItems) const;
        QList<XmlItem *> toXmlItems(QList<RegionItem *> xmlItems) const;

        Ui::MainWindow *ui;

        DockWidget dockRegion_;
        DockWidget dockPages_;
        DockWidget dockXml_;

        GraphicsView graphicsView_;
        Scene scene_;

        PagesWidget pagesWidget_;
        RegionWidget regionWidget_;

        AboutDialog *aboutDialog_;
        ProgressDialog *progressDialog_;
        PreferencesDialog *preferencesDialog_;

        XmlWidget xmlWidget_;
        Xml xml_;
        Runner runner_;

        QAction *segment_;
        QAction *previewPrinting_;
        QAction *print_;
        QAction *export_;

    public slots:
        void printScene(QPrinter *printer);

    private slots:
        void onOpen();
        void onSegment();
        void onPreviewPrint();
        void onPrint();
        void onExportation();
        inline void onPreferences();
        inline void onAbout();

        void onXmlSaved(const QString& filename);
        void onFileChanged(const QString& filename);

        void onRegionSelection(QList<RegionItem *> regionItems);

        void onXmlChangeSelection(QList<XmlItem *> xmlItems, bool select);
        inline void onXmlSelect(QList<XmlItem *> xmlItems);
        inline void onXmlUnselect(QList<XmlItem *> xmlItems);
};

inline void MainWindow::onPreferences()
{ preferencesDialog_->show(); }

inline void MainWindow::onAbout()
{ aboutDialog_->show(); }

inline void MainWindow::onXmlSelect(QList<XmlItem *> xmlItems)
{ onXmlChangeSelection(xmlItems, true); }

inline void MainWindow::onXmlUnselect(QList<XmlItem *> xmlItems)
{ onXmlChangeSelection(xmlItems, false); }

#endif // MAINWINDOW_H
