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

#endif // !DYN_ALL_METHODS_CC
