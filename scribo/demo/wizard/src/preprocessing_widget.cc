#include <src/preprocessing_widget.hh>

namespace scribo
{

  namespace demo
  {


    preprocessing_widget::preprocessing_widget(QWidget *parent)
      : QWidget(parent)
    {
      setupUi(this);
    }

    void preprocessing_widget::reset()
    {
      binarizationCBox->setCurrentIndex(2); // Sauvola ms
      clear_checked();
    }

    void preprocessing_widget::setup_picture()
    {
      qDebug() << "setup picture";

      removeBg->setChecked(true);
    }

    void preprocessing_widget::setup_mixed_doc()
    {
      qDebug() << "setup mixed doc";
    }

    void preprocessing_widget::setup_text_doc()
    {
      qDebug() << "setup text doc";

      // Disabled
      removeBg->setChecked(false);
      unskew->setChecked(false);

      // Enabled
      removeNoise->setChecked(true);
      improveContrast->setChecked(true);
    }


    void preprocessing_widget::clear_checked()
    {
      // Enabled

      // Disabled
      removeBg->setChecked(false);
      unskew->setChecked(false);
      removeNoise->setChecked(false);
      improveContrast->setChecked(false);
      reduceSize->setChecked(false);
    }


    preprocessing_task_set_t preprocessing_widget::tasks() const
    {
      preprocessing_task_set_t tasks;

      if (removeBg->isChecked())
	tasks.insert(RemoveBg);

      if (unskew->isChecked())
	tasks.insert(Unskew);

      if (removeNoise->isChecked())
	tasks.insert(RemoveNoise);

      if (improveContrast->isChecked())
	tasks.insert(ImproveContrast);

      if (reduceSize->isChecked())
	tasks.insert(ReduceSize);

      if (binarizationCBox->currentIndex() == 0)
	tasks.insert(BinarizationSimple);
      else if (binarizationCBox->currentIndex() == 1)
	tasks.insert(BinarizationSauvola);
      else if (binarizationCBox->currentIndex() == 2)
	tasks.insert(BinarizationSauvolaMs);

      return tasks;
    }


  } // end of namespace scribo::demo

} // end of namespace scribo
