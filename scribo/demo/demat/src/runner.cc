#include <src/runner.hh>

#include <shared/src/to_mln_image.hh>
#include <src/preprocessing_task.hh>
#include <src/doc_type.hh>

#include <mln/convert/to_qimage.hh>
#include <mln/convert/to_qimage_nocopy.hh>
#include <mln/fun/v2v/rgb_to_int_u.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/rgb8.hh>
#include <mln/data/convert.hh>
#include <mln/data/paste_without_localization.hh>
#include <mln/io/pbm/save.hh>
#include <mln/io/magick/load.hh>
#include <mln/subsampling/antialiased.hh>

#include <scribo/preprocessing/split_bg_fg.hh>
#include <scribo/preprocessing/denoise.hh>
#include <scribo/preprocessing/homogeneous_contrast.hh>
#include <scribo/preprocessing/deskew.hh>
#include <scribo/binarization/sauvola.hh>
#include <scribo/binarization/sauvola_ms.hh>
#include <scribo/binarization/sauvola_ms_split.hh>
#include <scribo/binarization/global_threshold_auto.hh>

#include <mln/logical/not.hh>

namespace scribo
{

  namespace demo
  {

    QString basedir_ = "";


    runner::runner(QObject *parent)
      : QThread(parent)
    {
      process_.moveToThread(this);
    }


    void runner::start(const QString& filename,
		       const mln::image2d<mln::value::rgb8>& input,
		       const Doc_Type& doc_type,
		       const preprocessing_task_set_t& tasks,
		       const process_args& args)
    {
      filename_ = filename;
      doc_type_ = doc_type;
      tasks_ = tasks;
      args_ = args;
      input_ = input;

      QThread::start();
    }


    void runner::run()
    {
      // Notification of the number of tasks.
      emit new_progress_max_value(tasks_.size() + 2);

      qDebug() << "progress steps : " << tasks_.size() + 2;

      image2d<value::rgb8> ima = load();

      preprocess(ima);

      process();
    }


    image2d<value::rgb8> runner::load()
    {
      emit new_progress_label("Loading image");

      image2d<value::rgb8> output(input_.nrows(), input_.ncols(),
				  input_.border());
      data::paste_without_localization(input_, output);

//      io::magick::load(output, filename_.toStdString());

      emit progress(1);

      return output;
    }


    void runner::preprocess(const image2d<value::rgb8>& ima)
    {

      image2d<value::rgb8> tmp_color = duplicate(ima);


      //
      //==========
      // Subsample
      //==========
      if (tasks_.contains(ReduceSize))
      {
	emit new_progress_label("Subsampling input image");
	tmp_color = mln::subsampling::antialiased(tmp_color,
						  find_best_scale(tmp_color));
	emit progress(1);
      }


      //==================
      // Remove background
      //==================
      if (tasks_.contains(RemoveBg))
      {
	std::cout << "Foreground extraction" << std::endl;
	emit new_progress_label("Foreground Extraction");

	tmp_color = scribo::preprocessing::split_bg_fg(tmp_color, 10000, 32).second();

	emit progress(1);
      }


      //=============================================
      // Convert to grayscale image (always happens).
      //=============================================
      emit new_progress_label("Convert to gray-scale image");
      image2d<value::int_u8>
	intensity_ima = data::transform(tmp_color,
					mln::fun::v2v::rgb_to_int_u<8>());
      emit progress(1);


      //=======
      // Deskew
      //=======
      if (tasks_.contains(Unskew))
      {
	std::cout << "Deskew" << std::endl;
	emit new_progress_label("Deskew");

	intensity_ima = scribo::preprocessing::deskew(intensity_ima);

	emit progress(1);
      }


      //=============================
      // Improve contrast homogeneity
      //=============================
      if (tasks_.contains(ImproveContrast))
      {
	std::cout << "Improve contrast homogeneity" << std::endl;
	emit new_progress_label("Improve contrast homogeneity");

	intensity_ima = arith::revert(preprocessing::homogeneous_contrast(intensity_ima, 75));

	emit progress(1);
      }


      std::cout << intensity_ima.domain() << std::endl;
      io::pgm::save(intensity_ima, "intensity_ima.pgm");

      //==============================
      // Binarization (always happens)
      //==============================
      image2d<bool> out_bool;
      emit new_progress_label("Binarization");

      if (tasks_.contains(BinarizationSauvola))
      {
	// FIXME: sauvola should not negate the image.
	std::cout << "Binarization Sauvola" << std::endl;
	out_bool = binarization::sauvola(intensity_ima);
      }
      else if (tasks_.contains(BinarizationSauvolaMs))
      {
	// FIXME: sauvola should not negate the image.
	std::cout << "Binarization Sauvola_ms" << std::endl;
	out_bool = binarization::sauvola_ms(intensity_ima, 51, 2);
      }
      else if (tasks_.contains(BinarizationSauvolaMsSplit))
      {
	// FIXME: sauvola should not negate the image.
	// FIXME: WARNING: work directly on the color image -> it is
	// NOT deskewed nor contrast improved.
	std::cout << "Binarization Sauvola_ms_split" << std::endl;
	out_bool = binarization::sauvola_ms_split(tmp_color, 51, 2, 2);
      }
      else if (tasks_.contains(BinarizationSimple))
      {
	std::cout << "Binarization Simple" << std::endl;
	out_bool = scribo::binarization::global_threshold_auto(intensity_ima);
      }
      else
      {
	std::cout << "Binary image conversion" << std::endl;
	out_bool = data::convert(bool(), intensity_ima);
      }

      emit progress(1);


      // FIXME: remove!
      logical::not_inplace(out_bool);


      //========
      // Denoise
      //========
      if (tasks_.contains(RemoveNoise))
      {
	std::cout << "Remove noise" << std::endl;
	emit new_progress_label("Remove noise");

	out_bool = preprocessing::denoise(out_bool, c8(), 2, 2);

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
	  emit new_progress_label("Finding text in document...");
	  qDebug() << "Running pbm_text_in_doc";
	  args << "/tmp/tmp.pbm" << "/tmp/out.txt";

	  // Denoise.
	  if (! tasks_.contains(RemoveNoise))
	    args << "0";
	  else
	  {
	    args << "1";
	    emit progress(1); // Consider denoising as done even though it is performed later.
	  }

	  args << "/tmp/";

	  qDebug() << "Running:"
		   <<  basedir_ + "/pbm_text_in_doc"
		   << args;

	  if (process_.execute(basedir_ + "/pbm_text_in_doc", args))
	  {
	    qDebug() << "Error while running pbm_text_in_doc.";
	    return;
	  }
	  break;

	case Picture:
	  emit new_progress_label("Finding text in picture...");
	  qDebug() << "Running text_in_picture";
	  args << "/tmp/tmp.pbm" << "/tmp/out.ppm" << "1" << "1" << "1";
	  if (process_.execute(basedir_ + "/text_in_picture", args))
	  {
	    qDebug() << "Error while running text_in_picture.";
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

      emit finished();
      qDebug() << "Done";
    }


    void runner::stop()
    {
      process_.kill();
      terminate();
    }


    template <typename V>
    unsigned runner::find_best_scale(const image2d<V>& ima)
    {
      if (ima.nrows() > 2500
	  && ima.nrows() < 5000
	  && ima.ncols() > 2500
	  && ima.ncols() < 5000)
	return 2;

      if (ima.nrows() > 5000
	  && ima.ncols() > 5000)
	return 3;

      return 1;
    }

  } // end of namespace scribo::demo

} // end of namespace scribo
