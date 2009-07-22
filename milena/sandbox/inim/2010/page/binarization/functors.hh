#ifndef FUNCTORS_HH
# define FUNCTORS_HH

namespace binarization
{
  template<typename T>
  struct Square
  {
      Square();
      unsigned long long operator()(T val) const;
  };

  template<typename T>
  struct Identity
  {
      Identity();
      unsigned long long operator()(T val) const;
  };
}

# include "functors.hxx"

#endif /* !FUNCTORS_HH */
