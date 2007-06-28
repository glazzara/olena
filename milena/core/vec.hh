#ifndef MLN_CORE_VEC_HH
# define MLN_CORE_VEC_HH

# include <core/concept/object.hh>


namespace mln
{

  template <unsigned n, typename T>
  struct vec : public Object< vec<n,T> >
  {
    enum { dim = n };
    typedef T coord;

    T& operator[](unsigned i)
    {
      assert(i < n);
      return coord_[i];
    }

    T operator[](unsigned i) const
    {
      assert(i < n);
      return coord_[i];
    }
    
  protected:
    T coord_[n];
  };
  
} // end of namespace mln


#endif // ! MLN_CORE_VEC_HH
