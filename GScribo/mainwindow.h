#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsView>
#include <QPlainTextEdit>
#include <QMainWindow>
#include <QHBoxLayout>
#include <QDockWidget>
#include <QFileDialog>
#include <QGroupBox>

//#include "Preferences/preferencesdialog.h"
//#include "Processing/progressdialog.h"
#include "PagesWidget/pageswidget.h"
#include "XmlWidget/xmlwidget.h"
//#include "Processing/runner.h"
#include "Rendering/scene.h"
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
        void initTextRegion();
        void initToolBar();
        void initPageWidget();
        void initXmlWidget();
        void initMenuBar();
        void connectWidgets();
        void connectShortcuts();

        Xml processTmpXml(const QString& filename) const;

        Ui::MainWindow *ui;
        QDockWidget dockText;
        QDockWidget dockPages;
        QDockWidget dockXml;
        QGraphicsView graphicsView;
        Scene scene;
        QPlainTextEdit textEdit;
        PagesWidget pagesWidget;
        //Runner runner;
        //ProgressDialog progressDialog;
        Xml xml;
        XmlWidget xmlWidget;

    private slots:
        void onOpen();
        void onSegment();
        void onXmlSaved(const QString& filename);
        void onPreferences();
        void onFileChanged(const QString& filename, const QPixmap& pixmap);
};

#endif // MAINWINDOW_H
