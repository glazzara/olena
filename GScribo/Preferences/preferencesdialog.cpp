#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"

PreferencesDialog::PreferencesDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::PreferencesDialog)
{
    ui->setupUi(this);

    // We may want to remove this useless object in the ui file.
    delete ui->widget;

    connect(ui->optionList, SIGNAL(currentRowChanged(int)), this, SLOT(onOptionListCurrentRowChanged(int)));
    setAttribute(Qt::WA_DeleteOnClose);

    loadOptionList();
    ui->optionList->setCurrentRow(0);
}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}

void PreferencesDialog::loadOptionList()
{
    static const char *options[] = { "General", "Preprocessing", "Segmentation", "OCR", 0 };

    int i;
    for (i = 0; options[i]; ++i)
        ui->optionList->insertItem(i, options[i]);

    widgets.fill(0, i);
}

void PreferencesDialog::selectOptionWidget(int row)
{
    if(!widgets[row])
    {
        QWidget *widget = 0;

        switch(row)
        {
            case 0:
                widget = new GeneralOptions(this);
                break;

            case 1:
                widget = new PreprocessingOptions(this);
                break;

            case 2:
                widget = new SegmentationOptions(this);
                break;

            case 3:
                widget = new OcrOptions(this);
                break;
        }

        if(widget)
            widgets[row] = widget;
    }

    if(ui->horizontalLayout_2->count() == 2)
    {
        QWidget *current_widget = ui->horizontalLayout_2->itemAt(1)->widget();
        ui->horizontalLayout_2->removeWidget(current_widget);
        current_widget->hide();
    }

    ui->horizontalLayout_2->insertWidget(1, widgets[row]);
    widgets[row]->show();
}


void PreferencesDialog::accept()
{
    for (int i = 0; i < widgets.size(); ++i)
    {
        if (widgets[i])
        {
            static_cast<OptionWidget *>(widgets[i])->saveConfig();
            delete widgets[i];
        }
    }

    QDialog::accept();
}


void PreferencesDialog::reject()
{
    for (int i = 0; i < widgets.size(); ++i)
        delete widgets[i];

    QDialog::reject();
}
