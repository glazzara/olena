#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QDockWidget>
#include <QGraphicsView>
#include <QGLWidget>
#include <QPlainTextEdit>
#include <QGroupBox>

#include "scene.h"

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
        void initGraphicsRegion(QWidget *parent, QLayout *layout);
        void initTextRegion(QWidget *parent, QLayout *layout);

        Ui::MainWindow *ui;
        Scene *scene;
        QGraphicsView *graphicsView;
        QPlainTextEdit *textEdit;
};

#endif // MAINWINDOW_H
