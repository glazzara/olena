// Copyright (C) 2009, 2012 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_ACCU_COUNT_LABELS_HH
# define MLN_ACCU_COUNT_LABELS_HH

/// \file
///
/// Define an accumulator that counts the number of different labels.

# include <vector>
# include <mln/accu/internal/base.hh>
# include <mln/core/concept/meta_accumulator.hh>
# include <mln/metal/is_a.hh>

namespace mln
{

  // Forward declaration.
  namespace value { template <typename E> struct Symbolic; }

  namespace accu
  {

    /// \brief Count the number of different labels in an image.
    /// The parameter \a L is the label type to be count.
    ///
    /// \ingroup modaccuvalues
    //
    template <typename L>
    struct count_labels
      : public mln::accu::internal::base< unsigned , count_labels<L> >
    //	mlc_is_a(L, mln::value::Symbolic)::check_t
    {
      typedef L argument;

      count_labels();

      /// Manipulators.
      /// \{
      void init();
      void take(const argument&);
      void take(const count_labels<L>& other);

      /// Force the value of the counter to \a c.
      void set_value(unsigned c);
      /// \}

      /// Get the value of the accumulator.
      unsigned to_result() const;

      /// Check whether this accu is able to return a result.
      /// Always true here.
      bool is_valid() const;

    protected:
      /// The value of the counter.
      unsigned count_labels_;
      std::vector<bool> deja_vu_;
    };


    namespace meta
    {

      /// Meta accumulator for count_labels.
      struct count_labels : public Meta_Accumulator< count_labels >
      {
	template <typename L>
	struct with
	{
	  typedef accu::count_labels<L> ret;
	};
      };

    } // end of namespace mln::accu::meta



# ifndef MLN_INCLUDE_ONLY

    template <typename L>
    inline
    count_labels<L>::count_labels()
    {
      init();
    }

    template <typename L>
    inline
    void
    count_labels<L>::init()
    {
      count_labels_ = 0;
      deja_vu_.resize(mln_max(L), false);
    }

    template <typename L>
    inline
    void
    count_labels<L>::take(const argument& l)
    {
      if (!deja_vu_[l])
      {
	++count_labels_;
	deja_vu_[l] = true;
      }
      //else
      // No-op
    }

    template <typename L>
    inline
    void
    count_labels<L>::take(const count_labels<L>& other)
    {
      count_labels_ += other.count_labels_;
      for (unsigned i = 0; i < deja_vu_.size(); ++i)
	deja_vu_[i] = deja_vu_[i] || other.deja_vu_[i];
    }

    template <typename L>
    inline
    unsigned
    count_labels<L>::to_result() const
    {
      // The background label MUST not be counted.
      return count_labels_ - 1;
    }

    template <typename L>
    inline
    void
    count_labels<L>::set_value(unsigned c)
    {
      count_labels_ = c;
    }

    template <typename L>
    inline
    bool
    count_labels<L>::is_valid() const
    {
      return true;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_COUNT_LABELS_HH
