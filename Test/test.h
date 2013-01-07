#ifndef TEST_H
#define TEST_H

#include <QMainWindow>

namespace Ui {
    class Test;
}

class Test : public QMainWindow {
    Q_OBJECT
public:
    Test(QWidget *parent = 0);
    ~Test();

protected:
    void changeEvent(QEvent *e);
    void contextMenuEvent(QContextMenuEvent *event);

private:
    void createActions();
    void createMenus();

    Ui::Test *ui;
    QMenu *openMenu;
    QAction *openAction;
};

#endif // TEST_H
