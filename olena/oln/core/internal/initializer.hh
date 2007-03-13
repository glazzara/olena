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

#ifndef OLN_CORE_INTERNAL_INITIALIZER_HH
# define OLN_CORE_INTERNAL_INITIALIZER_HH


namespace oln
{


  namespace ERROR
  {

    template <typename S>
    struct bad_initialization_of_
    {
      template <typename D>
      struct with_;
    };

  } // end of namespace oln::ERROR



  namespace internal
  {


    template <typename D>
    struct initializer_
    {
      initializer_(const D& data);
      const D& operator*()  const;
      const D* operator->() const;

    protected:
      D data_;
    };


    template <typename Tag, typename Subject, typename D>
    void init__(const Tag& t, Subject& s, const initializer_<D>& d);

    template <typename Subject, typename D>
    void init__(Subject& s, const initializer_<D>& d);



# ifndef OLN_INCLUDE_ONLY

    template <typename D>
    initializer_<D>::initializer_(const D& data)
      : data_(data)
    {}

    template <typename D>
    const D&
    initializer_<D>::operator*()  const
    {
      return this->data_;
    }

    template <typename D>
    const D*
    initializer_<D>::operator->() const
    {
      return &(this->data_);
    }

    template <typename Tag, typename Subject, typename D>
    void init__(const Tag&, Subject&, const initializer_<D>&)
    {
      mlc::abort_<D, typename ERROR::bad_initialization_of_<Subject>::template with_<D> >::check();
    }

    template <typename Subject, typename D>
    void init__(Subject&, const initializer_<D>&)
    {
      mlc::abort_<D, typename ERROR::bad_initialization_of_<Subject>::template with_<D> >::check();
    }

# endif //  OLN_INCLUDE_ONLY


  } // end of namespace oln::internal

} // end of namespace oln


#endif // ! OLN_CORE_INTERNAL_INITIALIZER_HH
