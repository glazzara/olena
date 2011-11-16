// Copyright (C) 2007, 2008, 2009, 2011 EPITA Research and Development
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

#ifndef MLN_CORE_CONCEPT_GENERALIZED_PIXEL_HH
# define MLN_CORE_CONCEPT_GENERALIZED_PIXEL_HH

/// \file
///
/// Definition of the concept of mln::Generalized_Pixel.

# include <mln/core/concept/object.hh>
# include <mln/core/internal/force_exact.hh>
# include <mln/core/trait/qlf_value.hh>


namespace mln
{

  /// Base class for implementation classes that are pixels or that
  ///  have the behavior of pixels.
  ///
  /// \warning This class does \em not derive from mln::Object; it is
  /// for use as a parallel hierarchy.
  ///
  /// \see mln::doc::Generalized_Pixel for a complete documentation of this
  /// class contents.
  ///
  /// \todo (later) Add an access to the targetted image.
  ///
  template <typename E>
  struct Generalized_Pixel
  {
    /*
      typedef  value;
      typedef rvalue;
      typedef image; // is const or not

      rvalue val() const;
      image& ima() const;
      mln_qlf_value(image)** address_() const;
    */

    operator unsigned() const // FIXME: Change to std::size_t
    {
      const E& it_ = internal::force_exact<E>(*this);
      return & it_.val() - & it_.ima().buffer()[0];
    }

  protected:
    Generalized_Pixel();
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  inline
  Generalized_Pixel<E>::Generalized_Pixel()
  {
    typedef mln_value(E) value;
    typedef mln_rvalue(E) rvalue;

    rvalue (E::*m1)() const = & E::val;
    (void) m1;

    typedef mln_image(E) image;
    // FIXME: check "without &"
    image& (E::*m2)() const = & E::ima;
    (void) m2;

    mln_qlf_value(image)** (E::*m3)() const = & E::address_;
    (void ) m3;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_GENERALIZED_PIXEL_HH
