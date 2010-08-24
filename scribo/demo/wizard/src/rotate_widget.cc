#include <src/rotate_widget.hh>

namespace scribo
{

  namespace demo
  {


    rotate_widget::rotate_widget(QWidget *parent)
      : QWidget(parent)
    {
      setupUi(this);
    }


    void rotate_widget::load_image(const QPixmap& pixmap)
    {
      viewer_->draw_image(pixmap);
      viewer_->set_rotation_enabled(true);
    }


    const QRectF& rotate_widget::rotate_area() const
    {
      return viewer_->selection();
    }


    QPixmap rotate_widget::image() const
    {
      return viewer_->image();
    }


  } // end of namespace scribo::demo

} // end of namespace scribo
