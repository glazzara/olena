// Copyright (C) 2006 EPITA Research and Development Laboratory
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

#ifndef OLN_MORPHER_IDENTITY
# define OLN_MORPHER_IDENTITY

# include <oln/morpher/internal/image_extension.hh>
# include <oln/morpher/tags.hh>


namespace oln
{

  namespace morpher
  {
    // Forward declaration.
    template <typename Image> struct identity;

  } // end of namespace oln::morpher


  /// Super type.
  template <typename Image>
  struct set_super_type< morpher::identity<Image> >
  {
    typedef morpher::identity<Image> self_t;
    typedef morpher::internal::image_extension<Image, self_t> ret;
  };


  /// New virtual types associated with oln::morpher::identity.
  /// \{
  template <typename Image>
  struct vtypes< morpher::identity<Image> >
  {
    typedef oln::morpher::tag::identity morpher_type;
  };
  /// \}


  namespace morpher
  {
    /// Identity morpher.
    template <typename Image>
    class identity : public stc_get_supers(identity<Image>)
    {
    private:
      typedef identity<Image> self_t;
      typedef stc_get_nth_super(self_t, 1) super_t;

    public:
      // FIXME: Handle the constness.
      identity(const Image& image) :
	super_t(image)
      {
      }

    };

  } // end of namespace oln::morpher

} // end of namespace oln

#endif // ! OLN_MORPHER_IDENTITY
