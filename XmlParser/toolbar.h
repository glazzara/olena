#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QToolBar>

class ToolBar :
        public QToolBar
{
    public:
        explicit ToolBar(QWidget *parent = 0);
        explicit ToolBar(const QString& title, QWidget *parent = 0);

    private:
        void init();
};

#endif // TOOLBAR_H
