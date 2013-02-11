#ifndef CONFIGS_H
#define CONFIGS_H

#include <scribo/toolchain/internal/text_in_doc_preprocess_functor.hh>
#include <QSettings>
#include <QDir>

#include "region.h"

class Configs :
        public QSettings
{
    public:
        Configs() : QSettings("olena-scribo", "gui") {}

        static Configs * getInstance()
        { static Configs *conf = new Configs(); return conf; }

        inline bool preprocessingSubsample();
        inline void setPreprocessingSubsample(bool b);

        inline bool preprocessingRemoveBg();
        inline void setPreprocessingRemoveBg(bool b);

        inline bool preprocessingDeskew();
        inline void setPreprocessingDeskew(bool b);

        inline bool preprocessingRemoveNoise();
        inline void setPreprocessingRemoveNoise(bool b);

        inline int preprocessingBinAlgo();
        inline void setPreprocessingBinAlgo(int algo);

        inline int segmentationFindSeps();
        inline void setSegmentationFindSeps(int seps);

        inline bool ocrEnabled();
        inline void setOcrEnabled(bool b);

        inline QString ocrLanguage();
        inline void setOcrLanguage(const QString& lang);

        inline bool generalSaveXmlEnabled();
        inline void setGeneralSaveXmlEnabled(bool b);

        inline bool generalSaveXmlSameDir();
        inline void setGeneralSaveXmlSameDir(bool b);

        inline bool generalSaveXmlCustomDir();
        inline void setGeneralSaveXmlCustomDir(bool b);

        inline QString generalSaveXmlCustomDirPath();
        inline void setGeneralSaveXmlCustomDirPath(const QString& path);
};

inline bool Configs::preprocessingSubsample()
{ return value("preprocessing/subsample", false).toBool(); }

inline void Configs::setPreprocessingSubsample(bool b)
{ setValue("preprocessing/subsample", b); }

inline bool Configs::preprocessingRemoveBg()
{ return value("preprocessing/remove_bg", false).toBool(); }

inline void Configs::setPreprocessingRemoveBg(bool b)
{ setValue("preprocessing/remove_bg", b); }

inline bool Configs::preprocessingDeskew()
{ return value("preprocessing/deskew", false).toBool(); }

inline void Configs::setPreprocessingDeskew(bool b)
{ setValue("preprocessing/deskew", b); }

inline bool Configs::preprocessingRemoveNoise()
{ return value("preprocessing/remove_noise", true).toBool(); }

inline void Configs::setPreprocessingRemoveNoise(bool b)
{ setValue("preprocessing/remove_noise", b); }

inline int Configs::preprocessingBinAlgo()
{ return value("preprocessing/bin_algo", scribo::toolchain::internal::SauvolaMs).toInt(); }

inline void Configs::setPreprocessingBinAlgo(int algo)
{ setValue("preprocessing/bin_algo", algo); }

inline int Configs::segmentationFindSeps()
{ return value("segmentation/find_seps", Separator::Both).toInt(); }

inline void Configs::setSegmentationFindSeps(int seps)
{ setValue("segmentation/find_seps", seps); }

inline bool Configs::ocrEnabled()
{ return value("ocr/enabled", true).toBool(); }

inline void Configs::setOcrEnabled(bool b)
{ setValue("ocr/enabled", b); }

inline QString Configs::ocrLanguage()
{ return value("ocr/language", 0).toString(); }

inline void Configs::setOcrLanguage(const QString& lang)
{ setValue("ocr/language", lang); }

inline bool Configs::generalSaveXmlEnabled()
{ return value("general/save_xml/enabled", true).toBool(); }

inline void Configs::setGeneralSaveXmlEnabled(bool b)
{ setValue("general/save_xml/enabled", b); }

inline bool Configs::generalSaveXmlSameDir()
{ return value("general/save_xml/same_dir", true).toBool(); }

inline void Configs::setGeneralSaveXmlSameDir(bool b)
{ setValue("general/save_xml/same_dir", b); }

inline bool Configs::generalSaveXmlCustomDir()
{ return value("general/save_xml/custom_dir", false).toBool(); }

inline void Configs::setGeneralSaveXmlCustomDir(bool b)
{ setValue("general/save_xml/custom_dir", b); }

inline QString Configs::generalSaveXmlCustomDirPath()
{ return value("general/save_xml/custom_dir_path", QDir::tempPath()).toString(); }

inline void Configs::setGeneralSaveXmlCustomDirPath(const QString& path)
{ setValue("general/save_xml/custom_dir_path", path); }

#endif // CONFIGS_H
