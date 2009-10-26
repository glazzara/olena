#ifndef FUNCTION_HH
#define FUNCTION_HH

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

    const fun_kind kind_;
    const std::string name_;
    const std::string header_paths_;
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

#endif
