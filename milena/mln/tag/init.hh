// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_TAG_INIT_HH
# define MLN_TAG_INIT_HH

/// \file
///
/// Definition of tags used in the mln::init mechanism.


namespace mln
{

  namespace tag
  {


    struct bbox_t      {};
    struct border_t    {};
    struct data_t      {};
    struct domain_t    {};
    struct extension_t {};
    struct function_t  {};
    struct image_t     {};

    extern bbox_t      bbox;
    extern border_t    border;
    extern data_t      data;
    extern domain_t    domain;
    extern extension_t extension;
    extern function_t  function;
    extern image_t     image;

# ifndef MLN_INCLUDE_ONLY

#  ifndef MLN_WO_GLOBAL_VARS

    bbox_t      bbox;
    border_t    border;
    data_t      data;
    domain_t    domain;
    extension_t extension;
    function_t  function;
    image_t     image;

#  endif // !MLN_WO_GLOBAL_VARS

# endif // !MLN_INCLUDE_ONLY

  } // end of namespace mln::tag

} // end of namespace mln


#endif // ! MLN_TAG_INIT_HH
