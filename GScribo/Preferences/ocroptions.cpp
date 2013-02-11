#include "ocroptions.h"
#include "ui_ocroptions.h"

//using namespace scribo::toolchain::internal;

static const char *language[][2] =
{
    { "English", "eng" },
    { "French", "fra" },
    { 0, 0 }
};

OcrOptions::OcrOptions(QWidget *parent) :
        OptionWidget(parent),
        ui(new Ui::OcrOptions)
{
    ui->setupUi(this);

    for (unsigned i = 0; language[i][0]; ++i)
        ui->ocr_language->insertItem(i, language[i][0]);

    loadConfig();
}

OcrOptions::~OcrOptions()
{
    delete ui;
}

int OcrOptions::findIndex(const QString& lang)
{
    for (unsigned i = 0; language[i][0]; ++i)
        if (lang == language[i][1])
            return i;

    return 0;
}


void OcrOptions::loadConfig()
{
    Configs * const conf = Configs::getInstance();

    ui->enable_ocr->setChecked(conf->ocrEnabled());
    ui->ocr_language->setCurrentIndex(findIndex(conf->ocrLanguage()));
}


void OcrOptions::saveConfig()
{
    Configs * const conf = Configs::getInstance();

    conf->setOcrEnabled(ui->enable_ocr->isChecked());
    conf->setOcrLanguage(language[ui->ocr_language->currentIndex()][1]);
}
