// Copyright (C) 2007, 2008, 2009, 2012, 2013 EPITA Research and
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

#ifndef MLN_CORE_INTERNAL_CLASSICAL_WINDOW_BASE_HH
# define MLN_CORE_INTERNAL_CLASSICAL_WINDOW_BASE_HH

/*! \file
 *
 * \brief Definition of a base class for classes based on a set of dpoints.
 *
 * \todo Remove the .vect() method.
 *
 * \todo Add a test that overridden delta_() and win.delta() give the
 * same result.
 */

# include <mln/core/window.hh>
# include <mln/core/dpsites_piter.hh>



namespace mln
{

  namespace internal
  {

    /*!
      \brief Base class for classes based on a set of dpoints.
     */
    template <typename D, typename E>
    class classical_window_base : public window_base<D, E>
    {
    public:


      /// Regular window associated type.
      typedef window<D> regular;


      /// Forward site iterator associated type.
      typedef dpsites_fwd_piter<E> fwd_qiter;

      /// Backward site iterator associated type.
      typedef dpsites_fwd_piter<E> bkd_qiter;

      /// Site iterator associated type.
      typedef fwd_qiter qiter;


      /// Give the number of delta-points.
      unsigned size() const;

      /// Test if the window is empty (null size; no delta-point).
      bool is_empty() const;


      /// Test if the window is centered; return true.
      bool is_centered() const;

      /// Test if the window is symmetric; return true.
      bool is_symmetric() const;

      /// Apply a central symmetry to the target window; a no-op here.
      void sym();


      /// Give the maximum coordinate gap between the window center
      /// and a window point.
      unsigned delta() const;


      /// Test if the delta-point \p dp belongs to the window.
      bool has(const D& dp) const;

      /// Give the \p i-th delta-point.
      const D& dp(unsigned i) const;

      /// Give the vector of delta-points.
      const std::vector<D>& vect() const;

      /// Give the vector of delta-points.
      const std::vector<D>& std_vector() const;

      /// Print into \p ostr the window definition.
      void print(std::ostream& ostr) const;

    protected:

      classical_window_base();

      void insert(const D& d);
      unsigned delta_() const;               // Default implementation based on win_.
      void print_(std::ostream& ostr) const; // Default implementation based on win_.

      mln::window<D> win_;
    };



# ifndef MLN_INCLUDE_ONLY

    template <typename D, typename E>
    inline
    classical_window_base<D,E>::classical_window_base()
    {
    }

    template <typename D, typename E>
    inline
    unsigned
    classical_window_base<D,E>::size() const
    {
      return win_.size();
    }

    template <typename D, typename E>
    inline
    bool
    classical_window_base<D,E>::is_empty() const
    {
      return win_.is_empty();
    }

    template <typename D, typename E>
    inline
    bool
    classical_window_base<D,E>::is_centered() const
    {
      mln_invariant(win_.is_centered());
      return true;
    }

    template <typename D, typename E>
    inline
    bool
    classical_window_base<D,E>::is_symmetric() const
    {
      mln_invariant(win_.is_symmetric());
      return true;
    }

    template <typename D, typename E>
    inline
    void
    classical_window_base<D,E>::sym()
    {
      mln_invariant(win_.is_symmetric());
      // No-op.
    }

    template <typename D, typename E>
    inline
    unsigned
    classical_window_base<D,E>::delta() const
    {
      //       void *v = (void*)(& classical_window_base<D,E>::delta_);
      //       void *w = (void*)(& E::delta_);
      //       std::cout << v << ' ' << w << std::endl;
      return exact(this)->delta_();
    }

    template <typename D, typename E>
    inline
    unsigned
    classical_window_base<D,E>::delta_() const
    {
      return win_.delta();
    }

    template <typename D, typename E>
    inline
    const D&
    classical_window_base<D,E>::dp(unsigned i) const
    {
      mln_precondition(i < size());
      return win_.dp(i);
    }

    template <typename D, typename E>
    inline
    const std::vector<D>&
    classical_window_base<D,E>::std_vector() const
    {
      return win_.std_vector();
    }

    template <typename D, typename E>
    inline
    const std::vector<D>&
    classical_window_base<D,E>::vect() const
    {
      return std_vector();
    }

    template <typename D, typename E>
    inline
    bool
    classical_window_base<D,E>::has(const D& dp) const
    {
      return win_.has(dp);
    }

    template <typename D, typename E>
    inline
    void
    classical_window_base<D,E>::insert(const D& d)
    {
      win_.insert(d);
    }

    template <typename D, typename E>
    inline
    void
    classical_window_base<D,E>::print(std::ostream& ostr) const
    {
      exact(this)->print_(ostr);
    }

    template <typename D, typename E>
    inline
    void
    classical_window_base<D,E>::print_(std::ostream& ostr) const
    {
      ostr << win_;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_CLASSICAL_WINDOW_BASE_HH
