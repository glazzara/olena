#include <src/crop_page.hh>

namespace scribo
{

  namespace demo
  {


    crop_page::crop_page(QWidget *parent)
      : QWizardPage(parent)
    {
      widget_ = new crop_widget(this);

      QVBoxLayout *layout = new QVBoxLayout;
      layout->addWidget(widget_);
      setLayout(layout);

      setTitle("Region of interest");
      setSubTitle("Select a region of interest");

    }


    void crop_page::cleanupPage()
    {
      setField("load.input_dsp",
               QPixmap(field("load.file_path").toString()));

    }


    void crop_page::initializePage()
    {
      widget_->load_image(field("load.input_dsp").value<QPixmap>());
    }


    bool crop_page::validatePage()
    {
      setField("load.input_dsp", widget_->image());
      return true;
    }


  } // end of namespace scribo::demo

} // end of namespace scribo
