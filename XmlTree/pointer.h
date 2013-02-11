#ifndef POINTER_H
#define POINTER_H

#include <QVariant>

template<typename T> class Pointer
{
    public:
        static inline QVariant toQVariant(T *pointer)
        { return QVariant::fromValue((void *)pointer); }

        static inline T *fromQVariant(const QVariant& variant)
        { return (T *)variant.value<void *>(); }

    private:
        Pointer() { }
};

#endif // POINTER_H
