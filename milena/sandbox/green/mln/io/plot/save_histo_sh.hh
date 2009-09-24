// Copyright (C) 2007 EPITA Research and Development Laboratory (LRDE)
// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_IO_PLOT_SAVE_HISTO_SH_HH
#define MLN_IO_PLOT_SAVE_HISTO_SH_HH

/// \file
///
/// \brief Define some functions to export to gnuplot format as a script shell.
///
/// Theses routines help to visualize histogram data.
///

#include <fstream>
#include <string>

#include <mln/core/macros.hh>
#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>


namespace mln
{
  
  namespace io
  {
    
    namespace plot
    {
   
      /// \brief Save an histogram image1d as a gnuplot script shell.
      /// \param[in] img      the 1d image which contains the data to save.
      /// \param[in] filename the name of the unix script shell.
      ///
      /// Such image is obtained from the greylevel image histogram.
      
      template <typename I>
      void save_histo_sh(const image1d<I>& img, const std::string& filename);


      //template <typename I>
      //void save_histo_sh(const image2d<I>& img, const std::string& filename);

      /// \brief Save an histogram image3d as a gnuplot script shell.
      /// \param[in] img      the 3d image which contains the data to save.
      /// \param[in] filename the name of the unix script shell.
      ///
      /// Such image is obtained from the RGB image histogram.
      template <typename I>
      void save_histo_sh(const image3d<I>& img, const std::string& filename);

    } // end of namespace mln::accu::stat
  
  } // end of namespace mln::accu


  namespace io
  {
    
    namespace plot
    {
      
#ifndef MLN_INCLUDE_ONLY

      template <typename I>
      inline
      void save_histo_sh(const image1d<I>& img, const std::string& filename)
      {
	trace::entering("mln::io::plot::save_histo_sh<1d>");
	mln_precondition(img.is_valid());

	std::ofstream         out(filename.c_str());
	mln_piter(image1d<I>) p(img.domain());

	out << "#!/bin/sh"                                  << std::endl;
	out << "##########################"                 << std::endl;
	out << "# Two columns = (x,data) #"                 << std::endl;
	out << "##########################"                 << std::endl;
	out                                                 << std::endl;
	out << "gnuplot <<EOF"                              << std::endl;
	out << "set terminal x11 persist 1"                 << std::endl;
	out << "plot '-' with impulses"                     << std::endl;
	//out << "plot '-' with points pointtype 20"          << std::endl;
	//out << "plot '-' with points pointtype 0"  << std::endl;
	//out << "plot '-' with histograms"  << std::endl;
	//out << "plot '-' smooth unique"  << std::endl;

	for_all(p)
	{
	  //	  for (unsigned i = 0; i < mln_dim(I); ++i)
	  out << p[0] << " ";

	  out << img(p) << std::endl;
	}
	out << "e"                                         << std::endl;
	out << "EOF"                                       << std::endl;

	out.close();
	trace::exiting("mln::io::plot::save_histo_sh<1d>");
      }
      /*
      template <typename I>
      inline
      void save_histo_sh(const image2d<I>& img, const std::string& filename)
      {
	trace::entering("mln::io::plot::save_histo_sh<2d>");
	mln_precondition(img.is_valid());

	std::ofstream     out(filename.c_str());
	mln_piter(image2d<I>)      p(img.domain());

	out << "#!/bin/sh"                                 << std::endl;
	out << "####################################"      << std::endl;
	out << "# Columns = (x,y,val)              #"      << std::endl;
	out << "####################################"      << std::endl;
	out                                                << std::endl;
	out << "gnuplot <<EOF"                             << std::endl;
	out << "set terminal x11 persist 1"                << std::endl;
	out << "splot '-' with points palette pointtype 0" << std::endl;
	
	for_all(p)
	//if (0 < img(p))
	    {
	      out << p.row()   << " ";
	      out << p.col()   << " ";
	      out << img(p)    << std::endl;
	    }
	out << "e"                                         << std::endl;
	out << "EOF"                                       << std::endl;

	out.close();
	trace::exiting("mln::io::plot::save_histo_sh<2d>");
      }

      */
      template <typename I>
      inline
      void save_histo_sh(const image3d<I>& img, const std::string& filename)
      {
	trace::entering("mln::io::plot::save_histo_sh<3d>");
	mln_precondition(img.is_valid());

	std::ofstream     out(filename.c_str());
	mln_piter(image3d<I>)      p(img.domain());

	out << "#!/bin/sh"                                 << std::endl;
	out << "####################################"      << std::endl;
	out << "# Columns = (red,green,blue,count) #"      << std::endl;
	out << "####################################"      << std::endl;
	out                                                << std::endl;
	out << "gnuplot <<EOF"                             << std::endl;
	out << "set terminal x11 persist 1"                << std::endl;
	out << "splot '-' with points palette pointtype 0" << std::endl;
	
	for_all(p)
	  if (0 < img(p))
	    {
	      out << p.row()   << " ";
	      out << p.col()   << " ";
	      out << p.sli()   << " ";
	      out << img(p)    << std::endl;
	    }
	out << "e"                                         << std::endl;
	out << "EOF"                                       << std::endl;

	out.close();
	trace::exiting("mln::io::plot::save_histo_sh<3d>");
      }

#endif // ! MLN_INCLUDE_ONLY


    } // end of namespace mln::accu::stat

  } // end of namespace mln::accu

} // end of namespace mln

#endif // ! MLN_IO_PLOT_SAVE_HISTO_SH_HH
