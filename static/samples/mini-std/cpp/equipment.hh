#ifndef MINI_STD_EQUIPMENT_HH
# define MINI_STD_EQUIPMENT_HH

# include <cassert>
# include <stc/any.hh>
# include <stc/scoop.hh>



// Patch.

# define abc_vtype_(From, Target) abc::vtype<From, abc::typedef_::Target>::ret
# define abc_vtype(From, Target) typename abc_vtype_(From, Target)

# define abc_typename(Vtype) typedef abc_vtype(Exact, Vtype) Vtype

# define stc_defer(Target)						\
   typename abc::deferred_vtype<Exact, abc::typedef_::Target>::ret

# define stc_introducing(Vtype) typedef abc_vtype(self_type, Vtype) Vtype

// stc_deferred_vtype(test, Exact, dim)



namespace abc
{

// Namespace equipment.
#include <stc/scoop.hxx>


  /// Typedef declarations.
  /// \{

  // From std.
  mlc_decl_typedef(value_type);
  mlc_decl_typedef(value_compare);
  mlc_decl_typedef(reference);
  mlc_decl_typedef(const_reference);
  mlc_decl_typedef(data_type);
  mlc_decl_typedef(size_type);
  mlc_decl_typedef(distance_type);
  mlc_decl_typedef(iterator);
  mlc_decl_typedef(const_iterator);
  mlc_decl_typedef(reverse_iterator);
  mlc_decl_typedef(const_reverse_iterator);
  mlc_decl_typedef(key_type);
  mlc_decl_typedef(key_compare);
  mlc_decl_typedef(std_type);

  // Properties.
  mlc_decl_typedef(has_random_access);
  mlc_decl_typedef(has_front_insertion);
  mlc_decl_typedef(has_back_insertion);

  mlc_decl_typedef(tag);

  /// \}



  namespace automatic
  {

    template < template <class> class abstraction,
	       typename Exact, typename tag = void >
    struct impl;

    template < template <class> class abstraction, typename Exact >
    struct impl< abstraction, Exact, mlc::none >
    {
      // none means nothing!
    };

    template < template <class> class abstraction, typename Exact >
    struct impl< abstraction, Exact, void >
      : impl< abstraction, Exact, abc_vtype(Exact, tag) >
    {
      // fetch impl w.r.t. tag
    };

  } // end of namespace abc::automatic



} // end of namespace abc


#endif // ! MINI_STD_EQUIPMENT_HH
