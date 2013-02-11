#include "configs.h"

Configs::Configs()
  : QSettings("Olena-Scribo", "viewer")
{
}

Configs * Configs::getInstance()
{
    static Configs * conf = new Configs();
    return conf;
}

// Preprocessing configs.
bool Configs::preprocessingSubsample()
{
    return value("preprocessing/subsample", false).toBool();
}

void Configs::setPreprocessingSubsample(bool b)
{
    setValue("preprocessing/subsample", b);
}



bool Configs::preprocessingRemoveBg()
{
    return value("preprocessing/remove_bg", false).toBool();
}

void Configs::setPreprocessingRemoveBg(bool b)
{
    setValue("preprocessing/remove_bg", b);
}



bool Configs::preprocessingDeskew()
{
    return value("preprocessing/deskew", false).toBool();
}

void Configs::setPreprocessingDeskew(bool b)
{
    setValue("preprocessing/deskew", b);
}



bool Configs::preprocessingRemoveNoise()
{
    return value("preprocessing/remove_noise", true).toBool();
}

void Configs::setPreprocessingRemoveNoise(bool b)
{
    setValue("preprocessing/remove_noise", b);
}



int Configs::preprocessingBinAlgo()
{
    return value("preprocessing/bin_algo", scribo::toolchain::internal::SauvolaMs).toInt();
}

void Configs::setPreprocessingBinAlgo(int algo)
{
    setValue("preprocessing/bin_algo", algo);
}





// Segmentation configs.
int Configs::segmentationFindSeps()
{
    return value("segmentation/find_seps", Separator::Both).toInt();
}

void Configs::setSegmentationFindSeps(int seps)
{
    setValue("segmentation/find_seps", seps);
}





// OCR configs.
bool Configs::ocrEnabled()
{
    return value("ocr/enabled", true).toBool();
}

void Configs::setOcrEnabled(bool b)
{
    setValue("ocr/enabled", b);
}



QString Configs::ocrLanguage()
{
    return value("ocr/language", 0).toString();
}

void Configs::setOcrLanguage(const QString &lang)
{
    setValue("ocr/language", lang);
}





// OCR configs.
bool Configs::generalSaveXmlEnabled()
{
    return value("general/save_xml/enabled", true).toBool();
}

void Configs::setGeneralSaveXmlEnabled(bool b)
{
    setValue("general/save_xml/enabled", b);
}



bool Configs::generalSaveXmlSameDir()
{
    return value("general/save_xml/same_dir", true).toBool();
}

void Configs::setGeneralSaveXmlSameDir(bool b)
{
    setValue("general/save_xml/same_dir", b);
}



bool Configs::generalSaveXmlCustomDir()
{
    return value("general/save_xml/custom_dir", false).toBool();
}

void Configs::setGeneralSaveXmlCustomDir(bool b)
{
    setValue("general/save_xml/custom_dir", b);
}



QString Configs::generalSaveXmlCustomDirPath()
{
    return value("general/save_xml/custom_dir_path", QDir::tempPath()).toString();
}

void Configs::setGeneralSaveXmlCustomDirPath(const QString &path)
{
    setValue("general/save_xml/custom_dir_path", path);
}
