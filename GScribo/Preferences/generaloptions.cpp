#include "generaloptions.h"
#include "ui_generaloptions.h"

GeneralOptions::GeneralOptions(QWidget *parent) :
        OptionWidget(parent),
        ui(new Ui::GeneralOptions)
{
    ui->setupUi(this);
    loadConfig();
}

GeneralOptions::~GeneralOptions()
{
    delete ui;
}

void GeneralOptions::loadConfig()
{
    Configs * const conf = Configs::getInstance();

    ui->saveXml->setChecked(conf->generalSaveXmlEnabled());
    ui->sameDir->setChecked(conf->generalSaveXmlSameDir());
    ui->customDir->setChecked(conf->generalSaveXmlCustomDir());
    ui->customDirValue->setText(conf->generalSaveXmlCustomDirPath());
}


void GeneralOptions::saveConfig()
{
    Configs * const conf = Configs::getInstance();

    conf->setGeneralSaveXmlEnabled(ui->saveXml->isChecked());
    conf->setGeneralSaveXmlSameDir(ui->sameDir->isChecked());
    conf->setGeneralSaveXmlCustomDir(ui->customDir->isChecked());
    conf->setGeneralSaveXmlCustomDirPath(ui->customDirValue->text());
}

void GeneralOptions::onCustomDirBrowseBtnClicked()
{
    QString dir = QFileDialog::getExistingDirectory(0, "Choose a directory");

    if (!dir.isEmpty())
        ui->customDirValue->setText(dir);
}
