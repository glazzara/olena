#include <src/crop_widget.hh>

namespace scribo
{

  namespace demo
  {


    crop_widget::crop_widget(QWidget *parent)
      : QWidget(parent)
    {
      setupUi(this);
    }


    void crop_widget::load_image(const QPixmap& pixmap)
    {
      viewer_->draw_image(pixmap);
      viewer_->set_selection_enabled(true);
    }


    const QRectF& crop_widget::crop_area() const
    {
      return viewer_->selection();
    }


    QPixmap crop_widget::image() const
    {
      return viewer_->image().copy(crop_area().toRect());
    }

  } // end of namespace scribo::demo

} // end of namespace scribo
