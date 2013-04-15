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

#ifndef MLN_CORE_INTERNAL_NEIGHB_NITER_BASE_HH
# define MLN_CORE_INTERNAL_NEIGHB_NITER_BASE_HH

/// \file
///
/// Base implementation for neighborhood iterators.

# include <mln/core/internal/site_relative_iterator_base.hh>
# include <mln/core/internal/neighb_niter_impl.hh>

namespace mln
{

  namespace internal
  {

    /*!
      \brief Base implementation for neighborhood iterators.
    */
    template <typename W, typename N, typename I, typename E>
    class neighb_niter_base
      : public internal::site_relative_iterator_base< N,
						      E,
						      mln_psite(N) >,
        public internal::neighb_niter_impl<W,E>
    {
    public:

      /// Constructor without argument.
      neighb_niter_base();

      template <typename P>
      neighb_niter_base(const N& nbh, const P& c);

      /// \cond INTERNAL_API
      /// Test the iterator validity.
      bool is_valid_() const;

      /// Invalidate the iterator.
      void invalidate_();

      /// Start an iteration.
      void do_start_();

      /// Go to the next point.
      void do_next_();

      /// Compute the current psite.
      mln_psite(W) compute_p_() const;
      /// \endcond
    protected:

      I i_;
    };



# ifndef MLN_INCLUDE_ONLY


    // neighb_niter_base<W,N,I,E>

    template <typename W, typename N, typename I, typename E>
    inline
    neighb_niter_base<W,N,I,E>::neighb_niter_base()
    {
    }

    template <typename W, typename N, typename I, typename E>
    template <typename P>
    inline
    neighb_niter_base<W,N,I,E>::neighb_niter_base(const N& nbh, const P& c)
    {
      this->change_target(nbh);
      this->center_at(c);

    }

    template <typename W, typename N, typename I, typename E>
    inline
    bool
    neighb_niter_base<W,N,I,E>::is_valid_() const
    {
      return i_.is_valid();
    }

    template <typename W, typename N, typename I, typename E>
    inline
    void
    neighb_niter_base<W,N,I,E>::invalidate_()
    {
      i_.invalidate();
    }

    template <typename W, typename N, typename I, typename E>
    inline
    void
    neighb_niter_base<W,N,I,E>::do_start_()
    {
      i_.start();
    }

    template <typename W, typename N, typename I, typename E>
    inline
    void
    neighb_niter_base<W,N,I,E>::do_next_()
    {
      i_.next();
    }

    template <typename W, typename N, typename I, typename E>
    inline
    mln_psite(W)
    neighb_niter_base<W,N,I,E>::compute_p_() const
    {
      return i_.compute_p_();
    }


# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace mln::internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_NEIGHB_NITER_BASE_HH
