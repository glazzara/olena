/* Copyright (C) 2005 EPITA Research and Development Laboratory (LRDE)

   This file is part of Olena.

   Olena is free software: you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation, version 2 of the License.

   Olena is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Olena.  If not, see <http://www.gnu.org/licenses/>.

   As a special exception, you may use this file as part of a free
   software project without restriction.  Specifically, if other files
   instantiate templates or use macros or inline functions from this
   file, or you compile this file and link it with other files to produce
   an executable, this file does not by itself cause the resulting
   executable to be covered by the GNU General Public License.  This
   exception does not however invalidate any other reasons why the
   executable file might be covered by the GNU General Public License.  */

#ifndef DYN_FUNCTION_CC
# define DYN_FUNCTION_CC

# include <cassert>
# include <string>
# include "data.hh"
# include <list>
# include "function_loader.hh"

namespace dyn
{

    <%- DYN_MAX_ARGUMENTS.times do |i| -%>

      <%- arguments = (0 .. i - 1).map { |j| "const data& arg#{j}" }.join(', ') -%>
      <%- objects   = (0 .. i - 1).map { |j| "arg#{j}" } -%>

      data
      generic_fun::operator() (<%= arguments %>) const
      {
        typedef data (*func_t)(<%= (['const dyn::data&'] * i).join(', ') %>);
        arguments_types_t arguments_types;

        if (obj_ptr_)
          arguments_types.push_back(obj_ptr_->type());

      <%- i.times do |j| -%>
        arguments_types.push_back(arg<%= j %>.type());
      <%- end -%>

        void* ptr = load_function(kind_, name_, arguments_types, header_paths_);
        assert(ptr);

        if (obj_ptr_)
        {
          typedef data (*func_t2)(<%= (['const dyn::data&'] * (i + 1)).join(', ') %>);
          return ((func_t2)ptr)(<%= (['*obj_ptr_'] + objects).join(', ') %>);
        }

        return ((func_t)ptr)(<%= objects.join(', ') %>);
      }

    <%- end -%>

} // end of namespace dyn

#endif // ! DYN_FUNCTION_CC
