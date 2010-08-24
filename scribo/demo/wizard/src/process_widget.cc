#include <src/process_widget.hh>

namespace scribo
{

  namespace demo
  {


    process_widget::process_widget(QWidget *parent)
      : QWidget(parent)
    {
      view_ = new QGraphicsView(this);
      QVBoxLayout *layout = new QVBoxLayout;
      layout->addWidget(view_);
      setLayout(layout);

      QVBoxLayout *progress_layout = new QVBoxLayout;
      progress_bar_ = new QProgressBar(this);
      progress_bar_->setMinimum(0);
      progress_label_ = new QLabel("Processing...");
      progress_layout->addWidget(progress_bar_);
      progress_layout->addWidget(progress_label_);

      progress_dialog_ = new QWidget();
      progress_dialog_->setLayout(progress_layout);

      view_->setScene(new QGraphicsScene());
      progress_ = view_->scene()->addWidget(progress_dialog_);
      progress_->setZValue(1);

      image_ = view_->scene()->addPixmap(QPixmap());
    }

    void process_widget::reset()
    {
      progress_bar_->show();
      progress_bar_->setValue(0);
      progress_bar_->setMaximum(0);
      image_->setPixmap(QPixmap());
    }

    void process_widget::set_progress_max_value(unsigned max)
    {
      progress_bar_->setMaximum(max);
    }

    void process_widget::update_pixmap(const QImage& image)
    {
      image_->setPixmap(QPixmap::fromImage(image));
    }

    void process_widget::progress(unsigned i)
    {
      progress_bar_->setValue(progress_bar_->value() + i);

      if (progress_bar_->value() == progress_bar_->maximum())
	progress_->hide();
    }

    void process_widget::update_progress_label(const QString& msg)
    {
      progress_label_->setText(msg);
    }

    void process_widget::resizeEvent(QResizeEvent* event)
    {
      QPoint tmp(view_->geometry().center().x()
		 - progress_->boundingRect().width() / 2,
		 view_->geometry().center().y()
		 - progress_->boundingRect().height() / 2);

      progress_->setPos(view_->mapToScene(tmp));

      event->ignore();
    }

  } // end of namespace scribo::demo

} // end of namespace scribo
