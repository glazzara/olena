#include <src/runner.hh>

#include <shared/src/to_mln_image.hh>
#include <src/preprocessing_task.hh>
#include <src/doc_type.hh>

#include <mln/convert/to_qimage.hh>
#include <mln/convert/to_qimage_nocopy.hh>
#include <mln/fun/v2v/rgb_to_int_u.hh>
#include <mln/value/int_u8.hh>
#include <mln/data/convert.hh>
#include <mln/io/pbm/save.hh>

#include <scribo/preprocessing/split_bg_fg.hh>
#include <scribo/preprocessing/denoise.hh>
#include <scribo/preprocessing/homogeneous_contrast.hh>
#include <scribo/preprocessing/unskew.hh>
#include <scribo/binarization/sauvola.hh>
#include <scribo/binarization/sauvola_ms.hh>
#include <scribo/binarization/simple.hh>

#include <mln/logical/not.hh>

namespace scribo
{

  namespace demo
  {

    QString basedir_ = "/home/lazzara/build/scribo/src";

    runner::runner(QObject *parent)
      : QThread(parent)
    {
      process_.moveToThread(this);
    }


    void runner::start(const QImage& input,
		       const Doc_Type& doc_type,
		       const preprocessing_task_set_t& tasks)
    {
      input_ = input;
      doc_type_ = doc_type;
      tasks_ = tasks;

      QThread::start();
    }


    void runner::run()
    {
      // Notification of the number of tasks.
      emit new_progress_max_value(tasks_.size() + 2);

      image2d<value::qt::rgb32> ima = load();

      preprocess(ima);

      process();
    }


    image2d<value::qt::rgb32> runner::load()
    {
      emit new_progress_label("Loading image");

      // FIXME: specify the right top left corner.
      // This is relevant when the image is cropped...
      image2d<value::qt::rgb32>
	output = shared::to_mln_image(input_, QPoint(0, 0));

      QImage q_ima = convert::to_qimage_nocopy(output);
      q_ima.save("/tmp/tmp.ppm");

//      io::magick::load(output, filename_.toStdString());

      emit progress(1);
      emit new_intermediate_result(input_);

      return output;
    }


