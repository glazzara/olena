// Copyright (C) 2009, 2012, 2013 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_CORE_MIXED_NEIGHB_HH
# define MLN_CORE_MIXED_NEIGHB_HH

/// \file
///
/// Definition of a window-to-neighborhood adapter.
///
/// Works for windows and neighborhoods having the elements' site set
/// different from the center's site set.
///
///
/// \todo See if the impl of from_to is fine.  What about removing the
/// origin?  etc.

# include <mln/core/neighb.hh>
# include <mln/core/internal/neighb_base.hh>
# include <mln/core/internal/neighb_niter_base.hh>
# include <mln/core/internal/site_relative_iterator_base.hh>
# include <mln/core/internal/neighb_niter_impl.hh>


namespace mln
{

  // Forward declarations.
  template <typename W> class mixed_neighb_fwd_niter;
  template <typename W> class mixed_neighb_bkd_niter;


  /// Adapter class from window to neighborhood.

  template <typename W>
  class mixed_neighb
    : public internal::neighb_base<W,mixed_neighb<W> >,
      private mlc_is_a(W, Window)::check_t
  {
    typedef internal::neighb_base<W,mixed_neighb<W> > super_;

  public:

    /// Forward site iterator associated type.
    typedef mixed_neighb_fwd_niter<W> fwd_niter;

    /// Backward site iterator associated type.
    typedef mixed_neighb_bkd_niter<W> bkd_niter;

    /// Site iterator associated type.
    typedef fwd_niter niter;


    /// Constructor without argument.
    mixed_neighb();

    /// Constructor from a window \p win.
    mixed_neighb(const W& win);

  };


  /// \cond INTERNAL_API
  /// Conversion: mixed_neighb<W> -> W
  template <typename W>
  void
  from_to_(const mln::mixed_neighb<W>& from, W& to);

  /// Conversion: mixed_neighb<W> -> W
  template <typename W>
  void
  from_to_(const W& from, mln::mixed_neighb<W>& to);
  /// \endcond


  // mixed_neighb_fwd_niter<W>

  template <typename W>
  class mixed_neighb_fwd_niter
    : public internal::neighb_niter_base<W,mixed_neighb<W>,
					 mln_fwd_qiter(W),neighb_fwd_niter<W> >
  {
    typedef
      internal::neighb_niter_base<W,mixed_neighb<W>,
				  mln_fwd_qiter(W),neighb_fwd_niter<W> > super_;

  public:
    mixed_neighb_fwd_niter();

    template <typename P>
    mixed_neighb_fwd_niter(const mixed_neighb<W>& nbh,
			   const mln_target(W)& nbh_site_set,
			   const P& c);

  };



  // mixed_neighb_bkd_niter<W,E>

  template <typename W>
  class mixed_neighb_bkd_niter
    : public internal::neighb_niter_base<W,mixed_neighb<W>,
					 mln_bkd_qiter(W),neighb_bkd_niter<W> >
  {
    typedef
      internal::neighb_niter_base<W,mixed_neighb<W>,
				  mln_bkd_qiter(W),neighb_bkd_niter<W> > super_;

  public:
    mixed_neighb_bkd_niter();

    template <typename P>
    mixed_neighb_bkd_niter(const mixed_neighb<W>& nbh,
			   const mln_target(W)& nbh_site_set,
			   const P& c);

  };



# ifndef MLN_INCLUDE_ONLY

  // mixed_neighb<W>

  template <typename W>
  inline
  mixed_neighb<W>::mixed_neighb()
  {
  }

  template <typename W>
  inline
  mixed_neighb<W>::mixed_neighb(const W& win)
    : super_(win)
  {
  }


  // Conversion

  template <typename W>
  void
  from_to_(const mln::mixed_neighb<W>& from, W& to)
  {
    to = from.win();
  }

  template <typename W>
  void
  from_to_(const W& from, mln::mixed_neighb<W>& to)
  {
    to.change_window(from);
  }


  // mixed_neighb_fwd_niter<W>

  template <typename W>
  inline
  mixed_neighb_fwd_niter<W>::mixed_neighb_fwd_niter()
  {
  }

  template <typename W>
  template <typename P>
  inline
  mixed_neighb_fwd_niter<W>::mixed_neighb_fwd_niter(const mixed_neighb<W>& nbh,
						    const mln_target(W)& nbh_site_set,
						    const P& c)
    : super_(nbh, c)
  {
    this->i_.init_(nbh.win(), nbh_site_set, c);
  }



  // mixed_neighb_bkd_niter<W>

  template <typename W>
  inline
  mixed_neighb_bkd_niter<W>::mixed_neighb_bkd_niter()
  {
  }

  template <typename W>
  template <typename P>
  inline
  mixed_neighb_bkd_niter<W>::mixed_neighb_bkd_niter(const mixed_neighb<W>& nbh,
						    const mln_target(W)& nbh_site_set,
						    const P& c)
    : super_(nbh, c)
  {
    this->i_.init_(nbh.win(), nbh_site_set, c);
  }


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_MIXED_NEIGHB_HH
