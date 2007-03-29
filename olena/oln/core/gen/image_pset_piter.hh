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

#ifndef OLN_CORE_GEN_IMAGE_PSET_PITER_HH
# define OLN_CORE_GEN_IMAGE_PSET_PITER_HH

# include <oln/core/internal/iterator_on_points_base.hh>
# include <oln/core/concept/point_set.hh>
# include <oln/core/concept/image.hh>


namespace oln
{

  // Fwd decls.

  template <typename Ps> struct image_pset_fwd_piter_;
  template <typename Ps> struct image_pset_bkd_piter_;


  // Super types.

  template <typename Ps>
  struct super_trait_< image_pset_fwd_piter_<Ps> >
  {
    typedef image_pset_fwd_piter_<Ps> current__;
    typedef internal::iterator_on_points_base_<current__> ret;
  };

  template <typename Ps>
  struct super_trait_< image_pset_bkd_piter_<Ps> >
  {
    typedef image_pset_bkd_piter_<Ps> current__;
    typedef internal::iterator_on_points_base_<current__> ret;
  };


  // Virtual types.

  template <typename Ps>
  struct vtypes< image_pset_fwd_piter_<Ps> >
  {
    typedef oln_point(Ps) point;
  };

  template <typename Ps>
  struct vtypes< image_pset_bkd_piter_<Ps> >
  {
    typedef oln_point(Ps) point;
  };


  /// Class image_pset_fwd_piter_<Ps>.

  template <typename Ps>
  class image_pset_fwd_piter_ : public internal::iterator_on_points_base_< image_pset_fwd_piter_<Ps> >,
				private mlc::assert_< mlc_is_a(Ps, Point_Set) >
  {
    typedef image_pset_fwd_piter_<Ps> current;
    typedef internal::iterator_on_points_base_<current> super;
  public:

    stc_using(point);

    image_pset_fwd_piter_();

    template <typename I>
    image_pset_fwd_piter_(const Image<I>& ima);

    void impl_start();
    void impl_next();
    void impl_invalidate();
    bool impl_is_valid() const;
    point impl_to_point() const;
    const point* impl_point_adr() const;

  private:
    typename Ps::fwd_piter i_;
  };


  /// Class image_pset_bkd_piter_<Ps>.

  template <typename Ps>
  class image_pset_bkd_piter_ : public internal::iterator_on_points_base_< image_pset_bkd_piter_<Ps> >,
				private mlc::assert_< mlc_is_a(Ps, Point_Set) >
  {
    typedef image_pset_fwd_piter_<Ps> current;
    typedef internal::iterator_on_points_base_<current> super;
  public:

    stc_using(point);

    image_pset_bkd_piter_();

    template <typename I>
    image_pset_bkd_piter_(const Image<I>& ima);

    void impl_start();
    void impl_next();
    void impl_invalidate();
    bool impl_is_valid() const;
    point impl_to_point() const;
    const point* impl_point_adr() const;

  private:
    typename Ps::bkd_piter i_;
  };




# ifndef OLN_INCLUDE_ONLY


  // --------------------   image_pset_fwd_piter_<Ps>


  template <typename Ps>
  image_pset_fwd_piter_<Ps>::image_pset_fwd_piter_()
  {
  }

  template <typename Ps>
  template <typename I>
  image_pset_fwd_piter_<Ps>::image_pset_fwd_piter_(const Image<I>& ima)
    : i_(ima.points())
  {
  }

  template <typename Ps>
  void image_pset_fwd_piter_<Ps>::impl_start()
  {
    i_.start();
  }

  template <typename Ps>
  void image_pset_fwd_piter_<Ps>::impl_next()
  {
    i_.next();
  }

  template <typename Ps>
  void image_pset_fwd_piter_<Ps>::impl_invalidate()
  {
    i_.next();
  }

  template <typename Ps>
  bool image_pset_fwd_piter_<Ps>::impl_is_valid() const
  {
    return i_.is_valid();
  }

  template <typename Ps>
  typename image_pset_fwd_piter_<Ps>::point
  image_pset_fwd_piter_<Ps>::impl_to_point() const
  {
    return i_;
  }

  template <typename Ps>
  const typename image_pset_fwd_piter_<Ps>::point*
  image_pset_fwd_piter_<Ps>::impl_point_adr() const
  {
    return i_.point_adr();
  }


  // --------------------   image_pset_bkd_piter_<Ps>


  template <typename Ps>
  image_pset_bkd_piter_<Ps>::image_pset_bkd_piter_()
  {
  }

  template <typename Ps>
  template <typename I>
  image_pset_bkd_piter_<Ps>::image_pset_bkd_piter_(const Image<I>& ima)
    : i_(ima.points())
  {
  }

  template <typename Ps>
  void image_pset_bkd_piter_<Ps>::impl_start()
  {
    i_.start();
  }

  template <typename Ps>
  void image_pset_bkd_piter_<Ps>::impl_next()
  {
    i_.next();
  }

  template <typename Ps>
  void image_pset_bkd_piter_<Ps>::impl_invalidate()
  {
    i_.invalidate();
  }

  template <typename Ps>
  bool image_pset_bkd_piter_<Ps>::impl_is_valid() const
  {
    return i_.is_valid();
  }

  template <typename Ps>
  typename image_pset_bkd_piter_<Ps>::point
  image_pset_bkd_piter_<Ps>::impl_to_point() const
  {
    return i_;
  }

  template <typename Ps>
  const typename image_pset_bkd_piter_<Ps>::point*
  image_pset_bkd_piter_<Ps>::impl_point_adr() const
  {
    return i_.point_adr();
  }

# endif // ! OLN_INCLUDE_ONLY

} // end of namespace oln


#endif // ! OLN_CORE_GEN_IMAGE_PSET_PITER_HH
