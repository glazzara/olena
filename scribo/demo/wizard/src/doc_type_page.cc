#include <src/doc_type_page.hh>
#include <src/doc_type_widget.hh>

namespace scribo
{

  namespace demo
  {


    doc_type_page::doc_type_page(QWidget *parent)
      : QWizardPage(parent)
    {
      widget_ = new doc_type_widget(this);

      QVBoxLayout *layout = new QVBoxLayout;
      layout->addWidget(widget_);
      setLayout(layout);

      setSubTitle("Please choose the document type.");
      setTitle("Document type");

      registerField("doc_type.type_mixed_doc", widget_, "type_mixed_doc", SIGNAL(toggled(bool)));
      registerField("doc_type.type_picture", widget_, "type_picture", SIGNAL(toggled(bool)));
      registerField("doc_type.type_text_doc", widget_, "type_text_doc", SIGNAL(toggled(bool)));
    }


    void doc_type_page::cleanupPage()
    {
      widget_->reset();
    }


    void doc_type_page::initializePage()
    {
    }


    bool doc_type_page::isComplete()
    {
      return true;
    }


  } // end of namespace scribo::demo

} // end of namespace scribo
