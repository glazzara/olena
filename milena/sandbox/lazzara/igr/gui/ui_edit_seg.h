/********************************************************************************
** Form generated from reading ui file 'edit_seg.ui'
**
** Created: Wed Oct 28 12:45:22 2009
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_EDIT_SEG_H
#define UI_EDIT_SEG_H

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

class Ui_EditSeg
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *filepath;
    QPushButton *browseBtn;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer;
    QPushButton *reloadBtn;
    QPushButton *saveBtn;
    mln::demo::image_viewer *viewer;

    void setupUi(QWidget *EditSeg)
    {
    if (EditSeg->objectName().isEmpty())
        EditSeg->setObjectName(QString::fromUtf8("EditSeg"));
    EditSeg->resize(400, 300);
    gridLayout = new QGridLayout(EditSeg);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    verticalLayout = new QVBoxLayout();
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    horizontalLayout_2 = new QHBoxLayout();
    horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
    label_2 = new QLabel(EditSeg);
    label_2->setObjectName(QString::fromUtf8("label_2"));

    horizontalLayout_2->addWidget(label_2);

    filepath = new QLineEdit(EditSeg);
    filepath->setObjectName(QString::fromUtf8("filepath"));

    horizontalLayout_2->addWidget(filepath);

    browseBtn = new QPushButton(EditSeg);
    browseBtn->setObjectName(QString::fromUtf8("browseBtn"));

    horizontalLayout_2->addWidget(browseBtn);


    verticalLayout->addLayout(horizontalLayout_2);

    horizontalLayout_3 = new QHBoxLayout();
    horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
    horizontalSpacer = new QSpacerItem(48, 17, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_3->addItem(horizontalSpacer);

    reloadBtn = new QPushButton(EditSeg);
    reloadBtn->setObjectName(QString::fromUtf8("reloadBtn"));

    horizontalLayout_3->addWidget(reloadBtn);

    saveBtn = new QPushButton(EditSeg);
    saveBtn->setObjectName(QString::fromUtf8("saveBtn"));
    saveBtn->setEnabled(false);

    horizontalLayout_3->addWidget(saveBtn);


    verticalLayout->addLayout(horizontalLayout_3);

    viewer = new mln::demo::image_viewer(EditSeg);
    viewer->setObjectName(QString::fromUtf8("viewer"));
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(viewer->sizePolicy().hasHeightForWidth());
    viewer->setSizePolicy(sizePolicy);

    verticalLayout->addWidget(viewer);


    gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


    retranslateUi(EditSeg);

    QMetaObject::connectSlotsByName(EditSeg);
    } // setupUi

    void retranslateUi(QWidget *EditSeg)
    {
    EditSeg->setWindowTitle(QApplication::translate("EditSeg", "Form", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("EditSeg", "Segmentation", 0, QApplication::UnicodeUTF8));
    browseBtn->setText(QApplication::translate("EditSeg", "&Browse", 0, QApplication::UnicodeUTF8));
    reloadBtn->setText(QApplication::translate("EditSeg", "&Reload", 0, QApplication::UnicodeUTF8));
    saveBtn->setText(QApplication::translate("EditSeg", "&Save changes", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(EditSeg);
    } // retranslateUi

};

namespace Ui {
    class EditSeg: public Ui_EditSeg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDIT_SEG_H
