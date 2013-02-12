#include "segmentationoptions.h"
#include "ui_segmentationoptions.h"

SegmentationOptions::SegmentationOptions(QWidget *parent) :
        OptionWidget(parent),
        ui(new Ui::SegmentationOptions)
{
    ui->setupUi(this);

    ui->find_sepsCbox->insertItem(Separator::Lines, "Lines");
    ui->find_sepsCbox->insertItem(Separator::Whitespaces, "Whitespaces");
    ui->find_sepsCbox->insertItem(Separator::Both, "Lines and whitespaces");

    loadConfig();
}

SegmentationOptions::~SegmentationOptions()
{
    delete ui;
}

void SegmentationOptions::loadConfig()
{
    Configs *const conf = Configs::getInstance();
    ui->find_sepsCbox->setCurrentIndex(conf->segmentationFindSeps());
}


void SegmentationOptions::saveConfig()
{
    Configs *const conf = Configs::getInstance();
    conf->setSegmentationFindSeps(ui->find_sepsCbox->currentIndex());
}
