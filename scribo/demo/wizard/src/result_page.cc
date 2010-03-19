#include <src/result_page.hh>
#include <src/result_widget.hh>


namespace scribo
{

  namespace demo
  {


    result_page::result_page(QWidget *parent)
      : QWizardPage(parent)
    {
      widget_ = new result_widget(this);
      QVBoxLayout *layout = new QVBoxLayout;
      layout->addWidget(widget_);
      setLayout(layout);

      setTitle("Result");
      setSubTitle("Copy and paste the recognized text.");

      connect(parent, SIGNAL(customButtonClicked(int)),
	      parent, SLOT(restart()));
    }


    void result_page::cleanupPage()
    {
      widget_->reset();
    }


    void result_page::initializePage()
    {
      setButtonText(QWizard::CustomButton1, "restart");
      widget_->load("/tmp/out.txt");
    }


  } // end of namespace scribo::demo

} // end of namespace scribo
