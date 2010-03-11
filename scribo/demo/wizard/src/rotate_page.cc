#include <src/rotate_page.hh>

namespace scribo
{

  namespace demo
  {


    rotate_page::rotate_page(QWidget *parent)
      : QWizardPage(parent)
    {
      widget_ = new rotate_widget(this);

      QVBoxLayout *layout = new QVBoxLayout;
      layout->addWidget(widget_);
      setLayout(layout);

      setTitle("Rotation.");
      setSubTitle("Rotate the input image using the buttons above.");

    }


    void rotate_page::cleanupPage()
    {
    }


    void rotate_page::initializePage()
    {
      widget_->load_image(field("load.input_dsp").value<QPixmap>());
    }


    bool rotate_page::isComplete()
    {
      setField("load.input_dsp", widget_->image());
      return true;
    }


  } // end of namespace scribo::demo

} // end of namespace scribo
