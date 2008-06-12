// Copyright (C) 2007 EPITA Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_CORE_WINDOW_HH
# define MLN_CORE_WINDOW_HH

/*! \file mln/core/window.hh
 *
 * \brief Definition of the generic window class mln::window.
 *
 * \todo Make naming coherent: we have window (without '_') but
 * point_, neighb_, etc.
 */

# include <mln/core/internal/window_base.hh>
# include <mln/core/internal/basic_window_impl.hh>
# include <mln/core/dpoint.hh>
# include <mln/core/box.hh>

# include <mln/convert/to_dpoint.hh>
# include <mln/geom/sym.hh>
# include <mln/metal/is_a.hh>


namespace mln
{

  /*! \brief Generic window class.
   *
   * This type of window is just like a set of delta-points.  The
   * parameter is \c D, type of delta-point.
   */
  template <typename D>
  class window : public internal::window_base< D, window<D> >,
		 public internal::basic_window_impl< D, window<D> >
  {
  public:

    /*! \brief Constructor without argument.
     *
     * The constructed window is empty. 
     */
    window();


    /*! \brief Test if the window is centered.
     *
     * \return True if the delta-point 0 belongs to the window.
     */
    bool is_centered() const;

    /*! \brief Test if the window is symmetric.
     */
    bool is_symmetric() const;


    /// Apply a central symmetry to the target window.
    window<D>& sym();

  protected:
    
    box_<mln_point(D)> b_;
  };


  // FIXME: Doc!
  template <typename D>
  std::ostream& operator<<(std::ostream& ostr, const window<D>& win);



# ifndef MLN_INCLUDE_ONLY

  // window<D>

  template <typename D>
  inline
  window<D>::window()
  {
    // FIXME HERE: Was: mln::metal::is_a<D, Dpoint>::check();
    // mln::metal::is_a<D, Delta_Point_Site>::check();
  }

  template <typename D>
  inline
  bool window<D>::is_symmetric() const
  {
    return geom::sym(*this) == *this;
  }

  template <typename D>
  inline
  bool window<D>::is_centered() const
  {
    static const D origin = all_to(0);
    return this->dps_.has(origin); // FIXME: Use literal::origin.
  }

  template <typename D>
  inline
  window<D>&
  window<D>::sym()
  {
    window<D> tmp;
    const unsigned n = this->ndpoints();
    for (unsigned i = 0; i < n; ++i)
      tmp.insert(- this->dp(i));
    *this = tmp;
    return *this;
  }

  template <typename D>
  std::ostream& operator<<(std::ostream& ostr, const window<D>& win)
  {
    return ostr << win.dps_hook();
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


# include <mln/core/dpoints_piter.hh>


#endif // ! MLN_CORE_WINDOW_HH
