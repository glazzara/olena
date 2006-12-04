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

#ifndef OLN_CORE_ABSTRACT_WINDOW_HH
# define OLN_CORE_ABSTRACT_WINDOW_HH

# include <oln/core/typedefs.hh>


namespace oln
{

  namespace abstract { template <typename E> class window; }


  template <typename E>
  struct set_super_type< abstract::window<E> >
  {
    typedef mlc::none ret;
  };


  namespace abstract
  {


    /// Abstract window class.
    template <typename E>
    class window : public virtual stc::any__simple<E>,
		   public virtual oln::type
    {
    public:

      bool is_valid() const;

      struct decl
      {
	oln_virtual_typedef(grid);

	decl();
      };

    protected:

      window();
      ~window();

    }; // end of class oln::abstract::window<E>



# ifndef OLN_INCLUDE_ONLY

    template <typename E>
    bool window<E>::is_valid() const
    {
      return this->exact().impl_is_valid();
    }

    template <typename E>
    window<E>::window()
    {
    }

    template <typename E>
    window<E>::~window()
    {
      decl();
    }

    template <typename E>
    window<E>::decl::decl()
    {
    }

# endif


  } // end of namespace oln::abstract

} // end of namespace oln


#endif // ! OLN_CORE_ABSTRACT_WINDOW_HH
