#ifndef MLN_CORE_CONCEPT_PSITE_HH
# define MLN_CORE_CONCEPT_PSITE_HH

/*! \file mln/core/concept/psite.hh
 * This file defines the concept of mln::Psite.
 */

# include <mln/core/concept/genpoint.hh>


namespace mln
{

  /*! Base class for implementation classes of the notion of "point
   *  site".
   *
   * A point site ("psite" for short) is an object that allows an
   * efficient access to data associated with a point.  A point site
   * is either a point or designates a point: regular points, deriving
   * from mln::Point, are point sites, yet some point sites are not
   * points.)  A point site has the behavior expected from every
   * point; see mln::GenPoint.
   *
   * When a point site is not a point, it is automatically convertible
   * to the point it designates.
   *
   *
   * Let us take the example of a 2D image encoded as an array of runs
   * of values.  With a point, a pair (row index, column index),
   * retrieving the corresponding pixel value would mean to browse the
   * array of runs to find the value location.  That would not be
   * efficient.  Conversely, a point site dedicated to this image
   * structure allows for value access in contant time; precisely the
   * proper point site is a pair (index of run, index within the run).
   */
  template <typename E>
  struct Psite : public Object<E>,
		 public GenPoint<E>
  {
    /*
    const point* pointer() const
    {
      return & (exact(this)->operator point());
    }
    */

  protected:
    Psite();
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  Psite<E>::Psite()
  {
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_PSITE_HH
