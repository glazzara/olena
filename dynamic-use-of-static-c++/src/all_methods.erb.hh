#ifndef DYN_ALL_METHODS_HH
# define DYN_ALL_METHODS_HH

# include <string>

namespace dyn
{
  struct data;
 
  struct all_methods
  {

    virtual const data& self() const = 0;
    virtual data& self() = 0;

    virtual ~all_methods() {};

# ifdef DYN_FULL_IMPLEMENTATION
<%- (DYN_MAX_ARGUMENTS - 1).times do |i| -%>
  <%- arguments = (0 .. i - 1).map { |j| "const data& arg#{j}" } -%>
    <%- ALL_METHODS.each do |meth, includes| -%>

    data <%= meth %>(<%= arguments.join(', ') %>) const;

    <%- end -%>
      
    data send(<%= (['const std::string& meth_name'] + arguments).join(', ') %>) const;

  <%- end -%>

# endif
    
  };

}

#endif // !DYN_ALL_METHODS_HH
