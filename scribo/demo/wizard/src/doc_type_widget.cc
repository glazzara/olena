#include <src/doc_type_widget.hh>

namespace scribo
{

  namespace demo
  {


    doc_type_widget::doc_type_widget(QWidget *parent)
      : QWidget(parent)
    {
      setupUi(this);
    }

    void doc_type_widget::reset()
    {
      type_text_doc_->setChecked(true);
    }

    bool doc_type_widget::type_mixed_doc() const
    {
      return type_mixed_doc_->isChecked();
    }

    bool doc_type_widget::type_picture() const
    {
      return type_picture_->isChecked();
    }

    bool doc_type_widget::type_text_doc() const
    {
      return type_text_doc_->isChecked();
    }

  } // end of namespace scribo::demo

} // end of namespace scribo
