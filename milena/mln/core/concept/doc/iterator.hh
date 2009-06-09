// Copyright (C) 2007 EPITA Research and Development Laboratory
//
// This file is part of the Milena Library.  This library is free
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

/*! \file
 * \brief This file documents the concept of mln::Iterator.
 */


namespace mln
{

  namespace doc
  {

    /*! \brief Documentation class for mln::Iterator.
     * \see mln::Iterator
     */
    template <typename E>
    struct Iterator : public Object<E>
    {
      /*! \brief Returns true if the iterator is valid, that is, designates
       * an element.
       */
      bool is_valid() const;

      /*! \brief Invalidate the iterator.
       */ 
      void invalidate();

      /*! \brief Start an iteration.
       *
       * Make the iterator designate the first element if it exists.
       * If this first element does not exist, the iterator is not
       * valid.
       */ 
      void start();

      /*! \brief Go to the next element.
       *
       * Make the iterator jump from the current element to the next
       * one.  The current element is thus updated.
       */ 
      void next_();

    protected:
      Iterator();
    };

  } // end of namespace mln::doc

} // end of namespace mln
