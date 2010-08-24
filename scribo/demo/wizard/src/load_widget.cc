#include <src/load_widget.hh>

namespace scribo
{

  namespace demo
  {


    load_widget::load_widget(QWidget *parent)
      : browse_widget(parent)
    {
      QGroupBox *group_box = new QGroupBox(tr("Options"), this);

      QVBoxLayout *layout = new QVBoxLayout(group_box);
      layout->setSizeConstraint(QLayout::SetMaximumSize);
      must_crop_ = new QCheckBox(tr("Select a region of interest"),
					  group_box);
      must_rotate_ = new QCheckBox(tr("Perform a manual rotation"),
					    group_box);

      layout->addWidget(must_crop_);
      layout->addWidget(must_rotate_);


      QSpacerItem*
	vspacer = new QSpacerItem(20, 40, QSizePolicy::Minimum,
				  QSizePolicy::Expanding);

      gridLayout_2->addWidget(group_box, 2, 1);
      gridLayout_2->addItem(vspacer, 3, 1, 1, 1);

      gridLayout_2->setSizeConstraint(QLayout::SetMaximumSize);
    }

    void load_widget::reset()
    {
      browse_widget::reset();
      must_crop_->setChecked(false);
      must_rotate_->setChecked(false);
    }

    bool load_widget::must_crop() const
    {
      return must_crop_->isChecked();
    }

    bool load_widget::must_rotate() const
    {
      return must_rotate_->isChecked();
    }


  } // end of namespace scribo::demo

} // end of namespace scribo
