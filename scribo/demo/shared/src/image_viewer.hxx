#ifndef SCRIBO_DEMO_SHARED_SRC_IMAGE_VIEWER_HH
# error "This file cannot be included directly."
#endif


namespace scribo
{

  namespace demo
  {

    namespace shared
    {


      template <typename V>
      void image_viewer::draw_image(const mln::image2d<V>& ima)
      {
	QImage
	  qima = mln::convert::to_qimage_nocopy(ima);
	QPixmap pixmap = QPixmap::fromImage(qima);

	draw_image(pixmap, QPoint(ima.domain().pmin().col(),
				  ima.domain().pmin().row()));
      }

    } // end of namespace scribo::demo::shared

  } // end of namespace scribo::demo

} // end of namespace scribo
