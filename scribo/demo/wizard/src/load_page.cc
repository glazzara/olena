#include <src/load_page.hh>

namespace scribo
{

  namespace demo
  {


    load_page::load_page(QWidget *parent)
      : QWizardPage(parent)
    {
      widget_ = new load_widget(this);

      QStringList filters;
      filters << tr("PNG image (*.png)")
	      << tr("JPEG image (*.jpg *.jpeg *.jpe)")
	      << tr("PNM image (*.pnm *.pbm *.pgm *.ppm)");
      widget_->setDialogFilter(filters);


      QVBoxLayout *layout = new QVBoxLayout;
      layout->addWidget(widget_);
      setLayout(layout);

      setSubTitle("Choose a file to load");
      setTitle("Image loading");

      registerField("load.file_path*", widget_,
		    "path", SIGNAL(path_updated()));

      registerField("load.must_crop", widget_, "must_crop");
      registerField("load.must_rotate", widget_, "must_rotate");

      registerField("load.input_dsp", this, "input_dsp");

    }


    void load_page::cleanupPage()
    {
      widget_->reset();
    }


    void load_page::initializePage()
    {
    }


    bool load_page::validatePage()
    {
      input_dsp_ = QPixmap(widget_->path());
      return true;
    }


    const QPixmap& load_page::input_dsp() const
    {
      return input_dsp_;
    }

    void load_page::set_input_dsp(const QPixmap& pixmap)
    {
      input_dsp_ = pixmap;
    }

  } // end of namespace scribo::demo

} // end of namespace scribo
