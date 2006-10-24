// Copyright (C) 2006 EPITA Research and Development Laboratory
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
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
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

#ifndef OLN_CORE_VALUE_COLOR_RGB_HH
# define OLN_CORE_VALUE_COLOR_RGB_HH

# include <xtd/vec.hh>
# include <oln/core/abstract/value.hh>


namespace oln
{

  namespace value
  {

    // FIXME: Perhaps this additional namespace is not necessary.
    namespace color
    {


      /// General RGB class.
      template <typename T>
      class rgb_ : public oln::abstract::value
      {
      public:
      
	/// Ctor.
	rgb_();
      
	/// Ctor.
	rgb_(const T& r, const T& g, const T& b);
      
	/// Cpy ctor.
	template <typename U>
	rgb_(const rgb_<U>& rhs);

	/// Op=.
	template <typename U>
	rgb_<T>& operator=(const rgb_<U>& rhs);

	T  red() const;
	T& red();
	typedef T red_type;

	T  green() const;
	T& green();
	typedef T green_type;
      
	T  blue() const;
	T& blue();
	typedef T blue_type;

	const xtd::vec<3,T>& to_vec() const;
	xtd::vec<3,T>& to_vec();

      protected:
	xtd::vec<3,T> val_;
      };

      typedef rgb_<unsigned char> rgb8;

      template <unsigned n, typename T>
      std::ostream& operator<<(std::ostream& ostr, const rgb_<T>& c);


# ifndef OLN_INCLUDE_ONLY

      
      template <typename T>
      rgb_<T>::rgb_()
      {
      }

      template <typename T>
      rgb_<T>::rgb_(const T& r, const T& g, const T& b)
      {
	val_[0] = r;
	val_[1] = g;
	val_[2] = b;
      }
      
      template <typename T>
      template <typename U>
      rgb_<T>::rgb_(const rgb_<U>& rhs)
      {
	val_[0] = rhs.red();
	val_[1] = rhs.green();
	val_[2] = rhs.blue();
      }

      template <typename T>
      template <typename U>
      rgb_<T>& rgb_<T>::operator=(const rgb_<U>& rhs)
      {
	val_[0] = rhs.red();
	val_[1] = rhs.green();
	val_[2] = rhs.blue();
	return *this;
      }

      template <typename T>
      T  rgb_<T>::red() const
      {
	return val_[0];
      }

      template <typename T>
      T& rgb_<T>::red()
      {
	return val_[0];
      }

      template <typename T>
      T  rgb_<T>::green() const
      {
	return val_[1];
      }

      template <typename T>
      T& rgb_<T>::green()
      {
	return val_[1];
      }
      
      template <typename T>
      T  rgb_<T>::blue() const
      {
	return val_[2];
      }

      template <typename T>
      T& rgb_<T>::blue()
      {
	return val_[2];
      }

      template <typename T>
      const xtd::vec<3,T>& rgb_<T>::to_vec() const
      {
	return val_;
      }

      template <typename T>
      xtd::vec<3,T>& rgb_<T>::to_vec()
      {
	return val_;
      }


      template <typename T>
      std::ostream& operator<<(std::ostream& ostr, const rgb_<T>& c)
      {
	return ostr << c.to_vec();
      }

# endif


    } // end of namespace oln::value::color

  } // end of namespace oln::value

} // end of namespace oln


#endif // ! OLN_CORE_VALUE_COLOR_RGB_HH
