/********************************************************************************
** Form generated from reading ui file 'main_window.ui'
**
** Created: Wed Oct 28 13:14:49 2009
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MAIN_WINDOW_H
#define UI_MAIN_WINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QWidget>
#include "src/display_seg.hh"
#include "src/edit_seg.hh"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    mln::demo::display_seg *Visualization;
    mln::demo::edit_seg *Edition;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
    if (MainWindow->objectName().isEmpty())
        MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
    MainWindow->resize(487, 335);
    centralwidget = new QWidget(MainWindow);
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
    gridLayout = new QGridLayout(centralwidget);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    tabWidget = new QTabWidget(centralwidget);
    tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
    Visualization = new mln::demo::display_seg();
    Visualization->setObjectName(QString::fromUtf8("Visualization"));
    tabWidget->addTab(Visualization, QString());
    Edition = new mln::demo::edit_seg();
    Edition->setObjectName(QString::fromUtf8("Edition"));
    tabWidget->addTab(Edition, QString());

    gridLayout->addWidget(tabWidget, 0, 0, 1, 1);

    MainWindow->setCentralWidget(centralwidget);
    menubar = new QMenuBar(MainWindow);
    menubar->setObjectName(QString::fromUtf8("menubar"));
    menubar->setGeometry(QRect(0, 0, 487, 31));
    MainWindow->setMenuBar(menubar);
    statusbar = new QStatusBar(MainWindow);
    statusbar->setObjectName(QString::fromUtf8("statusbar"));
    MainWindow->setStatusBar(statusbar);

    retranslateUi(MainWindow);

    tabWidget->setCurrentIndex(0);


    QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
    MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Segmentation tools", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(Visualization), QApplication::translate("MainWindow", "Visualization", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(Edition), QApplication::translate("MainWindow", "Edition", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(MainWindow);
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAIN_WINDOW_H
