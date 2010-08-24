// Copyright (C) 2010 EPITA Research and Development Laboratory
// (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.


# include <shared/src/image_viewer.hh>

namespace scribo
{

  namespace demo
  {

    namespace shared
    {

      image_viewer::image_viewer(QWidget *parent)
	: QWidget(parent), item_(0)
      {
	setupUi(this);

	connect(image_->verticalScrollBar(), SIGNAL(valueChanged(int)),
		this, SLOT(move_vertical_sliders(int)));
	connect(image_->horizontalScrollBar(), SIGNAL(valueChanged(int)),
		this, SLOT(move_horizontal_sliders(int)));

	setup_scene();

	slider->setMinimum(0);
	visible_slider(false);
      }



      image_viewer::~image_viewer()
      {

      }

      unsigned image_viewer::current_slice() const
      {
	return slider->value();
      }

      internal::interactiveScene * image_viewer::scene()
      {
	return static_cast<internal::interactiveScene *>(image_->scene());
      }

      const internal::interactiveScene * image_viewer::scene() const
      {
	return static_cast<const internal::interactiveScene *>(image_->scene());
      }

      // Private members

      void image_viewer::setup_scene()
      {
	QGraphicsScene *scene = new internal::interactiveScene();
	image_->setScene(scene);
	image_->show();
      }

      void image_viewer::draw_image(const mln::image2d<dsp_data_t>& ima)
      {
	QImage
	  qima = mln::convert::implementation::to_qimage_qt_rgb32_nocopy(ima);
	QPixmap pixmap = QPixmap::fromImage(qima);

	if (item_ != 0)
	{
	  image_->scene()->removeItem(item_);
	  delete item_;
	}

	item_ = image_->scene()->addPixmap(pixmap);
	image_->scene()->setSceneRect(item_->boundingRect());

	if (pixmap.width() > image_->maximumViewportSize().width())
	  image_->fitInView(item_->boundingRect(), Qt::KeepAspectRatio);

	zoomLabel->setEnabled(true);
	zoomIn->setEnabled(true);
	zoomOut->setEnabled(true);
	zoomFixed->setEnabled(true);
	zoomOriginal->setEnabled(true);

	item_->translate(ima.domain().pmin().col(), ima.domain().pmin().row());

	image_->setSceneRect(item_->sceneBoundingRect());
      }


      void image_viewer::resize_image(const QRectF& rect)
      {
	if (item_ != 0)
	  image_->fitInView(rect, Qt::KeepAspectRatio);
      }

      void image_viewer::resizeEvent(QResizeEvent * event)
      {
	if (item_ != 0)
	  resize_image(item_->boundingRect());
	event->ignore();
      }


      void image_viewer::keyPressEvent(QKeyEvent *event)
      {
	if (event->text() == QString("+"))
	{
	  on_zoomIn_clicked();
	  event->accept();
	}
	else if (event->text() == QString("-"))
	{
	  on_zoomOut_clicked();
	  event->accept();
	}
	else if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
	{
	  if (event->modifiers() == Qt::ControlModifier)
	    on_zoomFixed_clicked();
	  else
	    on_zoomOriginal_clicked();
	  event->accept();
	}
	else
	  event->ignore();
      }


      // Private slots

      void image_viewer::visible_slider(bool b)
      {
	slider->setVisible(b);
	label_2->setVisible(b);
	frame_nb->setVisible(b);
      }

      void image_viewer::move_vertical_sliders(int value)
      {
	image_->verticalScrollBar()->setValue(value);
      }


      void image_viewer::move_horizontal_sliders(int value)
      {
	image_->horizontalScrollBar()->setValue(value);
      }


      void image_viewer::on_slider_valueChanged(int sli)
      {
	frame_nb->setText(QString("%1/%2").arg(sli).arg(slider->maximum()));
	emit slider_valueChanged(sli);
      }

      void image_viewer::update_image(const mln::image2d<dsp_data_t>& ima)
      {
	draw_image(ima);
      }

      void image_viewer::on_zoomIn_clicked()
      {
	image_->scale(1.2, 1.2);
      }

      void image_viewer::on_zoomOut_clicked()
      {
	image_->scale(1 / 1.2, 1 / 1.2);
      }

      void image_viewer::on_zoomFixed_clicked()
      {
	if (item_ != 0)
	  resize_image(item_->boundingRect());
      }

      void image_viewer::on_zoomOriginal_clicked()
      {
	if (item_ != 0)
	  resize_image(image_->viewport()->geometry());
      }


      void image_viewer::clear()
      {
	visible_slider(false);
	if (item_ != 0 && item_->scene() == image_->scene())
	{
	  image_->scene()->removeItem(item_);
	  delete item_;
	  item_ = 0;
	}
      }


      // Public slots

      void image_viewer::set_image_layer_count(unsigned nslis)
      {
	visible_slider(nslis > 1);

	slider->setMaximum(nslis - 1);

	unsigned sli = current_slice();
	if (current_slice() >= nslis)
	  sli = 0;

	slider->setSliderPosition(sli);
	on_slider_valueChanged(sli);
      }

    } // end of namespace scribo::demo::shared

  } // end of namespace scribo::demo

} // end of namespace scribo
