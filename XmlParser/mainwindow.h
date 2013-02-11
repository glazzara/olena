#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QFileDialog>
#include "selectionitem.h"
#include "graphicsview.h"

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
        Ui::MainWindow *ui;
        SelectionItem *selectionItem;
        GraphicsView *graphicsView;

    private slots:
        void on_actionOpen_triggered();
};

#endif // MAINWINDOW_H

