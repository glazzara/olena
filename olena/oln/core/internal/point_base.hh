// Copyright (C) 2001, 2002, 2003, 2004, 2006, 2007 EPITA Research and
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

#ifndef OLN_CORE_INTERNAL_POINT_BASE_HH
# define OLN_CORE_INTERNAL_POINT_BASE_HH

# include <xtd/vec.hh>
# include <mlc/int.hh>
# include <oln/core/concept/point.hh>



namespace oln
{

  // Fwd decl.
  namespace internal { template <typename Exact> class point_base_; }

  template <typename Exact>
  struct super_trait_< internal::point_base_<Exact> >
  {
    // super_trait_ set to mlc::none, since Point is a concept
    // (interface).
    typedef mlc::none ret;
  };

  template <typename Exact>
  struct vtypes< internal::point_base_<Exact> >
  {
    typedef stc::abstract grid;
    typedef stc::abstract coord;
    typedef stc::abstract dpoint;

    typedef stc_deferred(grid) grid__;

    // Final:
    // FIXME: Improve scoop-alt to get rid of this difference.
# ifndef OLENA_USE_SCOOP_ALT
    typedef stc::final< oln_dim(grid__) > dim;
# else
    typedef stc::final< stc_deferred_from(grid__, dim) > dim;
# endif
    typedef stc::final< stc::is<Point>  > category;
  };


  namespace internal
  {

    template <typename Exact>
    class point_base_ : public Point<Exact>
    {
      typedef Point<Exact> super;

    public:

      stc_using(grid);
      stc_using(coord);
      stc_using(dim);
      stc_using(dpoint);

      using super::n;

      coord  operator[](unsigned i) const;
      coord& operator[](unsigned i);

      bool impl_op_equal_(const Exact& rhs) const;

      bool impl_op_less_(const Exact& rhs) const;

      Exact& impl_op_plus_equal_(const dpoint& rhs);   // Definition is in dpoint_base.hh

      Exact& impl_op_minus_equal_(const dpoint& rhs);  // Likewise

      dpoint impl_op_minus_(const Exact& rhs) const;   // Likewise

      typedef xtd::vec<n, coord> vec_t;
      const vec_t& vec() const;
      vec_t& vec();
      void set_all(const coord& c);

      // FIXME: Add 'static const Exact& zero();'

    protected:
      point_base_();
      point_base_(coord val);

      vec_t v_;
    };


# ifndef OLN_INCLUDE_ONLY

    template <typename Exact>
    typename point_base_<Exact>::coord
    point_base_<Exact>::operator[](unsigned i) const
    {
      assert(i < n);
      return v_[i];
    }
    
    template <typename Exact>
    typename point_base_<Exact>::coord&
    point_base_<Exact>::operator[](unsigned i)
    {
      assert(i < n);
      return v_[i];
    }

    template <typename Exact>
    bool point_base_<Exact>::impl_op_equal_(const Exact& rhs) const
    {
      return v_ == rhs.v_;
    }

    template <typename Exact>
    bool point_base_<Exact>::impl_op_less_(const Exact& rhs) const
    {
      return xtd::lexi_less(v_, rhs.v_);
    }

    template <typename Exact>
    const typename point_base_<Exact>::vec_t&
    point_base_<Exact>::vec() const
    {
      return v_;
    }

    template <typename Exact>
    typename point_base_<Exact>::vec_t&
    point_base_<Exact>::vec()
    {
      return v_;
    }

    template <typename Exact>
    void
    point_base_<Exact>::set_all(const typename point_base_<Exact>::coord& c)
    {
      v_.set_all(c);
    }

    template <typename Exact>
    point_base_<Exact>::point_base_()
    {
    }

# endif // ! OLN_INCLUDE_ONLY

  } // end of namespace oln::internal




  template <typename Exact>
  std::ostream& operator<<(std::ostream& ostr, const internal::point_base_<Exact>& p);



# ifndef OLN_INCLUDE_ONLY

  template <typename Exact>
  std::ostream& operator<<(std::ostream& ostr, const internal::point_base_<Exact>& p)
  {
    ostr << p.vec();
    return ostr;
  }

# endif // ! OLN_INCLUDE_ONLY


} // end of namespace oln


#endif // ! OLN_CORE_INTERNAL_POINT_BASE_HH
