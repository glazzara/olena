#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QDockWidget>
#include <QGraphicsView>
#include <QGLWidget>
#include <QPlainTextEdit>
#include <QGroupBox>
#include <QFileDialog>

#include "scene.h"
#include "pageswidget.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
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
        void initTablePage();

        Ui::MainWindow *ui;
        QDockWidget *dockText;
        QDockWidget *dockPages;
        Scene *scene;
        QGraphicsView *graphicsView;
        QPlainTextEdit *textEdit;
        PagesWidget *pagesWidget;
        QString imagePath;

    private slots:
        void onOpen();
};

#endif // MAINWINDOW_H
