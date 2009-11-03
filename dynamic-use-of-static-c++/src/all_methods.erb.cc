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

#ifndef DYN_ALL_METHODS_CC
# define DYN_ALL_METHODS_CC

# define DYN_FULL_IMPLEMENTATION
# include "all_methods.hh"
# include "data.hh"
# include "function.hh"

namespace dyn
{

<%- (DYN_MAX_ARGUMENTS - 1).times do |i| -%>
  <%- arguments = (0 .. i - 1).map { |j| "const data& arg#{j}" } -%>
  <%- objects   = (0 .. i - 1).map { |j| "arg#{j}" } -%>

  data
  all_methods::send (<%= (['const std::string& meth_name'] + arguments).join(', ') %>) const
  {
    meth m(meth_name);
    return m(<%= (['self()'] + objects).join(', ') %>);
  }

  <%- ALL_METHODS.each do |meth_name, includes| -%>

  data
  all_methods::<%= meth_name %> (<%= arguments.join(', ') %>) const
  {
    meth m("<%= meth_name %>", "<%= includes.join(':') %>");
    return m(<%= (['self()'] + objects).join(', ') %>);
  }

  <%- end -%>
<%- end -%>

}

#endif // ! DYN_ALL_METHODS_CC
