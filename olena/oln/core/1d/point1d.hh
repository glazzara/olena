// Copyright (C) 2001, 2002, 2003, 2004, 2006 EPITA Research and
// Development Laboratory
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

#ifndef OLN_CORE_1D_POINT1D_HH
# define OLN_CORE_1D_POINT1D_HH

# include <mlc/int.hh>
# include <oln/core/internal/point_nd.hh>


namespace oln
{

  /// Fwd decls.
  template <unsigned D> struct grid_;
  template <typename C> struct dpoint1d_;
  template <typename C> struct  point1d_;


  typedef grid_<1> grid1d;
  typedef dpoint1d_<int> dpoint1d;
  typedef  point1d_<int>  point1d;



  /* FIXME: Is this the right place for these functions (on types)?
     In particular, the function on dpoint1d should be near the
     definition of dpoint1d, not point1d's.  */
  /// Specializations of functions point and dpoint :
  /// \f$(n, coord) \rightarrow type\f$ for \f$n = 1\f$.
  /// \{
  template <typename C> struct  point_ <1, C> { typedef  point1d_<C> ret; };
  template <typename C> struct dpoint_ <1, C> { typedef dpoint1d_<C> ret; };
  /// \}



  /// Super type.
  template<typename C>
  struct set_super_type< point1d_<C> >
  {
    typedef internal::point_nd< point1d_<C> > ret;
  };


  /// Virtual types associated to oln::point1d_<C>.
  template <typename C>
  struct vtypes< point1d_<C> >
  {
    typedef grid1d        grid_type;
    typedef dpoint1d      dpoint_type;
    typedef C             coord_type;
    typedef mlc::uint_<1> dim_type;

    typedef mlc::uint_<0> index_comp_type;
  };


  /// General 1D point class.
  template <typename C>
  class point1d_ : public internal::point_nd< point1d_<C> >
  {
    typedef point1d_<C> self_t;
    typedef internal::point_nd<self_t> super_t;

    using super_t::v_;

  public:

    /// Ctor.
    point1d_();

    /// Ctor.
    point1d_(C index);

    /// Ctor.
    point1d_(const xtd::vec<1,C>& v);

    /// Dtor.
    ~point1d_()
    {
    }

    C  index() const;
    C& index();
  };



# ifndef OLN_INCLUDE_ONLY


  template <typename C>
  point1d_<C>::point1d_()
  {
  }

  template <typename C>
  point1d_<C>::point1d_(C index)
    : super_t (xtd::mk_vec(index))
  {
  }

  template <typename C>
  point1d_<C>::point1d_(const xtd::vec<1,C>& v)
    : super_t(v)
  {
  }

  template <typename C>
  C point1d_<C>::index() const { return v_[0]; }

  template <typename C>
  C& point1d_<C>::index()      { return v_[0]; }

# endif


} // end of namespace oln


#endif // ! OLN_CORE_1D_POINT1D_HH
