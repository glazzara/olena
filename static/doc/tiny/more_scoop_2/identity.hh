// Copyright (C) 2007 EPITA Research and Development Laboratory.
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

#ifndef STC_DOC_TINY_MORE_SCOOP_2_IDENTITY_HH

/* \file doc/tiny/more_scoop_2/identity.hh

   \brief Identity behavior for iterators.  */


namespace abc
{


  // Behavior tag.

  namespace behavior { struct identity; }



  namespace automatic
  {


    // Iterator

    template <typename Exact>
    struct set_impl< Iterator, behavior::identity, Exact > : public virtual any<Exact>
    {
      void impl_init()            { this->exact().delegatee_.init();  }
      bool impl_is_valid() const  { return this->exact().delegatee_.is_valid(); }
    };



    // Forward_Iterator

    template <typename Exact>
    struct set_impl< Forward_Iterator, behavior::identity, Exact > : public virtual any<Exact>
    {
      void impl_next()  { this->exact().delegatee_.next();  }
    };


    // Backward_Iterator

    template <typename Exact>
    struct set_impl< Backward_Iterator, behavior::identity, Exact > : public virtual any<Exact>
    {
      void impl_prev()  { this->exact().delegatee_.prev();  }
    };


    // Bidirectional_Iterator

    template <typename Exact>
    struct set_impl< Bidirectional_Iterator, behavior::identity, Exact > : public virtual any<Exact>
    {
    };



    // Read_Iterator

    template <typename Exact>
    struct set_impl< Read_Iterator, behavior::identity, Exact > : public virtual any<Exact>
    {
      stc_typename(value);
      const value& impl_get_value() const  { return this->exact().delegatee_.get_value(); }
    };


    // Read_Only_Iterator

    template <typename Exact>
    struct set_impl< Read_Only_Iterator, behavior::identity, Exact > : public virtual any<Exact>
    {
    };


    // Write_Iterator

    template <typename Exact>
    struct set_impl< Write_Iterator, behavior::identity, Exact > : public virtual any<Exact>
    {
      stc_typename(value);
      void impl_set_value(const value& v)  { this->exact().delegatee_.set_value(v); }
    };


    // Write_Only_Iterator

    template <typename Exact>
    struct set_impl< Write_Only_Iterator, behavior::identity, Exact > : public virtual any<Exact>
    {
    };


    // Read_Write_Iterator

    template <typename Exact>
    struct set_impl< Read_Write_Iterator, behavior::identity, Exact > : public virtual any<Exact>
    {
    };




  } // end of namespace abc::automatic


} // end of namespace abc


#endif // ! STC_DOC_TINY_MORE_SCOOP_2_IDENTITY_HH
