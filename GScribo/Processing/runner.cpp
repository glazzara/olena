#include "runner.h"

using namespace mln;
using namespace scribo::toolchain::internal;

static QString get_pathto(const QString& file, const QString localdirsuffix = QString())
{
    QFile f(SCRIBO_LOCAL_DEMODIR "/" + localdirsuffix + "/" + file);
    if (f.exists())
        return SCRIBO_LOCAL_DEMODIR "/" + localdirsuffix;
    f.setFileName(SCRIBO_PREFIX_BINDIR "/" + file);
    if (f.exists())
        return SCRIBO_PREFIX_BINDIR;

  return "";
}

Runner::Runner(QObject *parent)
  : QThread(parent)
{
    moveToThread(this);
}

void Runner::run()
{
    switch(mode_)
    {
        default:
        case Demat:
        {
            image2d<value::rgb8> ima;
            io::magick::load(ima, args_.at(0).toUtf8().constData());
            image2d<bool> bin_ima = preprocess(ima);
            process(ima, bin_ima);
            emit finished();
        }
        break;

        case Export:
            export_as();
            break;
    }
}

void Runner::stop()
{
    terminate();
}

// Demat related stuff
void Runner::start_demat(const QString& filename)
{
    args_.clear();
    args_ << filename;
    mode_ = Demat;
    QThread::start();
}


image2d<bool> Runner::preprocess(const image2d<value::rgb8>& ima)
{
    emit new_step("Preprocessing");
    Preprocess f;

    // Load config.
    Configs * const conf = Configs::getInstance();

    connect(&f, SIGNAL(progress()), this, SIGNAL(progress()));
    connect(&f, SIGNAL(newProgressLabel(QString)),
            this, SIGNAL(new_progress_label(const QString&)));

    f.enable_subsample = conf->preprocessingSubsample();
    f.enable_fg_extraction = conf->preprocessingRemoveBg();
    f.enable_deskew = conf->preprocessingDeskew();
    f.enable_denoising = conf->preprocessingRemoveNoise();

    f.binarization_algo = static_cast<Binarization_Algo>(conf->preprocessingBinAlgo());

    emit new_progress_max_value(f.nsteps());

    // Perform preprocessing.
    f(ima);

    return f.output;
}


void Runner::process(const image2d<value::rgb8>& original_ima,
                       const image2d<bool>& processed_ima)
{
    emit new_step("Page segmentation");

    Process f(args_.at(0).toUtf8().constData());

    connect(&f, SIGNAL(progress()), this, SIGNAL(progress()));
    connect(&f, SIGNAL(newProgressLabel(QString)),
            this, SIGNAL(new_progress_label(QString)));
    connect(&f, SIGNAL(xmlSaved(QString)),
            this, SIGNAL(xml_saved(const QString&)));

    // Load config.
    Configs * const conf = Configs::getInstance();

    Separator::FindSeparator find_seps = static_cast<Separator::FindSeparator>(conf->segmentationFindSeps());
    f.enable_line_seps = (find_seps == Separator::Lines
                          || find_seps == Separator::Both);
    f.enable_whitespace_seps = (find_seps == Separator::Whitespaces
                                || find_seps == Separator::Both);
    f.enable_ocr = conf->ocrEnabled();
    f.ocr_language = conf->ocrLanguage().toAscii().data();
    f.xml_format = scribo::io::xml::PageExtended;


    f.save_doc_as_xml = true;
    QFileInfo file(args_.at(0));
    QString output_dir = QDir::tempPath();
    if (conf->generalSaveXmlEnabled())
    {
        if (conf->generalSaveXmlSameDir())
            output_dir = file.absolutePath();
        else if (conf->generalSaveXmlCustomDir())
            output_dir = conf->generalSaveXmlCustomDirPath();

        QDir dir(output_dir);
        if (!dir.exists() && !dir.mkpath(output_dir))
            output_dir = QDir::tempPath();
    }
    QString filename = (output_dir  + "/" + file.baseName() + "_gui.xml");
    f.output_file = filename.toUtf8().constData();
    emit new_progress_max_value(f.nsteps());

    // Perform text detection.
    f(original_ima, processed_ima);
    emit xml_saved(filename);
}



// Export related stuff
void Runner::start_export(const QString& imgfile,
                          const QString& xmlfile, const QString& outfile)
{
    args_.clear();
    args_ << imgfile << xmlfile << outfile;
    mode_ = Export;

    QThread::start();
}


void Runner::export_as()
{
    emit new_step("Exporting document...");
    emit new_progress_max_value(2);

    // Checking output format
    QFileInfo f(args_.at(2));

    QString pathto_xml2doc = get_pathto("scribo-xml2doc", "xml2doc");

    if (pathto_xml2doc.isEmpty())
    {
        QMessageBox::critical(0, "Fatal error", "Cannot export! Cannot find scribo-xml2doc program!");
        return;
    }

    emit progress();

    int rvalue = 0;
    if (f.suffix() == "pdf")
    {
        emit new_step("Exporting as PDF...");
        rvalue = system(QString("%1/scribo-xml2doc --pdf %2 %3 %4")
                        .arg(pathto_xml2doc).arg(args_.at(1)).arg(args_.at(0))
                        .arg(args_.at(2)).toAscii().constData());
    }
    else if (f.suffix() == "html" || f.suffix() == "htm")
    {
        emit new_step("Exporting as HTML...");
        rvalue = system(QString("%1/scribo-xml2doc --html %2 %3 %4")
                        .arg(pathto_xml2doc).arg(args_.at(1)).arg(args_.at(0))
                        .arg(args_.at(2)).toAscii().constData());
    }
    else
        QMessageBox::critical(0, "Fatal error", "Cannot export! Invalid output format!");

    if (rvalue != 0)
        QMessageBox::critical(0, "Fatal error", "Cannot export! Return value is not 0!");
}
