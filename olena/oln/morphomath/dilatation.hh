#ifndef   	OLN_MORPHOMATH_DILATATION_HH_
# define   	OLN_MORPHOMATH_DILATATION_HH_

// Facade.

namespace impl
{

  /// Generic version

  template <typename I, typename W>
  I dilatation(const Image<I>& input)
  {
    max_<oln_value(I)> max;
    return apply(max, input);
  }

}


/// Facade.

template <typename I, typename W>
I erosion(const Image<I>& input)
{
  return impl::dilatation(exact(input));
}

#endif	    /* !OLN_MORPHOMATH_DILATATION_HH_ */
