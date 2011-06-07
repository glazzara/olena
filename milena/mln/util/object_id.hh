// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_UTIL_OBJECT_ID_HH
# define MLN_UTIL_OBJECT_ID_HH

/// \file
///
/// Base class of an object id.


# include <mln/core/concept/object.hh>
# include <mln/value/concept/integer.hh>
# include <mln/metal/abort.hh>

namespace mln
{

    // Forward declaration
  namespace util { template <typename Tag, typename V> class object_id; }

  namespace convert
  {

    namespace over_load
    {

      // object_id<Tag,V> -> V.
      template <typename Tag, typename V>
      void from_to_(const util::object_id<Tag,V>& from, V& to_);

    } // end of namespace mln::convert::over_load

  } // end of namespace mln::convert


  namespace util
  {

    /// Base class of an object id.
    /// \tparam Tag the tag type
    /// \tparam Equiv the equivalent value.
    template <typename Tag, typename V>
    class object_id : public value::Integer< object_id<Tag, V> >
    {
      public:
	/// The underlying type id.
	typedef V value_t;
	typedef unsigned equiv;
	typedef V enc;

	/// Constructors
	/// @{
	object_id();

	template <typename V2>
	  object_id(const V2& id);

	template <typename Tag2, typename V2>
	  object_id(const object_id<Tag2,V2>& other);
	/// @}

	template <typename V2>
	  object_id<Tag,V>& operator=(const V2& e);

	const V& value() const;
	V& value();

	operator unsigned() const;

	bool is_valid() const;
	void invalidate();

	unsigned to_equiv() const;

      protected:
	V id_;
    };


    template <typename Tag, typename V>
    bool
    operator==(const object_id<Tag,V>& lhs, const object_id<Tag,V>& rhs);

    template <typename Tag, typename V, typename V2>
    bool
    operator<(const object_id<Tag,V>& lhs, const object_id<Tag,V>& rhs);

  } // end of namespace mln::util

# ifndef MLN_INCLUDE_ONLY

  namespace util
  {

    template <typename Tag, typename V>
    inline
    object_id<Tag,V>::object_id()
    : id_(mln_max(V))
    {
    }

    template <typename Tag, typename V>
    template <typename V2>
    inline
    object_id<Tag,V>::object_id(const V2& id)
      : id_(id)
    {
      mlc_converts_to(V2,V)::check();
    }

    template <typename Tag, typename V>
    template <typename Tag2, typename V2>
    inline
    object_id<Tag,V>::object_id(const object_id<Tag2,V2>& id)
    {
      typedef object_id<Tag2,V2> id_t;
      mlc_abort(id_t)::check();
    }

    template <typename Tag, typename V>
    template <typename V2>
    inline
    object_id<Tag,V>&
    object_id<Tag,V>::operator=(const V2& v)
    {
      mlc_converts_to(V2,V)::check();

      id_ = v;
      return *this;
    }

    template <typename Tag, typename V>
    inline
    V&
    object_id<Tag,V>::value()
    {
      return id_;
    }

    template <typename Tag, typename V>
    inline
    const V&
    object_id<Tag,V>::value() const
    {
      return id_;
    }

    template <typename Tag, typename V>
    inline
    object_id<Tag,V>::operator unsigned() const
    {
      return id_;
    }


    template <typename Tag, typename V>
    inline
    bool
    object_id<Tag,V>::is_valid() const
    {
      return id_ != mln_max(V);
    }

    template <typename Tag, typename V>
    inline
    void
    object_id<Tag,V>::invalidate()
    {
      id_ = mln_max(V);
    }

    template <typename Tag, typename V>
    inline
    unsigned
    object_id<Tag,V>::to_equiv() const
    {
      return id_;
    }



    template <typename Tag, typename V, typename V2>
    inline
    bool
    operator==(const object_id<Tag,V>& lhs, const Value<V2>& rhs)
    {
      return lhs.value() == exact(rhs).to_equiv();
    }

    template <typename Tag, typename V>
    inline
    bool
    operator==(const object_id<Tag,V>& lhs, const object_id<Tag,V>& rhs)
    {
      return lhs.value() == rhs.value();
    }

    template <typename Tag, typename V>
    inline
    bool
    operator<(const object_id<Tag,V>& lhs, const object_id<Tag,V>& rhs)
    {
      return lhs.value() < rhs.value();
    }

  } // end of namespace mln::util

  namespace convert
  {

    namespace over_load
    {

      // object_id<Tag,V> -> V.
      template <typename Tag, typename V>
      void from_to_(const util::object_id<Tag,V>& from, V& to_)
      {
	to_ = from.value();
      }

    } // end of namespace mln::convert::over_load

  } // end of namespace mln::convert

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // ! MLN_UTIL_OBJECT_ID_HH
