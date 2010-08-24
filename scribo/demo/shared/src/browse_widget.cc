#include <shared/src/browse_widget.hh>

namespace scribo
{

  namespace demo
  {

    namespace shared
    {


      browse_widget::browse_widget(QWidget *parent)
	: QWidget(parent)
      {
	setupUi(this);

	QStringList filters;
	filters << tr("Any (*.*)");
	setDialogFilter(filters);

	setDialogFileMode(QFileDialog::ExistingFile);
	setDialogTitle(tr("Choose an existing file"));
	setDescription(tr("Choose a file:"));
      }

      void browse_widget::setDescription(const QString& desc)
      {
	description_->setText(desc);
      }

      QString browse_widget::path() const
      {
	return path_->text();
      }

      void browse_widget::reset()
      {
	path_->clear();
      }

      void browse_widget::setDialogTitle(const QString& title)
      {
	dialog_.setLabelText(QFileDialog::LookIn, title);
      }

      void browse_widget::setDialogFileMode(const QFileDialog::FileMode& filemode)
      {
	dialog_.setFileMode(filemode);
      }

      void browse_widget::setDialogFilter(const QStringList& filters)
      {
	dialog_.setNameFilters(filters);
      }


      void browse_widget::on_browseBtn__clicked()
      {
	if (dialog_.exec())
	{
	  path_->setText(dialog_.selectedFiles().at(0));
	  emit path_updated();
	}
      }


    } // end of namespace scribo::demo::shared

  } // end of namespace scribo::demo

} // end of namespace scribo
