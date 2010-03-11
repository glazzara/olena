#include <src/preprocessing_page.hh>

namespace scribo
{

  namespace demo
  {


    preprocessing_page::preprocessing_page(QWidget *parent)
      : QWizardPage(parent)
    {
      widget_ = new preprocessing_widget(this);

      QVBoxLayout *layout = new QVBoxLayout;
      layout->addWidget(widget_);
      setLayout(layout);

      setTitle("Preprocessing tasks");
      setSubTitle("Choose preprocessing algorithms.");

      registerField("preprocessing.tasks", this, "tasks");
    }


    void preprocessing_page::cleanupPage()
    {
      widget_->reset();
    }


    void preprocessing_page::initializePage()
    {
      if (field("doc_type.type_text_doc").toBool())
	widget_->setup_text_doc();
      else if (field("doc_type.type_mixed_doc").toBool())
	widget_->setup_mixed_doc();
      else if (field("doc_type.type_picture").toBool())
	widget_->setup_picture();
      else
	qDebug() << "preprocessing_page - initializePage - invalid document type";
    }


    const preprocessing_task_set_t& preprocessing_page::tasks() const
    {
      return tasks_;
    }


    bool preprocessing_page::validatePage()
    {
      tasks_ = widget_->tasks();
      return true;
    }



  } // end of namespace scribo::demo

} // end of namespace scribo
