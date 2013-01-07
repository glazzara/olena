#ifndef TEST2_H
#define TEST2_H

#include <QScrollArea>
#include <QTextEdit>
#include <QMainWindow>

namespace Ui {
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
    Ui::Test2 *ui;
    QScrollArea *scrollArea;

private slots:
    void on_actionOpen_triggered();
};

#endif // TEST2_H
