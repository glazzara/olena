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

#ifndef MLN_VALUE_VITER_HH
# define MLN_VALUE_VITER_HH

/*! \file mln/value/viter.hh
 *
 * \brief Definition of iterators on value sets.
 */

# include <mln/core/concept/viter.hh>



namespace mln
{

  namespace value
  {


    /*! \brief FIXME: Forward iterator on a set of values.
     *
     * The parameter \c S is the type of value set.
     */
    template <typename S>
    struct fwd_viter_ : public Viter< fwd_viter_<S> >
    {
      /// Value associated type.
      typedef mln_value(S) value;

      /// Constructor.
      fwd_viter_(const Value_Set<S>& s);

      /// Convertion into a value.
      operator value() const;

      /// Test if the iterator is valid.
      bool is_valid() const;

      /// Invalidate the iterator.
      void invalidate();

      /// Start an iteration.
      void start();

      /// Go to the next value.
      void next_();
      
    private:

      const S& s_;
      std::size_t i_;
    };




    /*! \brief FIXME: Backward iterator on a set of values.
     *
     * The parameter \c S is the type of value set.
     */
    template <typename S>
    struct bkd_viter_ : public Viter< bkd_viter_<S> >
    {
      /// Value associated type.
      typedef mln_value(S) value;

      /// Constructor.
      bkd_viter_(const Value_Set<S>& s);

      /// Convertion into a value.
      operator value() const;

      /// Test if the iterator is valid.
      bool is_valid() const;

      /// Invalidate the iterator.
      void invalidate();

      /// Start an iteration.
      void start();

      /// Go to the next value.
      void next_();
      
    private:

      const S& s_;
      std::size_t i_;
    };



# ifndef MLN_INCLUDE_ONLY


    // fwd_viter_<S>

    template <typename S>
    fwd_viter_<S>::fwd_viter_(const Value_Set<S>& s)
      : s_(exact(s))
    {
      invalidate();
    }

    template <typename S>
    fwd_viter_<S>::operator mln_value(S) () const
    {
      mln_precondition(is_valid());
      return s_[i_];
    }

    template <typename S>
    bool
    fwd_viter_<S>::is_valid() const
    {
      return i_ < s_.nvalues();
    }

    template <typename S>
    void
    fwd_viter_<S>::invalidate()
    {
      i_ = s_.nvalues();
    }

    template <typename S>
    void
    fwd_viter_<S>::start()
    {
      i_ = 0;
    }

    template <typename S>
    void
    fwd_viter_<S>::next_()
    {
      ++i_;
    }


    // bkd_viter_<S>

    template <typename S>
    bkd_viter_<S>::bkd_viter_(const Value_Set<S>& s)
      : s_(exact(s))
    {
      invalidate();
    }

    template <typename S>
    bkd_viter_<S>::operator mln_value(S) () const
    {
      mln_precondition(is_valid());
      return s_[i_];
    }

    template <typename S>
    bool
    bkd_viter_<S>::is_valid() const
    {
      return i_ != s_.nvalues();
    }

    template <typename S>
    void
    bkd_viter_<S>::invalidate()
    {
      i_ = s_.nvalues();
    }

    template <typename S>
    void
    bkd_viter_<S>::start()
    {
      i_ = s_.nvalues() - 1;
    }

    template <typename S>
    void
    bkd_viter_<S>::next_()
    {
      if (i_ == 0)
	{
	  invalidate();
	  return;
	}
      --i_;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::value

} // end of namespace mln


#endif // ! MLN_VALUE_VITER_HH
