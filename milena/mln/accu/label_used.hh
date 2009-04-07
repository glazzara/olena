// Copyright (C) 2009 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_ACCU_LABEL_USED_HH
# define MLN_ACCU_LABEL_USED_HH

/// \file mln/accu/label_used.hh
///
/// Define an accumulator that references all the labels used.

# include <mln/accu/internal/base.hh>
# include <mln/core/concept/meta_accumulator.hh>
# include <mln/math/max.hh>


namespace mln
{

  namespace accu
  {

    /// Define an accumulator that references all the labels used.
    /// The parameter \a L is the label type.
    template <typename L>
    struct label_used : public mln::accu::internal::base< const fun::i2v::array<bool>& , label_used<L> >
    {
      typedef L argument;

      label_used();

      /// Initialize accumulator attributes.
      void init();

      /// Manipulators.
      /// \{
      void take(const argument&);
      void take(const label_used<L>& other);
      /// \}

      /// Get the value of the accumulator.
      const fun::i2v::array<bool>& to_result() const;

      /// Check whether this accu is able to return a result.
      /// Always true here.
      bool is_valid() const;

    protected:
      /// True if a label is used.
      fun::i2v::array<bool> label_used_;
    };


    namespace meta
    {

      /// Meta accumulator for label_used.
      struct label_used : public Meta_Accumulator< label_used >
      {
	template <typename L>
	struct with
	{
	  typedef accu::label_used<L> ret;
	};
      };

    } // end of namespace mln::accu::meta



# ifndef MLN_INCLUDE_ONLY

    template <typename L>
    inline
    label_used<L>::label_used()
    {
      init();
    }

    template <typename L>
    inline
    void
    label_used<L>::init()
    {
      label_used_.resize(1, true);
    }

    template <typename L>
    inline
    void
    label_used<L>::take(const argument& l)
    {
      if (label_used_.size() <= l)
	label_used_.resize(l.next(), false);

      label_used_(l) = true;
    }

    template <typename L>
    inline
    void
    label_used<L>::take(const label_used<L>& other)
    {
      unsigned
	max_size = math::max(other.to_result().size(), label_used_.size());

      label_used_.resize(max_size, false);
      for (unsigned i = 1; i < label_used_.size(); ++i)
	label_used_(i) = label_used_(i) || other.to_result()(i);
    }

    template <typename L>
    inline
    const fun::i2v::array<bool>&
    label_used<L>::to_result() const
    {
      return label_used_;
    }

    template <typename L>
    inline
    bool
    label_used<L>::is_valid() const
    {
      return true;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_LABEL_USED_HH
