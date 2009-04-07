// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_ACCU_LABEL_MAJ_HH
# define MLN_ACCU_LABEL_MAJ_HH

/// \file mln/accu/label_maj.hh
///
/// Define an accumulator that computes the most represented label in
/// a component.


# include <mln/core/concept/meta_accumulator.hh>
# include <mln/accu/internal/base.hh>
# include <mln/trait/value_.hh>
# include <mln/util/array.hh>
# include <mln/util/couple.hh>
# include <vector>
# include <map>


namespace mln
{

  namespace accu
  {


    /// Computes the most represented label in a component.
    /// The parameter \c L1 is the label type of the component in the
    /// first label image.
    /// The parameter \c L2 is the label type in the second label image.
    template <typename L1, typename L2>
    struct label_maj
//      : public mln::accu::internal::base< util::array<util::couple<L2,float> >,
//					  label_maj<L1,L2> >
    {
      typedef L2 argument;

      label_maj();
      label_maj(unsigned nlabels);

      /// Manipulators.
      /// \{
      void init();
      void take(const L1& l, const L2& L);
      void take(const label_maj<L1,L2>& other);
      /// \}

      /// Get the value of the accumulator.
      util::array< util::couple<L2,float> > to_result() const;

      //operator L2 () const;

      /// Check whether this accu is able to return a result.
      /// Always true here.
      bool is_valid() const;

      util::array< util::couple<L2,float> > finalize_() const;

      const util::array<std::map<L2,unsigned> >& maj_array_() const;

    protected:
      util::array<std::map<L2,unsigned> > maj_;

    };

# ifndef MLN_INCLUDE_ONLY

    template <typename L1, typename L2>
    inline
    label_maj<L1,L2>::label_maj()
    {
    }

    template <typename L1, typename L2>
    inline
    label_maj<L1,L2>::label_maj(unsigned nlabels)
      : maj_(nlabels + 1) // Do not forget to count the background label.
    {
    }

    template <typename L1, typename L2>
    inline
    void
    label_maj<L1,L2>::init()
    {
    }

    template <typename L1, typename L2>
    inline
    void
    label_maj<L1,L2>::take(const L1& l, const L2& L)
    {
      if (maj_[l].find(L) != maj_[l].end())
	maj_[l][static_cast<unsigned>(L)] += 1;
      else
	maj_[l][static_cast<unsigned>(L)] = 1;
    }

    template <typename L1, typename L2>
    inline
    void
    label_maj<L1,L2>::take(const label_maj<L1,L2>& other)
    {
      //FIXME
      abort();
    }

    template <typename L1, typename L2>
    inline
    util::array< util::couple<L2,float> >
    label_maj<L1,L2>::finalize_() const
    {
      util::array< util::couple<L2,float> > p(maj_.nelements());
      for (unsigned i = 1; i < maj_.nelements(); ++i)
      {
	L2
	  maj_l = literal::zero;
	unsigned
	  maj_card = 0,
	  total_card = 0;

	for (typename std::map<L2,unsigned>::const_iterator it = maj_[i].begin();
	      it != maj_[i].end(); ++it)
	{
	  if (maj_card < it->second)
	  {
	    maj_l = it->first;
	    maj_card = it->second;
	  }
	  total_card += it->second;
	}

	p[i] = make::couple(maj_l, maj_card / static_cast<float>(total_card));
      }

      return p;
    }

    template <typename L1, typename L2>
    inline
    const util::array<std::map<L2,unsigned> >&
    label_maj<L1,L2>::maj_array_() const
    {
      return maj_;
    }


    template <typename L1, typename L2>
    inline
    util::array< util::couple<L2,float> >
    label_maj<L1,L2>::to_result() const
    {
      return finalize_();
    }

    template <typename L1, typename L2>
    inline
    bool
    label_maj<L1,L2>::is_valid() const
    {
      return maj_.nelements() > 0;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_LABEL_MAJ_H _HH
