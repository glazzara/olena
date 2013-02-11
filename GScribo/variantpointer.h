#ifndef VARIANTPOINTER_H
#define VARIANTPOINTER_H

#include <QVariant>

template<typename T> class VariantPointer
{
    public:
        static inline QVariant toQVariant(T *pointer)
        { return QVariant::fromValue(static_cast<void *>(pointer)); }

        static inline T *fromQVariant(const QVariant& variant)
        { return static_cast<T *>(variant.value<void *>()); }

    private:
        VariantPointer() { }
};

#endif // VARIANTPOINTER_H
