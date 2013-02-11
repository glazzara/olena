#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsView>
#include <QPlainTextEdit>
#include <QMainWindow>
#include <QHBoxLayout>
#include <QDockWidget>
#include <QFileDialog>
#include <QGroupBox>

#include "Preferences/preferencesdialog.h"
#include "Processing/progressdialog.h"
#include "PagesViewer/pageswidget.h"
#include "Processing/runner.h"
#include "Rendering/scene.h"

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
        void initMenuBar();
        void connectWidgets();
        void connectShortcuts();

        Ui::MainWindow *ui;
        QDockWidget dockText;
        QDockWidget dockPages;
        QGraphicsView graphicsView;
        Scene scene;
        QPlainTextEdit textEdit;
        PagesWidget pagesWidget;
        Runner runner;
        ProgressDialog progressDialog;

    private slots:
        void onOpen();
        void onSegment();
        void onPreferences();
};

#endif // MAINWINDOW_H
