#ifndef CONFIGS_H
#define CONFIGS_H

#include <QSettings>
#include <QDir>

class Configs :
        public QSettings
{
    public:
        Configs();
        static Configs * getInstance();
        bool preprocessingSubsample();
        void setPreprocessingSubsample(bool b);
        bool preprocessingRemoveBg();
        void setPreprocessingRemoveBg(bool b);
        bool preprocessingDeskew();
        void setPreprocessingDeskew(bool b);
        bool preprocessingRemoveNoise();
        void setPreprocessingRemoveNoise(bool b);
        int preprocessingBinAlgo();
        void setPreprocessingBinAlgo(int algo);
        int segmentationFindSeps();
        void setSegmentationFindSeps(int seps);
        bool ocrEnabled();
        void setOcrEnabled(bool b);
        QString ocrLanguage();
        void setOcrLanguage(const QString& lang);
        bool generalSaveXmlEnabled();
        void setGeneralSaveXmlEnabled(bool b);
        bool generalSaveXmlSameDir();
        void setGeneralSaveXmlSameDir(bool b);
        bool generalSaveXmlCustomDir();
        void setGeneralSaveXmlCustomDir(bool b);
        QString generalSaveXmlCustomDirPath();
        void setGeneralSaveXmlCustomDirPath(const QString& path);
};

#endif // CONFIGS_H
