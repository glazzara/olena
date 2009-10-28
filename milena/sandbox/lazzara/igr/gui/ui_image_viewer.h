/********************************************************************************
** Form generated from reading ui file 'image_viewer.ui'
**
** Created: Wed Oct 28 12:45:22 2009
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_IMAGE_VIEWER_H
#define UI_IMAGE_VIEWER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGraphicsView>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ImageViewer
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QLabel *zoomLabel;
    QPushButton *zoomIn;
    QPushButton *zoomOut;
    QPushButton *zoomFixed;
    QPushButton *zoomOriginal;
    QSpacerItem *horizontalSpacer_2;
    QGraphicsView *image;
    QHBoxLayout *sliderLayout;
    QSlider *slider;
    QLabel *label_2;
    QLabel *frame_nb;

    void setupUi(QWidget *ImageViewer)
    {
    if (ImageViewer->objectName().isEmpty())
        ImageViewer->setObjectName(QString::fromUtf8("ImageViewer"));
    ImageViewer->resize(464, 350);
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(ImageViewer->sizePolicy().hasHeightForWidth());
    ImageViewer->setSizePolicy(sizePolicy);
    verticalLayout_2 = new QVBoxLayout(ImageViewer);
    verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
    verticalLayout = new QVBoxLayout();
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    horizontalLayout_2 = new QHBoxLayout();
    horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_2->addItem(horizontalSpacer);

    zoomLabel = new QLabel(ImageViewer);
    zoomLabel->setObjectName(QString::fromUtf8("zoomLabel"));
    zoomLabel->setEnabled(false);

    horizontalLayout_2->addWidget(zoomLabel);

    zoomIn = new QPushButton(ImageViewer);
    zoomIn->setObjectName(QString::fromUtf8("zoomIn"));
    zoomIn->setEnabled(false);

    horizontalLayout_2->addWidget(zoomIn);

    zoomOut = new QPushButton(ImageViewer);
    zoomOut->setObjectName(QString::fromUtf8("zoomOut"));
    zoomOut->setEnabled(false);

    horizontalLayout_2->addWidget(zoomOut);

    zoomFixed = new QPushButton(ImageViewer);
    zoomFixed->setObjectName(QString::fromUtf8("zoomFixed"));
    zoomFixed->setEnabled(false);

    horizontalLayout_2->addWidget(zoomFixed);

    zoomOriginal = new QPushButton(ImageViewer);
    zoomOriginal->setObjectName(QString::fromUtf8("zoomOriginal"));
    zoomOriginal->setEnabled(false);

    horizontalLayout_2->addWidget(zoomOriginal);

    horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_2->addItem(horizontalSpacer_2);


    verticalLayout->addLayout(horizontalLayout_2);

    image = new QGraphicsView(ImageViewer);
    image->setObjectName(QString::fromUtf8("image"));
    image->setInteractive(true);
    image->setRenderHints(QPainter::SmoothPixmapTransform|QPainter::TextAntialiasing);

    verticalLayout->addWidget(image);

    sliderLayout = new QHBoxLayout();
    sliderLayout->setObjectName(QString::fromUtf8("sliderLayout"));
    slider = new QSlider(ImageViewer);
    slider->setObjectName(QString::fromUtf8("slider"));
    slider->setOrientation(Qt::Horizontal);
    slider->setTickPosition(QSlider::TicksAbove);

    sliderLayout->addWidget(slider);

    label_2 = new QLabel(ImageViewer);
    label_2->setObjectName(QString::fromUtf8("label_2"));

    sliderLayout->addWidget(label_2);

    frame_nb = new QLabel(ImageViewer);
    frame_nb->setObjectName(QString::fromUtf8("frame_nb"));
    QFont font;
    font.setBold(true);
    font.setWeight(75);
    frame_nb->setFont(font);

    sliderLayout->addWidget(frame_nb);


    verticalLayout->addLayout(sliderLayout);


    verticalLayout_2->addLayout(verticalLayout);


    retranslateUi(ImageViewer);

    QMetaObject::connectSlotsByName(ImageViewer);
    } // setupUi

    void retranslateUi(QWidget *ImageViewer)
    {
    ImageViewer->setWindowTitle(QApplication::translate("ImageViewer", "Form", 0, QApplication::UnicodeUTF8));
    zoomLabel->setText(QApplication::translate("ImageViewer", "Zoom:", 0, QApplication::UnicodeUTF8));
    zoomIn->setText(QApplication::translate("ImageViewer", "+", 0, QApplication::UnicodeUTF8));
    zoomOut->setText(QApplication::translate("ImageViewer", "-", 0, QApplication::UnicodeUTF8));
    zoomFixed->setText(QApplication::translate("ImageViewer", "Fixed width", 0, QApplication::UnicodeUTF8));
    zoomOriginal->setText(QApplication::translate("ImageViewer", "Original size", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("ImageViewer", "Current Frame :", 0, QApplication::UnicodeUTF8));
    frame_nb->setText(QApplication::translate("ImageViewer", "0", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(ImageViewer);
    } // retranslateUi

};

namespace Ui {
    class ImageViewer: public Ui_ImageViewer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMAGE_VIEWER_H
