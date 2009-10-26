#ifndef FUNCTION_HXX
#define FUNCTION_HXX

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

#endif
