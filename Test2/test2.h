#ifndef TEST2_H
#define TEST2_H

#include <QScrollArea>
#include <QTextEdit>
#include <QMainWindow>
#include <QShortcut>
#include "picture.h"

namespace Ui
{
    class Test2;
}

class Test2 : public QMainWindow {
    Q_OBJECT
public:
    Test2(QWidget *parent = 0);
    ~Test2();

protected:
    void changeEvent(QEvent *e);

private:
    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);

    Ui::Test2 *ui;
    Picture *picture;
    QScrollArea *scrollArea;
    double scaleFactor;

    QShortcut *shortcutZoomIn;
    QShortcut *shortcutZoomOut;

private slots:
    void on_actionZoomOut_triggered();
    void on_actionZoomIn_triggered();
    void on_actionOpen_triggered();
};

#endif // TEST2_H
