#ifndef BACKGROUNDTEXTURE_H
#define BACKGROUNDTEXTURE_H

#include <QPixmap>

class BackgroundTexture : public QPixmap
{
    public:
        explicit BackgroundTexture(const QPixmap& );
};

#endif // BACKGROUNDTEXTURE_H
