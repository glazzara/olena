// Copyright (C) 2010, 2011 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef SCRIBO_TOOLCHAIN_INTERNAL_TOOLCHAIN_FUNCTOR_HH
# define SCRIBO_TOOLCHAIN_INTERNAL_TOOLCHAIN_FUNCTOR_HH

# include <unistd.h>
# include <iostream>

namespace scribo
{

  namespace toolchain
  {

    namespace internal
    {

      class Toolchain_Functor
      {
      public:

	Toolchain_Functor();
        virtual ~Toolchain_Functor();

	virtual int nsteps() const = 0;


	//==========
	// Triggers
	//==========

	virtual void on_start();
	virtual void on_end();
	virtual void on_progress();
	virtual void on_new_progress_label(const char *label);

	// Attributes
	bool verbose;
      };


# ifndef MLN_INCLUDE_ONLY

      inline
      Toolchain_Functor::Toolchain_Functor()
	: verbose(true)
      {
      }

      Toolchain_Functor::~Toolchain_Functor()
      {
      }

      //==========
      // Triggers
      //==========


      inline
      void Toolchain_Functor::on_start()
      {
	// Nothing
      }

      inline
      void Toolchain_Functor::on_end()
      {
	// Nothing
      }

      inline
      void Toolchain_Functor::on_progress()
      {
	// Nothing
	if (verbose)
	  std::cout << std::endl;
      }

      inline
      void Toolchain_Functor::on_new_progress_label(const char *label)
      {
	if (verbose)
	{
	  if (isatty(1))
	    std::cout << "> \e[0;32m " << label << " \e[m - ";
	  else
	    std::cout << "> " << label << " - ";
	}
      }

# endif // ! MLN_INCLUDE_ONLY


    } // end of namespace scribo::toolchain::internal

  } // end of namespace scribo::toolchain

} // end of namespace scribo

#endif // ! SCRIBO_TOOLCHAIN_INTERNAL_TOOLCHAIN_FUNCTOR_HH
