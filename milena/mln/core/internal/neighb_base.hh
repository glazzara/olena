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

#ifndef MLN_CORE_INTERNAL_NEIGHB_BASE_HH
# define MLN_CORE_INTERNAL_NEIGHB_BASE_HH

/// \file
///
/// Base implementation for neighborhood classes.

# include <mln/core/internal/neighborhood_base.hh>
# include <mln/core/internal/site_relative_iterator_base.hh>
# include <mln/core/internal/neighb_niter_impl.hh>


namespace mln
{

  namespace internal
  {


    /*!
      \internal
      \brief Adapter class from window to neighborhood.

      \tparam W The underlying window type.
      \tparam E The exact neighborhood type.
    */
    template <typename W, typename E>
    class neighb_base
      : public internal::neighborhood_base< W, E >,
	private mlc_is_a(W, Window)::check_t
    {
      public:

      /// Constructor without argument.
      neighb_base();

      /// Constructor from a window \p win.
      neighb_base(const W& win);

      /// Get the corresponding window.
      const W& win() const;

      /// Change the corresponding window.
      void change_window(const W& new_win);


      /// \cond INTERNAL_API
      /// \brief Hook to the window.
      W& hook_win_();
      /// \endcond

      private:

      W win_;
    };


# ifndef MLN_INCLUDE_ONLY

    // neighb<W,E>

    template <typename W, typename E>
    inline
    neighb_base<W,E>::neighb_base()
    {
    }

    template <typename W, typename E>
    inline
    neighb_base<W,E>::neighb_base(const W& win)
    {
      change_window(win);
    }

    template <typename W, typename E>
    inline
    const W&
    neighb_base<W,E>::win() const
    {
      return win_;
    }

    template <typename W, typename E>
    inline
    void
    neighb_base<W,E>::change_window(const W& new_win)
    {
      mln_precondition(new_win.is_neighbable_());
      win_ = new_win;
    }

    template <typename W, typename E>
    inline
    W&
    neighb_base<W,E>::hook_win_()
    {
      return win_;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_NEIGHB_BASE_HH
