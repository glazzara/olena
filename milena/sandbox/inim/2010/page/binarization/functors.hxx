#ifndef FUNCTORS_HXX
# define FUNCTORS_HXX

# include <functors.hxx>

namespace binarization
{
  template<typename T>
  Square<T>::Square()
  { }

  template<typename T>
  unsigned long long
  Square<T>::operator()(T val) const
  {
    return static_cast<unsigned long long>(val) * static_cast<unsigned long long>(val);
  }

  template<typename T>
  Identity<T>::Identity()
  { }

  template<typename T>
  unsigned long long
  Identity<T>::operator()(T val) const
  {
    return static_cast<unsigned long long>(val);
  }


}

#endif /* !FUNCTORS_HXX */
