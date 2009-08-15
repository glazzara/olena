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

#ifndef MLN_WALUE_RGB_T_HH
# define MLN_WALUE_RGB_T_HH

/// \file
/// \brief FIXME

# include <mln/walue/internal/rgb_base.hh>


namespace mln
{

  namespace walue
  {

    // Forward declaration.
    template <typename T> class rgb_t;


    // FIXME...


    /// \brief FIXME
    //
    template <typename T>
    class rgb_t : public internal::rgb_base< T, T, T,
					     rgb_t<T> >
    {
      typedef internal::rgb_base< T, T, T,
				  rgb_t<T> > super_;
    public:

      // Constructor without argument.
      rgb_t();

      rgb_t(float r, float g, float b);

      template <typename T>
      rgb_t(const internal::encoding<T,T,T>& e)
	: super_(set_encoding(e.t0),
		 set_encoding(e.t1),
		 set_encoding(e.t2))
      {
      }

    };



# ifndef MLN_INCLUDE_ONLY

    template <typename T>
    inline
    rgb_t<T>::rgb_t()
    {
    }

    template <typename T>
    inline
    rgb_t<T>::rgb_t(float r, float g, float b)
      : super_(r, g, b)
    {
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::walue

} // end of namespace mln


#endif // ! MLN_WALUE_RGB_T_HH
