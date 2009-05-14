// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory
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

#ifndef MLN_ACCU_CENTER_WEIGHT_HH
# define MLN_ACCU_CENTER_WEIGHT_HH

/// \file mln/accu/center.hh
///
/// Define an accumulator that computes the mass center of a site set.
///
/// \todo Fix to_result() value when invalid...

# include <mln/accu/internal/base.hh>
# include <mln/accu/bbox.hh>
# include <mln/util/couple.hh>

namespace mln
{

  namespace accu
  {


    /// Generic center accumulator class.
    ///
    /// \tparam P the type of site.
    /// \tparam V the type of vector to be used as result.
    ///		  The default vector type is the one provided by P.
    template <typename I, typename V = util::couple<mln_psite(I)::vec, int> >
    struct center_weight
      : public mln::accu::internal::base<V, center_weight<I,V> >
    {
      typedef mln_psite(I) argument;
      typedef V result;
      typedef I image_weight;
      typedef argument P;

      center_weight(const Image<image_weight>& im);

      /// Manipulators.
      /// \{
      void init();
      void take(const argument& t);
      void take(const center_weight<I,V>& other);
      /// \}

      /// Get the value of the accumulator.
      V to_result() const;
      operator P() const;

      /// Check whether this accu is able to return a result.
      bool is_valid() const;

    protected:
      algebra::vec<P::dim, mln_sum(mln_coord(P))> center_;
      unsigned nsites_;
      const image_weight& im_;
      int border_;
    };

    namespace meta
    {

      /// Meta accumulator for center.
      struct center_weight : public Meta_Accumulator< center_weight >
      {

	template <typename I>
	struct with
	{
	  typedef accu::center_weight< I> ret;
	};

      };

    } // end of namespace mln::accu::meta



# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename V>
    inline
    center_weight<I,V>::center_weight(const Image<image_weight>& im)
      : im_(exact(im))
    {
      init();
    }

    template <typename I, typename V>
    inline
    void
    center_weight<I,V>::init()
    {
      center_ = literal::zero;
      nsites_ = 0;
      border_ = 0;
    }

    template <typename I, typename V>
    inline
    void
    center_weight<I,V>::take(const argument& t)
    {
      center_ += (im_(t) * t.to_vec());
      nsites_ += im_(t);
      if (t[1] == 0)
        border_ = 1;
      if (static_cast<unsigned>(t[1]) == im_.ncols() - 1)
        border_ = 2;
    }

    template <typename I, typename V>
    inline
    void
    center_weight<I,V>::take(const center_weight<I,V>& other)
    {
      center_ += other.center_;
      nsites_ += other.nsites_;
      if (border_ == 0)
        border_ = other.border_;
    }

    template <typename I, typename V>
    inline
    V
    center_weight<I,V>::to_result() const
    {
      // mln_precondition(is_valid());
      if (! is_valid())
	return V();
      return V(center_ / nsites_, border_);
    }

    template <typename I, typename V>
    inline
    center_weight<I,V>::operator P() const
    {
      return P(to_result());
    }

    template <typename I, typename V>
    inline
    bool
    center_weight<I,V>::is_valid() const
    {
      return nsites_ > 0;
    }

# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_CENTER_WEIGHT_HH
