#include <src/result_widget.hh>

namespace scribo
{

  namespace demo
  {


    result_widget::result_widget(QWidget *parent)
      : QWidget(parent)
    {
      text_ = new QTextEdit(this);
      QVBoxLayout *layout = new QVBoxLayout;
      layout->addWidget(text_);
      setLayout(layout);
    }

    void result_widget::load(const QString& filename)
    {
      QFile file(filename);
      if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	return;

      QTextStream in(&file);
      in.setCodec("UTF-8");
      while (!in.atEnd())
      {
	QString line = in.readLine();
	text_->append(line);
      }

    }

    void result_widget::reset()
    {
      text_->clear();
    }


  } // end of namespace scribo::demo

} // end of namespace scribo
