#ifndef MLN_CORE_CONCEPT_WINDOW_HH
# define MLN_CORE_CONCEPT_WINDOW_HH

/*! \file mln/core/concept/window.hh
 * \brief Definition of the concept of mln::Window.
 */

# include <mln/core/concept/object.hh>


namespace mln
{

  /*! \brief Base class for implementation classes that are windows.
   *
   * \see mln::doc::Window for a complete documentation of this class
   * contents.
   */
  template <typename E>
  struct Window : public Object<E>
  {
    /*
      typedef qiter;
      typedef fwd_qiter;
      typedef bkd_qiter;

      bool is_empty() const;
      bool is_centered() const;
      bool is_symmetric() const;
    */

  protected:
    Window();
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  Window<E>::Window()
  {
    typedef mln_qiter(E) qiter;
    typedef mln_fwd_qiter(E) fwd_qiter;
    typedef mln_bkd_qiter(E) bkd_qiter;

    bool (E::*m1)() const = & E::is_empty;
    m1 = 0;
    bool (E::*m2)() const = & E::is_centered;
    m2 = 0;
    bool (E::*m3)() const = & E::is_symmetric;
    m3 = 0;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_WINDOW_HH
