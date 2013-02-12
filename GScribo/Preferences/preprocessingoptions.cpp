#include "preprocessingoptions.h"
#include "ui_preprocessingoptions.h"

//using namespace scribo::toolchain::internal;

PreprocessingOptions::PreprocessingOptions(QWidget *parent) :
        OptionWidget(parent),
        ui(new Ui::PreprocessingOptions)
{
    ui->setupUi(this);

    /*ui->bin_algoCbox->insertItem(Convert, "Violent convert");
    ui->bin_algoCbox->insertItem(Sauvola, "Local threshold");
    ui->bin_algoCbox->insertItem(SauvolaMs, "Local threshold multiscale");*/

    loadConfig();
}

PreprocessingOptions::~PreprocessingOptions()
{
    delete ui;
}

void PreprocessingOptions::loadConfig()
{
    Configs *const conf = Configs::getInstance();

    ui->subsampleCb->setChecked(conf->preprocessingSubsample());
    ui->remove_bgCb->setChecked(conf->preprocessingRemoveBg());
    ui->deskewCb->setChecked(conf->preprocessingDeskew());
    ui->remove_noiseCb->setChecked(conf->preprocessingRemoveNoise());
    //ui->bin_algoCbox->setCurrentIndex(conf->preprocessingBinAlgo());
}


void PreprocessingOptions::saveConfig()
{
    Configs *const conf = Configs::getInstance();

    conf->setPreprocessingSubsample(ui->subsampleCb->isChecked());
    conf->setPreprocessingRemoveBg(ui->remove_bgCb->isChecked());
    conf->setPreprocessingDeskew(ui->deskewCb->isChecked());
    conf->setPreprocessingRemoveNoise(ui->remove_noiseCb->isChecked());
    //conf->setPreprocessingBinAlgo(ui->bin_algoCbox->currentIndex());
}
