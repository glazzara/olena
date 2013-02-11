#ifndef ICON_H
#define ICON_H

#include <QIcon>

class Icon :
        QIcon
{
    public:
        explicit Icon(const QPixmap& pixmap);
        QString path() const;
        QPixmap * pixmap();
        void setPath(QString path);
        void setPixmap(QPixmap *pixmap);

    private:
        QString filename;
        QPixmap *pix;
};

#endif // ICON_H
