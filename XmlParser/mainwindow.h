#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QFileDialog>
#include "scene.h"
#include "xml.h"
#include "polygonitem.h"

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

    private slots:
        void on_actionOpen_triggered();
        //void on_action(QString& string);
};

#endif // MAINWINDOW_H

