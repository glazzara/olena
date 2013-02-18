#ifndef DOCKWIDGET_H
#define DOCKWIDGET_H

#include <QDockWidget>

class DockWidget :
        public QDockWidget
{
        Q_OBJECT

    public:
        explicit DockWidget(QWidget *parent = 0, Qt::WindowFlags flags = 0) : QDockWidget(parent, flags) { }
        explicit DockWidget(const QString& title, QWidget *parent = 0, Qt::WindowFlags flags = 0) : QDockWidget(title, parent, flags) { }

    public slots:
        inline void switchVisibility();
};

inline void DockWidget::switchVisibility()
{ setVisible(!isVisible()); }

#endif // DOCKWIDGET_H
