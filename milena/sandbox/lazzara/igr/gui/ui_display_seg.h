/********************************************************************************
** Form generated from reading ui file 'display_seg.ui'
**
** Created: Wed Oct 28 12:45:22 2009
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_DISPLAY_SEG_H
#define UI_DISPLAY_SEG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "src/image_viewer.hh"

QT_BEGIN_NAMESPACE

class Ui_DisplaySeg
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *filepath;
    QPushButton *browseBtn;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *segfilepath;
    QPushButton *browseSegBtn;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer;
    QPushButton *loadBtn;
    mln::demo::image_viewer *viewer;

    void setupUi(QWidget *DisplaySeg)
    {
    if (DisplaySeg->objectName().isEmpty())
        DisplaySeg->setObjectName(QString::fromUtf8("DisplaySeg"));
    DisplaySeg->resize(400, 300);
    gridLayout = new QGridLayout(DisplaySeg);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    verticalLayout = new QVBoxLayout();
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    label = new QLabel(DisplaySeg);
    label->setObjectName(QString::fromUtf8("label"));

    horizontalLayout->addWidget(label);

    filepath = new QLineEdit(DisplaySeg);
    filepath->setObjectName(QString::fromUtf8("filepath"));

    horizontalLayout->addWidget(filepath);

    browseBtn = new QPushButton(DisplaySeg);
    browseBtn->setObjectName(QString::fromUtf8("browseBtn"));

    horizontalLayout->addWidget(browseBtn);


    verticalLayout->addLayout(horizontalLayout);

    horizontalLayout_2 = new QHBoxLayout();
    horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
    label_2 = new QLabel(DisplaySeg);
    label_2->setObjectName(QString::fromUtf8("label_2"));

    horizontalLayout_2->addWidget(label_2);

    segfilepath = new QLineEdit(DisplaySeg);
    segfilepath->setObjectName(QString::fromUtf8("segfilepath"));

    horizontalLayout_2->addWidget(segfilepath);

    browseSegBtn = new QPushButton(DisplaySeg);
    browseSegBtn->setObjectName(QString::fromUtf8("browseSegBtn"));

    horizontalLayout_2->addWidget(browseSegBtn);


    verticalLayout->addLayout(horizontalLayout_2);

    horizontalLayout_3 = new QHBoxLayout();
    horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
    horizontalSpacer = new QSpacerItem(48, 17, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_3->addItem(horizontalSpacer);

    loadBtn = new QPushButton(DisplaySeg);
    loadBtn->setObjectName(QString::fromUtf8("loadBtn"));

    horizontalLayout_3->addWidget(loadBtn);


    verticalLayout->addLayout(horizontalLayout_3);

    viewer = new mln::demo::image_viewer(DisplaySeg);
    viewer->setObjectName(QString::fromUtf8("viewer"));
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(viewer->sizePolicy().hasHeightForWidth());
    viewer->setSizePolicy(sizePolicy);

    verticalLayout->addWidget(viewer);


    gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


    retranslateUi(DisplaySeg);

    QMetaObject::connectSlotsByName(DisplaySeg);
    } // setupUi

    void retranslateUi(QWidget *DisplaySeg)
    {
    DisplaySeg->setWindowTitle(QApplication::translate("DisplaySeg", "Form", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("DisplaySeg", "Image", 0, QApplication::UnicodeUTF8));
    browseBtn->setText(QApplication::translate("DisplaySeg", "&Browse", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("DisplaySeg", "Segmentation", 0, QApplication::UnicodeUTF8));
    browseSegBtn->setText(QApplication::translate("DisplaySeg", "&Browse", 0, QApplication::UnicodeUTF8));
    loadBtn->setText(QApplication::translate("DisplaySeg", "&Load", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(DisplaySeg);
    } // retranslateUi

};

namespace Ui {
    class DisplaySeg: public Ui_DisplaySeg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DISPLAY_SEG_H
