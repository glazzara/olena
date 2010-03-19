#include <src/process_page.hh>
#include <src/process_widget.hh>
#include <src/preprocessing_task.hh>


namespace scribo
{

  namespace demo
  {


    process_page::process_page(QWidget *parent)
      : QWizardPage(parent)
    {
      widget_ = new process_widget(this);
      QVBoxLayout *layout = new QVBoxLayout;
      layout->addWidget(widget_);
      setLayout(layout);

      setSubTitle("Processing...");
      setTitle("Please wait while computing results.");

      connect(&runner_, SIGNAL(new_intermediate_result(const QImage&)),
	      widget_, SLOT(update_pixmap(const QImage&)));

      connect(&runner_, SIGNAL(new_progress_max_value(unsigned)),
	      widget_, SLOT(set_progress_max_value(unsigned)));

      connect(&runner_, SIGNAL(new_progress_label(const QString&)),
	      widget_, SLOT(update_progress_label(const QString&)));

      connect(&runner_, SIGNAL(progress(unsigned)),
	      widget_, SLOT(progress(unsigned)));
    }


    void process_page::cleanupPage()
    {
      widget_->reset();
      runner_.stop();
    }


    void process_page::initializePage()
    {
      runner_.start(field("load.input_dsp").value<QPixmap>().toImage(),
		    get_doc_type(),
		    field("preprocessing.tasks").value<preprocessing_task_set_t>());
    }


    Doc_Type process_page::get_doc_type()
    {
      if (field("doc_type.type_text_doc").toBool())
	return Text_Doc;
      else if (field("doc_type.type_picture").toBool())
	return Picture;
      else if (field("doc_type.type_mixed_doc").toBool())
	return Mixed_Doc;

      return Invalid_Doc;
    }


  } // end of namespace scribo::demo

} // end of namespace scribo
