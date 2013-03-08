// Copyright (C) 2007, 2008, 2009, 2011, 2012 EPITA Research and
// Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_CORE_CONCEPT_NEIGHBORHOOD_HH
# define MLN_CORE_CONCEPT_NEIGHBORHOOD_HH

/// \file
///
/// Definition of the concept of mln::Neighborhood.

# include <mln/core/concept/window.hh>
# include <mln/trait/windows.hh>



# define mln_is_simple_neighborhood(N)  mln_is_simple_window(mln_window(N))




namespace mln
{


  // Forward declaration.
  template <typename E> struct Neighborhood;


  /*!
    \internal
    \brief Neighborhood category flag type.
  */
  template <>
  struct Neighborhood<void>
  {
    typedef Object<void> super;
  };


  /// \brief Base class for implementation classes that are neighborhoods.
  ///
  /// \see mln::doc::Neighborhood for a complete documentation of this
  /// class contents.

  template <typename E>
  struct Neighborhood : public Object<E>
  {
    typedef Neighborhood<void> category;

    /*
      typedef     niter;
      typedef fwd_niter;
      typedef bkd_niter;

      typedef window;
      either
        const window& win() const;
      or
        window win() const;
    */

  protected:
    Neighborhood();
  };



  template <typename L, typename R>
  bool operator==(const Neighborhood<L>& lhs, const Neighborhood<R>& rhs);


  template <typename N>
  std::ostream&
  operator<<(std::ostream&ostr, const Neighborhood<N>& nbh);


  // FIXME: Move as a method of Image?
  template <typename I, typename N>
  util::array<int>
  offsets_wrt(const Image<I>& ima, const Neighborhood<N>& nbh);

  template <typename I, typename N>
  util::array<int>
  positive_offsets_wrt(const Image<I>& ima, const Neighborhood<N>& nbh);

  template <typename I, typename N>
  util::array<int>
  negative_offsets_wrt(const Image<I>& ima, const Neighborhood<N>& nbh);



# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  inline
  Neighborhood<E>::Neighborhood()
  {
    typedef mln_niter(E)         niter;
    typedef mln_fwd_niter(E) fwd_niter;
    typedef mln_bkd_niter(E) bkd_niter;

    typedef mln_window(E) window;
    bool m = (& E::win) == (& E::win);
    (void) m;
    bool (E::*m2)() const = &E::is_valid;
    (void) m2;

# if 0
    /* FIXME: Disabled, as win() can either return a const reference
       or a copy of the window (see documentation above).  Hence the
       simpler, relaxed check above (m0).  */
    const window& (E::*m1)() const = & E::win;
    (void) m1;
# endif
  }


  template <typename L, typename R>
  inline
  bool operator==(const Neighborhood<L>& lhs, const Neighborhood<R>& rhs)
  {
    return exact(lhs).win() == exact(rhs).win();
  }

  template <typename N>
  inline
  std::ostream&
  operator<<(std::ostream&ostr, const Neighborhood<N>& nbh)
  {
    return ostr << exact(nbh).win();
  }

  template <typename I, typename N>
  inline
  util::array<int>
  offsets_wrt(const Image<I>& ima_, const Neighborhood<N>& nbh_)
  {
    mln_is_simple_neighborhood(N)::check();

    const I& ima = exact(ima_);
    const N& nbh = exact(nbh_);
    mln_precondition(ima.is_valid());
    mln_precondition(nbh.is_valid());

    return offsets_wrt(ima, nbh.win());
  }

  template <typename I, typename N>
  util::array<int>
  positive_offsets_wrt(const Image<I>& ima_, const Neighborhood<N>& nbh_)
  {
    mln_is_simple_neighborhood(N)::check();

    const I& ima = exact(ima_);
    const N& nbh = exact(nbh_);
    mln_precondition(ima.is_valid());
    mln_precondition(nbh.is_valid());

    return positive_offsets_wrt(ima, nbh.win());
  }

  template <typename I, typename N>
  util::array<int>
  negative_offsets_wrt(const Image<I>& ima_, const Neighborhood<N>& nbh_)
  {
    mln_is_simple_neighborhood(N)::check();

    const I& ima = exact(ima_);
    const N& nbh = exact(nbh_);
    mln_precondition(ima.is_valid());
    mln_precondition(nbh.is_valid());

    return negative_offsets_wrt(ima, nbh.win());
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_NEIGHBORHOOD_HH
