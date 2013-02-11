#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QProgressDialog>
#include "scene.h"
#include "toolbar.h"
#include "xml.h"
#include "polygonitem.h"
#include "runner.h"
#include "progressdialog.h"

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
        Scene * scene;
        ProgressDialog *progressDialog;
        QString imagePath;
        Runner runner;

    private slots:
        void on_actionOpen_triggered();
        void onBeginSegmentation();
        void onXmlSaved(const QString& filename);
        //void on_action(QString& string);
};

#endif // MAINWINDOW_H
