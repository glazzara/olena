#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QToolBar>
#include <QAction>

class ToolBar :
        public QToolBar
{
    public:
        explicit ToolBar(QWidget *parent);
        ~ToolBar();
        QAction * openAction() const;
        QAction * segmentAction() const;
        QAction * quitAction() const;

    private:
        void init();

        QAction *open;
        QAction *segment;
        QAction *quit;


};

#endif // TOOLBAR_H
