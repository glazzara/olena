/* Copyright (C) 2005, 2009 EPITA Research and Development Laboratory (LRDE)

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

#ifndef DYN_FUNCTION_HH
# define DYN_FUNCTION_HH

# include <cassert>
# include <string>

namespace dyn
{

  enum fun_kind
  {
    FUN,
    OP,
    METH,
    CTOR
  };

  struct data;

  struct generic_fun
  {

    generic_fun(fun_kind kind,
                const std::string name,
                const std::string header_paths,
                const data*       obj_ptr) :
      kind_(kind), name_(name),
      header_paths_(header_paths), obj_ptr_(obj_ptr) {}


    <%- DYN_MAX_ARGUMENTS.times do |i| -%>
      data
      operator() (<%= (["const data&"] * i).join(', ') %>) const;
    <%- end -%>

    fun_kind kind_;
    std::string name_;
    std::string header_paths_;
    const data* obj_ptr_;
  };

  <% %w[ fun ctor meth ].each do |name| %>
  struct <%= name %> : public generic_fun
  {
    <%= name %>(const std::string name,
                const std::string header_paths="*",
                const data*       obj_ptr=0) :
      generic_fun(<%= name.upcase %>, name, header_paths, obj_ptr) {}
  };
  <% end %>

  struct op : public generic_fun
  {
    op(const std::string name,
       const std::string header_paths="*",
       const data*       obj_ptr=0) :
      generic_fun(OP, std::string("operator") + name,
                  header_paths, obj_ptr) {}
  };

} // end of namespace dyn

#endif // ! DYN_FUNCTION_HH
