// Copyright (C) 2013 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_BINARIZATION_INTERNAL_GLOBAL_MAX_STDDEV_FUNCTOR_HH
# define SCRIBO_BINARIZATION_INTERNAL_GLOBAL_MAX_STDDEV_FUNCTOR_HH

/// \file
///
/// \brief Functor computing the maximum standard deviation during an
/// integral image browsing.


namespace scribo
{

  namespace binarization
  {

    namespace internal
    {

      /*! \internal \brief Functor computing the maximum standard
	deviation during an integral image browsing.

	It is used in Wolf's algorithm.
      */
      template <typename I>
      struct global_max_stddev
      {
	global_max_stddev();

	void init();

	void exec(double mean, double stddev);

	void end_of_row(int);

	void finalize();

	double max_stddev;
      };


# ifndef MLN_INCLUDE_ONLY

      template <typename I>
      global_max_stddev<I>::global_max_stddev()
	: max_stddev(0)
      {
      }

      template <typename I>
      void
      global_max_stddev<I>::init()
      {
      }

      template <typename I>
      void
      global_max_stddev<I>::exec(double mean, double stddev)
      {
	(void) mean;
	if (max_stddev < stddev)
	  max_stddev = stddev;
      }

      template <typename I>
      void
      global_max_stddev<I>::end_of_row(int)
      {
      }

      template <typename I>
      void
      global_max_stddev<I>::finalize()
      {
      }

    } // end of namespace scribo::binarization::internal

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::binarization

} // end of namespace scribo


#endif // ! SCRIBO_BINARIZATION_INTERNAL_GLOBAL_MAX_STDDEV_FUNCTOR_HH
