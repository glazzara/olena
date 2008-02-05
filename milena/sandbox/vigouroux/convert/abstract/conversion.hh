// Copyright (C) 2001, 2002, 2003, 2004  EPITA Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
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
// the Free Software Foundation, 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
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

#ifndef OLENA_CONVERT_ABSTRACT_CONVERSION_HH
# define OLENA_CONVERT_ABSTRACT_CONVERSION_HH

# include <mlc/type.hh>
# include <ntg/utils/debug.hh>
# include <functional>

namespace oln {
  namespace convert {
    /*!
    ** \brief Base classes for conversion.
    */
    namespace abstract {

      // fwd_decl
      template <class Exact, class Base>
      class conversion;

      template<class Result_Type,
	       class Exact, class Base>
      struct conversion_to_type;

      template<class Argument_Type, class Result_Type,
	       class Exact, class Base>
      struct conversion_from_type_to_type;

      template<class Conv>
      struct conversion_traits;

      /*! \brief Internal purpose only.
      */
      namespace internal {
	/// Retrieve the result type of a conversion.
	template <class Base, class T>
	struct output {};

	template <class Argument_Type, class Result_Type,
		  class Exact, class Base>
	struct output<conversion_from_type_to_type<Argument_Type, Result_Type, Exact, Base >, Argument_Type>
	{
	  typedef Result_Type ret;
	};

	template <class Result_Type,
		  class Exact, class Base, class T>
	struct output<conversion_to_type<Result_Type, Exact, Base >, T>
	{
	  typedef Result_Type ret;
	};
      }

    } // end of namespace abstract

    namespace abstract {
      /*! Base class for conversion.
      **
      ** \note If you write an class derived from this one, you
      ** must write the specialization of the output trait.
      */
      template<class Exact, class Base>
      struct conversion : public mlc_hierarchy::any< Exact >
      {
	static std::string
	name()
	{
	  return std::string("conversion<") + Exact::name() + ">";
	}

      public:
	template<class T>
	struct output
	{
	  typedef typename internal::output<Base, T>::ret ret;
	};

	/// Call the conversion written in the exact class.
	template <class T>
	typename output<T>::ret
	operator()(const T& in) const
	{
	  return this->exact().doit(in);
	}

      protected:
	conversion() {}
      };

      /// Base class for the conversion to a specific type.
      template<class Result_Type, class Exact = mlc::final, class Base = mlc::final>
      struct conversion_to_type :
	public conversion< typename mlc::exact_vt<conversion_to_type< Result_Type, Exact >, Exact>::ret,
                           typename mlc::exact_vt<conversion_to_type< Result_Type, Exact >, Base>::ret >
      {
	/* Additionally define result_type.  This is not required
	   in a conversion class (generally not possible to define).
	   But it's useful when it's available (like here)
	   because it make the conversion appear almost as Adaptable
	   Unary Function (it will just lack the argument_type, but
	   this typedef is not used very much.)  */

	typedef Result_Type result_type;

	static std::string
	name()
	{
	  // FIXME: Exact is not an integre type !
	  return std::string("conversion_to_type<")
	    + ntg_name(Result_Type) + ", "
	    + Exact::name() + ">";
	}

      protected:
	conversion_to_type() {}
      };

      /// Base class if both input and output types of the conversion are fixed.
      template<class Argument_Type, class Result_Type,
	       class Exact = mlc::final, class Base = mlc::final>
      struct conversion_from_type_to_type:
	public conversion_to_type< Result_Type,
				   typename mlc::exact_vt<conversion_from_type_to_type< Argument_Type, Result_Type, Base, Exact>, Exact>::ret,
				   typename mlc::exact_vt<conversion_from_type_to_type< Argument_Type, Result_Type, Base, Exact>, Base>::ret >
      {

	/* By defining argument_type, and inheriting from result_type,
	   we comply to the STL concept of Adaptable Unary Function.  */

	typedef Argument_Type argument_type;

	static std::string
	name()
	{
	  // FIXME: Exact is not an integre type !
	  return std::string("conversion_from_type_to_type<")
	    + ntg_name(Argument_Type) + ", "
	    + ntg_name(Result_Type) + ", "
	    + "FIXME: ntg_name(Exact)" + ">";
	}

      protected :
	conversion_from_type_to_type() {}
      };
    }

  } // convert

} // oln


#endif // OLENA_CONVERT_ABSTRACT_CONVERSION_HH
