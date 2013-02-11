#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsView>
#include <QMainWindow>

#include "xmlwidget.h"
#include "xml.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow :
        public QMainWindow
{
        Q_OBJECT

    public:
        MainWindow(QWidget *parent = 0);
        ~MainWindow();

    protected:
        void changeEvent(QEvent *e);

    private:
        Ui::MainWindow *ui;
        QGraphicsView *view;
        Xml xml;
};

#endif // MAINWINDOW_H
