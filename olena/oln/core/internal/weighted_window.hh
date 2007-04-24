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

#ifndef OLN_CORE_INTERNAL_WEIGHTED_WINDOW_HH
# define OLN_CORE_INTERNAL_WEIGHTED_WINDOW_HH

# include <oln/core/internal/weighted_window_base.hh>
# include <oln/core/internal/weighted_dpoints_impl.hh>
// # include <oln/core/gen/dpoints_piter.hh>


namespace oln
{


  // Fwd decl.
  namespace internal { template <typename Exact> class weighted_window_; }


  // Super type.
  template <typename Exact>
  struct super_trait_< internal::weighted_window_<Exact> >
  {
    typedef internal::weighted_window_base_<Exact> ret;
  };


  // Virtual types.
  template <typename Exact>
  struct vtypes< internal::weighted_window_<Exact> >
  {
    typedef stc_deferred(point) point__;
    typedef stc::final< oln_dpoint(point__) > dpoint;
//     typedef stc::final< dpoints_fwd_piter_<point__> > fwd_qiter;
//     typedef stc::final< dpoints_bkd_piter_<point__> > bkd_qiter;
  };

  namespace internal
  {

    /// Base implementation class for regular weighted_window classes.

    template <typename Exact>
    class weighted_window_ : public weighted_window_base_<Exact>,
			     public weighted_dpoints_impl_< stc_deferred(weight),
							    stc_deferred(dpoint) >
    {
      typedef weighted_window_base_<Exact> super;
    public:

      stc_using(weight);
      stc_using(point);
      typedef oln_dpoint(point) dpoint;

      Exact& take(const weight& w, const dpoint& dp);
      Exact& impl_take(const weight& w, const dpoint& dp);

      template <unsigned n>
      void fill_with(const weight (&values)[n]);

    protected:
      weighted_window_();

    }; // end of class oln::internal::weighted_window_<Exact>


# ifndef OLN_INCLUDE_ONLY

    template <typename Exact>
    weighted_window_<Exact>::weighted_window_()
    {
    }

    template <typename Exact>
    Exact&
    weighted_window_<Exact>::take(const typename weighted_window_<Exact>::weight& w,
				  const typename weighted_window_<Exact>::dpoint& dp)
    {
      return exact(this)->impl_take(w, dp);
    }

    template <typename Exact>
    Exact&
    weighted_window_<Exact>::impl_take(const typename weighted_window_<Exact>::weight& w,
				       const typename weighted_window_<Exact>::dpoint& dp)
    {
      this->take_(w, dp); // from weighted_dpoints_impl_.
      return exact(*this);
    }

    template <typename Exact>
    template <unsigned n>
    void
    weighted_window_<Exact>::fill_with(const weight (&values)[n])
    {
      precondition(this->size() == 0);
      exact(this)->impl_fill_with(values);
    }

  } // end of namespace oln::internal

# endif // ! OLN_INCLUDE_ONLY

} // end of namespace oln


#endif // ! OLN_CORE_INTERNAL_WEIGHTED_WINDOW_HH