    void runner::preprocess(const image2d<value::qt::rgb32>& ima)
    {

      image2d<value::rgb8> tmp_color;
      io::ppm::load(tmp_color, "/tmp/tmp.ppm");

      image2d<bool> out_bool;


      //==================
      // Remove background
      //==================
      if (tasks_.contains(RemoveBg))
      {
	std::cout << "Foreground extraction" << std::endl;
	emit new_progress_label("Foreground Extraction");

	QStringList args;
	args << "/tmp/tmp.ppm" << "10000" << "32"
	     << "/tmp/tmp.ppm" << "/dev/null";
	if (process_.execute(basedir_ + "/preprocessing/split_bg_fg", args))
	{
	  qDebug() << "Error running split_bg_fg";
	  return;
	}

	emit progress(1);
	emit new_intermediate_result(mln::convert::to_qimage(tmp_color));
      }


      //=============================================
      // Convert to grayscale image (always happens).
      //=============================================
      emit new_progress_label("Convert to gray-scale image");
      image2d<value::int_u8>
	intensity_ima = data::transform(tmp_color,
					mln::fun::v2v::rgb_to_int_u<8>());
      emit progress(1);


      //=============================
      // Improve contrast homogeneity
      //=============================
      if (tasks_.contains(ImproveContrast))
      {
	std::cout << "Improve contrast homogeneity" << std::endl;
	emit new_progress_label("Improve contrast homogeneity");

	intensity_ima = preprocessing::homogeneous_contrast(intensity_ima, 75);

 	image2d<value::qt::rgb32>
 	  tmp_color32 = data::convert(value::qt::rgb32(), out_bool);
 	emit new_intermediate_result(convert::to_qimage(tmp_color32));
	emit progress(1);
      }


      //==============================
      // Binarization (always happens)
      //==============================
      emit new_progress_label("Binarization");

      if (tasks_.contains(BinarizationSauvola))
      {
	// FIXME: sauvola should not negate the image.
	std::cout << "Binarization Sauvola" << std::endl;
	out_bool = logical::not_(binarization::sauvola(intensity_ima));
      }
      else if (tasks_.contains(BinarizationSauvolaMs))
      {
	// FIXME: sauvola should not negate the image.
	std::cout << "Binarization Sauvola_ms" << std::endl;
	out_bool = logical::not_(binarization::sauvola_ms(intensity_ima,
							 51, 2, 67));
      }
      else if (tasks_.contains(BinarizationSimple))
      {
	std::cout << "Binarization Simple" << std::endl;
	out_bool = scribo::binarization::simple(intensity_ima);
      }
      else
      {
	std::cout << "Binary image conversion" << std::endl;
	out_bool = data::convert(bool(), intensity_ima);
      }

      image2d<value::qt::rgb32>
 	tmp_color32 = data::convert(value::qt::rgb32(), out_bool);
      emit new_intermediate_result(convert::to_qimage(tmp_color32));
      emit progress(1);


      //========
      // Denoise
      //========
      if (tasks_.contains(RemoveNoise))
      {
	std::cout << "Remove noise" << std::endl;
	emit new_progress_label("Remove noise");

	out_bool = preprocessing::denoise(out_bool, c8(), 2, 2);

 	image2d<value::qt::rgb32>
 	  tmp_color32 = data::convert(value::qt::rgb32(), out_bool);
 	emit new_intermediate_result(convert::to_qimage(tmp_color32));
	emit progress(1);
      }


      //=======
      // Unskew
      //=======
      if (tasks_.contains(Unskew))
      {
	std::cout << "Unskew" << std::endl;
	emit new_progress_label("Unskew");

	out_bool = scribo::preprocessing::unskew(out_bool).first();

 	image2d<value::qt::rgb32>
 	  tmp_color32 = data::convert(value::qt::rgb32(), out_bool);
 	emit new_intermediate_result(convert::to_qimage(tmp_color32));
	emit progress(1);
      }


      //=========================
      // Save preprocessed image.
      //=========================
      io::pbm::save(out_bool, "/tmp/tmp.pbm");
    }


    void runner::process()
    {
      QStringList args;


      // FIXME: require binaries to ask for a filename for bboxes
      // output files.
      switch(doc_type_)
      {
	case Text_Doc:
	  qDebug() << "Running text_in_article_pbm";
	  args << "/tmp/tmp.pbm" << "/tmp/out.ppm";
	  if (process_.execute(basedir_ + "/text_in_article_pbm", args))
	  {
	    qDebug() << "Error while running text_in_article_pbm.";
	    return;
	  }
	  break;

	case Picture:
	  qDebug() << "Running text_in_photo_pbm_fast";
	  args << "/tmp/tmp.pbm" << "/tmp/out.ppm" << "1" << "1" << "1";
	  if (process_.execute(basedir_ + "/text_in_photo_pbm_fast", args))
	  {
	    qDebug() << "Error while running text_in_photo_pbm_fast.";
	    return;
	  }
	  break;

	case Mixed_Doc:
	  qDebug() << "Running pbm_text_in_mixed_doc";
	  if (process_.execute(basedir_ + "/pbm_text_in_mixed_doc", args))
	  {
	    qDebug() << "Error while running pbm_text_in_mixed_doc.";
	    return;
	  }
	  break;

	default:
	case Invalid_Doc:
	  qDebug() << "runner - Invalid doc type...";
	  return;
      }

      process_.waitForFinished(-1);

      emit progress(1);
      emit new_intermediate_result(QImage("/tmp/out.ppm"));
      qDebug() << "Done";
    }


    void runner::stop()
    {
      process_.kill();
      terminate();
    }

  } // end of namespace scribo::demo

} // end of namespace scribo
