// Copyright (C) 2007, 2008, 2009, 2012 EPITA Research and Development
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

#ifndef MLN_VALUE_VITER_HH
# define MLN_VALUE_VITER_HH

/*! \file
 *
 * \brief Definition of iterators on value sets.
 *
 * \todo Proxify it!
 */

# include <mln/core/concept/value_iterator.hh>
# include <mln/core/concept/value_set.hh>


namespace mln
{

  namespace value
  {


    /*!
      \internal
      \brief FIXME: Forward iterator on a set of values.

      The parameter \c S is the type of value set.
    */
    template <typename S>
    struct fwd_viter_ : public Value_Iterator< fwd_viter_<S> >
    {
      /// Value associated type.
      typedef mln_value(S) value;

      /// Constructor without argument.
      fwd_viter_();

      /// Constructor.
      fwd_viter_(const Value_Set<S>& s);

      void change_target(const S& s);

      /// Test if the iterator is valid.
      bool is_valid() const;

      /// Invalidate the iterator.
      void invalidate();

      /// Start an iteration.
      void start();

      /// Go to the next value.
      void next_();

      /// Conversion into a value.
      operator mln_value(S) () const;

      /// Give the current index.
      unsigned index_() const;

    private:

      const S* s_;
      unsigned i_;
    };




    /*!
      \internal
      \brief FIXME: Backward iterator on a set of values.

      The parameter \c S is the type of value set.
    */
    template <typename S>
    struct bkd_viter_ : public Value_Iterator< bkd_viter_<S> >
    {
      /// Value associated type.
      typedef mln_value(S) value;

      /// Constructor without argument.
      bkd_viter_();

      /// Constructor.
      bkd_viter_(const Value_Set<S>& s);

      void change_target(const S& s);

      /// Test if the iterator is valid.
      bool is_valid() const;

      /// Invalidate the iterator.
      void invalidate();

      /// Start an iteration.
      void start();

      /// Go to the next value.
      void next_();

      /// Conversion into a value.
      operator mln_value(S) () const;

      /// Give the current index.
      unsigned index_() const;

    private:

      const S* s_;
      unsigned i_;
    };



# ifndef MLN_INCLUDE_ONLY


    // fwd_viter_<S>

    template <typename S>
    inline
    fwd_viter_<S>::fwd_viter_()
      : s_(0)
    {
    }

    template <typename S>
    inline
    fwd_viter_<S>::fwd_viter_(const Value_Set<S>& s)
    {
      change_target(exact(s));
    }

    template <typename S>
    inline
    void
    fwd_viter_<S>::change_target(const S& s)
    {
      s_ = &s;
      invalidate();
    }

    template <typename S>
    inline
    bool
    fwd_viter_<S>::is_valid() const
    {
      return s_ != 0 && i_ < s_->nvalues();
    }

    template <typename S>
    inline
    void
    fwd_viter_<S>::invalidate()
    {
      i_ = s_->nvalues();
    }

    template <typename S>
    inline
    void
    fwd_viter_<S>::start()
    {
      i_ = 0;
    }

    template <typename S>
    inline
    void
    fwd_viter_<S>::next_()
    {
      ++i_;
    }

    template <typename S>
    inline
    fwd_viter_<S>::operator mln_value(S) () const
    {
      mln_precondition(is_valid());
      return (*s_)[i_];
    }

    template <typename S>
    inline
    unsigned
    fwd_viter_<S>::index_() const
    {
      return i_;
    }


    // bkd_viter_<S>

    template <typename S>
    inline
    bkd_viter_<S>::bkd_viter_()
      : s_(0)
    {
    }

    template <typename S>
    inline
    bkd_viter_<S>::bkd_viter_(const Value_Set<S>& s)
    {
      change_target(exact(s));
    }

    template <typename S>
    inline
    void
    bkd_viter_<S>::change_target(const S& s)
    {
      s_ = &s;
      invalidate();
    }

    template <typename S>
    inline
    bool
    bkd_viter_<S>::is_valid() const
    {
      return s_ != 0 && i_ != s_->nvalues();
    }

    template <typename S>
    inline
    void
    bkd_viter_<S>::invalidate()
    {
      i_ = s_->nvalues();
    }

    template <typename S>
    inline
    void
    bkd_viter_<S>::start()
    {
      i_ = s_->nvalues() - 1;
    }

    template <typename S>
    inline
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

    template <typename S>
    inline
    bkd_viter_<S>::operator mln_value(S) () const
    {
      mln_precondition(is_valid());
      return (*s_)[i_];
    }

    template <typename S>
    inline
    unsigned
    bkd_viter_<S>::index_() const
    {
      return i_;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::value

} // end of namespace mln


#endif // ! MLN_VALUE_VITER_HH
