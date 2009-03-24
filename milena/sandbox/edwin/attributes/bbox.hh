// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory
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

#ifndef MLN_MORPHO_ATTRIBUTE_BBOX_HH_
# define MLN_MORPHO_ATTRIBUTE_BBOX_HH_

/// \file mln/morpho/attribute/bbox.hh
///
/// Define an accumulator that computes bounding box of a
/// component.

# include <mln/core/concept/box.hh>


namespace mln
{

 // Forward declaration.

  namespace morpho {
    namespace attribute {
      template <typename I> class bbox;
    }
  }

  // Traits.

  namespace trait
  {

    template <typename I>
    struct accumulator_< morpho::attribute::bbox<I> >
    {
      typedef accumulator::has_untake::no	has_untake;
      typedef accumulator::has_set_value::no	has_set_value;
      typedef accumulator::has_stop::no		has_stop;
      typedef accumulator::when_pix::use_p	when_pix;
    };

  } // end of namespace mln::trait

   namespace morpho {
     namespace attribute {

       template <typename I>
       struct bbox : public mln::accu::internal::base<const box<mln_psite(I)>&, bbox<I> >
       {
	 typedef mln::accu::internal::base<const box<mln_psite(I)>&, bbox<I> > super_;

       public:
	 typedef mln_psite(I) P;
	 typedef mln_psite(I) argument;

	 bbox();

	 /// Manipulators.
	 /// \{
	 void init();
	 void take(const argument& p);
	 void take(const bbox<I>& other);
	 void take_as_init(const argument& p);

	 /// \}

	 /// Get the value of the accumulator.
	 const box<P>& to_result() const;


	 /// Check whether this accu is able to return a result.
	 /// Always true here.
	 bool is_valid() const;

       protected:

	 bool is_valid_;
	 box<mln_psite(I)> b_;
       };

# ifndef MLN_INCLUDE_ONLY

       template <typename I>
       inline
       bbox<I>::bbox()
       {
	 init();
       }

       template <typename I>
       inline
       void
       bbox<I>::init()
       {
	 is_valid_ = false;
       }

       template <typename I>
       inline
       void
       bbox<I>::take_as_init(const mln_psite(I)& p)
       {
	 b_.pmin() = p;
	 b_.pmax() = p;
	 is_valid_ = true;
       }

       template <typename I>
       inline
       void
       bbox<I>::take(const mln_psite(I)& p)
       {
	 if (!is_valid_)
	   {
	     b_.pmin() = p;
	     b_.pmax() = p;
	     is_valid_ = true;
	     return;
	   }
	 for (unsigned i = 0; i < mln_psite_(I)::dim; ++i)
	   if (p[i] < b_.pmin()[i])
	     b_.pmin()[i] = p[i];
	   else if (p[i] > b_.pmax()[i])
	     b_.pmax()[i] = p[i];
       }

       template <typename I>
       inline
       void
       bbox<I>::take(const bbox<I>& other)
       {
	 if (! other.is_valid_)
	   {
	     // no-op
	     return;
	   }
	 if (! this->is_valid_)
	   {
	     // 'other' makes '*this' valid
	     *this = other;
	     is_valid_ = true;
	     return;
	   }
	 // both are valids so:
	 const box<mln_psite(I)>& o_b = other.b_;
	 for (unsigned i = 0; i < mln_psite_(I)::dim; ++i)
	   {
	     if (o_b.pmin()[i] < b_.pmin()[i])
	       b_.pmin()[i] = o_b.pmin()[i];
	     if (o_b.pmax()[i] > b_.pmax()[i])
	       b_.pmax()[i] = o_b.pmax()[i];
	   }
       }

       template <typename I>
       inline
       const box<mln_psite(I)>&
       bbox<I>::to_result() const
       {
	 mln_precondition(is_valid_);
	 return b_;
       }

       template <typename I>
       inline
       bool
       bbox<I>::is_valid() const
       {
	 return is_valid_;
       }


# endif /* !MLN_INCLUDE_ONLY */
     } // end of namespace mln::morpho::attribute
   } // end of namespace mln::morpho
} // end of namespace mln



#endif /* !MLN_MORPHO_ATTRIBUTE_BBOX_HH_ */
