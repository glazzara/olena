// Copyright (C) 2007,2008,2009,2010 EPITA LRDE
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

#ifndef MLN_IO_PLOT_SAVE_IMAGE_SH_HH
# define MLN_IO_PLOT_SAVE_IMAGE_SH_HH

/// \file
///
/// \brief Define some functions to export to gnuplot format as a script shell.
///
/// Theses routines are dedicated to image visualization. The aim is to display
/// image whatever the value used as pixel. The behaviour of gnuplot is more
/// like xv than imageMagick. A Gnuplot script shell file is a text dump file
/// with a preambule to let gnuplot interpret data. As a script shell, you can
/// launch it (don't forget the permissions), and every thing is packed in it.
/// The script file call gnuplot in batch mode, the result window persists and
/// that's all.
///
/// The following sample is a typical use of the gnuplot shell export library.
///
/// #include <mln/accu/stat/histo1d.hh>
/// #include <mln/data/compute.hh>
/// #include <mln/core/image/image1d.hh>
/// #include <mln/core/image/image2d.hh>
/// #include <mln/img_path.hh>
/// #include <mln/io/pgm/load.hh>
/// #include <mln/io/plot/save_image_sh.hh>
/// #include <mln/value/int_u8.hh>
///
/// int main()
/// {
///   typedef mln::value::int_u8 t_int_u8;
///   mln::image2d<t_int_u8>     img;
///   mln::image1d<unsigned>     histo;
///
///   mln::io::pgm::load(img, OLENA_IMG_PATH"/lena.pgm");
///   histo = mln::data::compute(mln::accu::meta::histo::histo1d(), img);
///   mln::io::plot::save_image_sh(histo, "histo.sh");
///
///   return 0;
/// }

# include <fstream>
# include <string>

# include <mln/trace/entering.hh>
# include <mln/trace/exiting.hh>

# include <mln/core/macros.hh>
# include <mln/core/contract.hh>
# include <mln/core/image/image1d.hh>
# include <mln/core/image/image2d.hh>
# include <mln/core/image/image3d.hh>

# include <mln/geom/min_ind.hh>
# include <mln/geom/max_ind.hh>
# include <mln/geom/min_row.hh>
# include <mln/geom/max_row.hh>
# include <mln/geom/min_col.hh>
# include <mln/geom/max_col.hh>

# include <mln/trait/value_.hh>

# include <mln/value/int_u.hh>
# include <mln/value/int_s.hh>
# include <mln/value/rgb.hh>
# include <mln/value/hsl.hh>
# include <mln/value/hsi.hh>

# include <mln/util/array.hh>


namespace mln
{

  namespace io
  {

    namespace plot
    {

      /// \brief Save an image as a gnuplot script shell.
      ///
      /// Every thing is save. The image could be 1d, 2d or 3d. The value of
      /// the pixels could be int_u<n>, int_s<n>, float, double, hsl_f, hsl_d,
      /// hsi_f, hsi_d and rgb<n>.
      ///
      /// \param[in] img      the image which contains the data to save.
      /// \param[in] filename the name of the unix script shell.
      /// \return             the status of the opening file operation.
      ///
      /// The result depends on the permission to save the file with
      /// filename parameter as unix path. The script shell file must have the
      /// permission to execute (chmod 755). Launch the script shell to call
      /// gnuplot in batchmode with fine parameters.
      template <typename I>
      bool save_image_sh(const Image<I>& img, const std::string& filename);

      /// \brief Save a stack of image.
      ///
      /// This is an experimental support.
      ///
      /// \param[in] stack    the stack of image to save.
      /// \param[in] filename the name of the unix script shell.
      /// \return             the status of the opening file operation.
      ///
      /// The result depends on the permission to save the file with
      /// filename parameter as unix path. The script shell file must have the
      /// permission to execute (chmod 755). Launch the script shell to call
      /// gnuplot in batchmode with fine parameters.
      template <typename I>
      bool save_image_sh(const util::array< image1d<I> >& stack,
			 const std::string& filename);

      /// \brief Save a stack of stack of image.
      ///
      /// This is an experimental support.
      ///
      /// \param[in] stack    the stack of image to save.
      /// \param[in] filename the name of the unix script shell.
      /// \return             the status of the opening file operation.
      ///
      /// The result depends on the permission to save the file with
      /// filename parameter as unix path. The script shell file must have the
      /// permission to execute (chmod 755). Launch the script shell to call
      /// gnuplot in batchmode with fine parameters.
      template <typename I>
      bool save_image_sh(const util::array< util::array< image1d<I> > >& stack,
			 const std::string& filename);

    } // end of namespace mln::io::plot

  } // end of namespace mln::io


  namespace io
  {

    namespace plot
    {

#ifndef MLN_INCLUDE_ONLY

      //------------------------------------------------------------------------
      // Impl.
      //------------------------------------------------------------------------

      namespace impl
      {


	//----------------------------------------------------------------------
	// save_image_sh_array_array_image1d(const array<array<image1d<I>>>&,
	//                                   const string&)
	//----------------------------------------------------------------------

	template <typename I>
	inline
	bool save_image_sh_array_array_image1d(const util::array< util::array<
					       image1d<I> > >&        stack,
					       const std::string&     filename)
	{
	  trace::entering("mln::io::plot::impl::"
			  "save_image_sh_array_array_image1d");

	  mln_precondition(!stack.is_empty());
	  mln_precondition(!stack[0].is_empty());
	  mln_precondition(stack[0][0].is_valid());

	  std::ofstream           out(filename.c_str());
	  bool                    result  = !out.fail();
	  unsigned                min_ind = geom::min_ind(stack[0][0]);
	  unsigned                max_ind = geom::max_ind(stack[0][0]);

	  if (result)
	  {
	    // Output data prelude (terminal X11, image).
	    out << "#!/bin/sh"                                     << std::endl;
	    out << "####################################"          << std::endl;
	    out << "# Columns = (x, y, val)            #"          << std::endl;
	    out << "####################################"          << std::endl;
	    out                                                    << std::endl;
	    out << "gnuplot <<EOF"                                 << std::endl;
	    out << "set terminal x11 persist 1"                    << std::endl;
	    out << "set xrange ["                                  << min_ind;
	    out << ":"                                             << max_ind;
	    out << "]"                                             << std::endl;
	    out << "plot '-' with line";

	    for (unsigned i = 1; i < stack.size(); ++i)
	    {
	      for (unsigned j = 1; j < stack[i].size(); ++j)
	      {

		out << ",\\"                                       << std::endl;
		out << "     '-' with line";
	      }
	    }

	    out                                                    << std::endl;

	    mln_eiter(util::array< util::array< image1d<I> > >) e0(stack);

	    for_all (e0)
	    {
	      mln_eiter(util::array< image1d<I> >) e1(stack[e0.index_()]);

	      for_all (e1)
	      {
		mln_piter(image1d< I >)
		  p(stack[e0.index_()][e1.index_()].domain());

		// Output data.
		for_all(p)
		{
		  out << p.ind()            << " ";
		  out << stack[e0.index_()][e1.index_()](p)   << std::endl;;
		}

	      // Close gnuplot data stream.
	      out << "e"                                           << std::endl;
	      }
	    }

	    out << "EOF"                                           << std::endl;
	    out.close();
	  }
	  else
	  {
	    std::cerr << "ERROR[mln::io::plot::save_image_sh]:"    << filename;
	    std::cerr << " couldn't be opened !!"                  << std::endl;
	  }

	  trace::exiting("mln::io::plot::impl::"
			 "save_image_sh_array_array_image1d");

	  return result;
	}

	//----------------------------------------------------------------------
	// save_image_sh_array_array_image1d_vec3(
	//                              const array<array<image1d<vec<3,T>>>>&,
	//                              const string&)
	//----------------------------------------------------------------------

	template <typename T>
	inline
	bool save_image_sh_array_array_image1d_vec3(
	  const util::array<util::array<image1d<algebra::vec<3,T> > > >& stack,
	  const std::string&                                           filename)
	{
	  trace::entering("mln::io::plot::impl::"
			  "save_image_sh_array_array_image1d_vec3");

	  typedef algebra::vec<3,T>    t_val;
	  typedef image1d<t_val>       t_img;
	  typedef util::array<t_img>   t_array;
	  typedef util::array<t_array> t_stack;

	  mln_precondition(!stack.is_empty());
	  mln_precondition(!stack[0].is_empty());
	  mln_precondition(stack[0][0].is_valid());

	  std::ofstream           out(filename.c_str());
	  bool                    result  = !out.fail();
	  unsigned                min_ind = geom::min_ind(stack[0][0]);
	  unsigned                max_ind = geom::max_ind(stack[0][0]);

	  if (result)
	  {
	    // Output data prelude (terminal X11, image).
	    out << "#!/bin/sh"                                     << std::endl;
	    out << "####################################"          << std::endl;
	    out << "# Columns = (x, y, val)            #"          << std::endl;
	    out << "####################################"          << std::endl;
	    out                                                    << std::endl;
	    out << "gnuplot <<EOF"                                 << std::endl;
	    out << "set terminal x11 persist 1"                    << std::endl;
	    out << "set xrange ["                                  << min_ind;
	    out << ":"                                             << max_ind;
	    out << "]"                                             << std::endl;
	    out << "splot '-' with line palette";

	    for (unsigned i = 1; i < stack.size(); ++i)
	    {
	      for (unsigned j = 1; j < stack[i].size(); ++j)
	      {

		out << ",\\"                                       << std::endl;
		out << "      '-' with line palette";
	      }
	    }

	    out                                                    << std::endl;

	    mln_eiter(t_stack)   e0(stack);

	    for_all (e0)
	    {
	      mln_eiter(t_array) e1(stack[e0.index_()]);

	      for_all (e1)
	      {
		mln_piter(t_img) p(stack[e0.index_()][e1.index_()].domain());

		// Output data.
		for_all(p)
		{
		  out << p.ind()                                  << " ";
		  out << stack[e0.index_()][e1.index_()](p)[0]    << " ";
		  out << stack[e0.index_()][e1.index_()](p)[1]    << " ";
		  out << stack[e0.index_()][e1.index_()](p)[2]    << std::endl;;
		}

	      // Close gnuplot data stream.
	      out << "e"                                           << std::endl;
	      }
	    }

	    out << "EOF"                                           << std::endl;
	    out.close();
	  }
	  else
	  {
	    std::cerr << "ERROR[mln::io::plot::save_image_sh]:"    << filename;
	    std::cerr << " couldn't be opened !!"                  << std::endl;
	  }

	  trace::exiting("mln::io::plot::impl::"
			 "save_image_sh_array_array_image1d_vec3");

	  return result;
	}


	//----------------------------------------------------------------------
	// save_image_sh_array_array_image1d_vec2(
	//                              const array<array<image1d<vec<2,T>>>>&,
	//                              const string&)
	//----------------------------------------------------------------------

	template <typename T>
	inline
	bool save_image_sh_array_array_image1d_vec2(
	  const util::array<util::array<image1d<algebra::vec<2,T> > > >& stack,
	  const std::string&                                           filename)
	{
	  trace::entering("mln::io::plot::impl::"
			  "save_image_sh_array_array_image1d_vec2");

	  typedef algebra::vec<2,T>    t_val;
	  typedef image1d<t_val>       t_img;
	  typedef util::array<t_img>   t_array;
	  typedef util::array<t_array> t_stack;

	  mln_precondition(!stack.is_empty());
	  mln_precondition(!stack[0].is_empty());
	  mln_precondition(stack[0][0].is_valid());

	  std::ofstream           out(filename.c_str());
	  bool                    result  = !out.fail();
	  unsigned                min_ind = geom::min_ind(stack[0][0]);
	  unsigned                max_ind = geom::max_ind(stack[0][0]);

	  if (result)
	  {
	    // Output data prelude (terminal X11, image).
	    out << "#!/bin/sh"                                     << std::endl;
	    out << "####################################"          << std::endl;
	    out << "# Columns = (x, y, val)            #"          << std::endl;
	    out << "####################################"          << std::endl;
	    out                                                    << std::endl;
	    out << "gnuplot <<EOF"                                 << std::endl;
	    out << "set terminal x11 persist 1"                    << std::endl;
	    out << "set xrange ["                                  << min_ind;
	    out << ":"                                             << max_ind;
	    out << "]"                                             << std::endl;
	    out << "splot '-' with line";

	    for (unsigned i = 1; i < stack.size(); ++i)
	    {
	      for (unsigned j = 1; j < stack[i].size(); ++j)
	      {

		out << ",\\"                                       << std::endl;
		out << "      '-' with line";
	      }
	    }

	    out                                                    << std::endl;

	    mln_eiter(t_stack)   e0(stack);

	    for_all (e0)
	    {
	      mln_eiter(t_array) e1(stack[e0.index_()]);

	      for_all (e1)
	      {
		mln_piter(t_img) p(stack[e0.index_()][e1.index_()].domain());

		// Output data.
		for_all(p)
		{
		  out << p.ind()                                  << " ";
		  out << stack[e0.index_()][e1.index_()](p)[0]    << " ";
		  out << stack[e0.index_()][e1.index_()](p)[1]    << std::endl;;
		}

	      // Close gnuplot data stream.
	      out << "e"                                           << std::endl;
	      }
	    }

	    out << "EOF"                                           << std::endl;
	    out.close();
	  }
	  else
	  {
	    std::cerr << "ERROR[mln::io::plot::save_image_sh]:"    << filename;
	    std::cerr << " couldn't be opened !!"                  << std::endl;
	  }

	  trace::exiting("mln::io::plot::impl::"
			 "save_image_sh_array_array_image1d_vec2");

	  return result;
	}

	//----------------------------------------------------------------------
	// save_image_sh_array_image1d(const array<image1d<I>>&, const string&)
	//----------------------------------------------------------------------

	template <typename I>
	inline
	bool save_image_sh_array_image1d(const util::array< image1d<I> >& stack,
					 const std::string&            filename)
	{
	  trace::entering("mln::io::plot::impl::save_image_sh_array_image1d");
	  mln_precondition(!stack.is_empty());
	  mln_precondition(stack[0].is_valid());

	  std::ofstream           out(filename.c_str());
	  bool                    result  = !out.fail();
	  unsigned                min_ind = geom::min_ind(stack[0]);
	  unsigned                max_ind = geom::max_ind(stack[0]);

	  if (result)
	  {
	    // Output data prelude (terminal X11, image).
	    out << "#!/bin/sh"                                     << std::endl;
	    out << "####################################"          << std::endl;
	    out << "# Columns = (x, y, val)            #"          << std::endl;
	    out << "####################################"          << std::endl;
	    out                                                    << std::endl;
	    out << "gnuplot <<EOF"                                 << std::endl;
	    out << "set terminal x11 persist 1"                    << std::endl;
	    out << "set xrange ["                                  << min_ind;
	    out << ":"                                             << max_ind;
	    out << "]"                                             << std::endl;
	    out << "plot '-' with line";

	    for (unsigned i = 1; i < stack.size(); ++i)
	    {

	      out << ",\\"                                         << std::endl;
	      out << "     '-' with line";
	    }

	    out                                                    << std::endl;

	    mln_eiter(util::array< image1d<I> >) e(stack);

	    for_all (e)
	    {
	      mln_piter(image1d< I >) p(stack[e.index_()].domain());

	      // Output data.
	      for_all(p)
	      {
		out << p.ind()       << " ";
		out << stack[e.index_()](p)   << std::endl;;
	      }

	      // Close gnuplot data stream.
	      out << "e"                                           << std::endl;
	    }

	    out << "EOF"                                           << std::endl;
	    out.close();
	  }
	  else
	  {
	    std::cerr << "ERROR[mln::io::plot::save_image_sh]:"    << filename;
	    std::cerr << " couldn't be opened !!"                  << std::endl;
	  }

	  trace::exiting("mln::io::plot::impl::save_image_sh_array_image1d");
	  return result;
	}


	//----------------------------------------------------------------------
	// save_image_sh_image2d_rgb(const image2d<rgb<n>>&, const string&)
	//----------------------------------------------------------------------

	template <unsigned n>
	inline
	bool save_image_sh_image2d_rgb(const image2d<value::rgb<n> >& img,
				       const std::string&             filename)
	{
	  trace::entering("mln::io::plot::impl::save_image_sh_image2d_rgb");
	  mln_precondition(img.is_valid());

	  std::ofstream           out(filename.c_str());
	  bool                    result  = !out.fail();
	  unsigned                min_row = geom::min_row(img);
	  unsigned                max_row = geom::max_row(img);
	  unsigned                min_col = geom::min_col(img);
	  unsigned                max_col = geom::max_col(img);

	  if (result)
	  {
	    mln_piter(image2d< value::rgb<n> >) p(img.domain());

	    // Output data prelude (terminal X11, image).
	    out << "#!/bin/sh"                                     << std::endl;
	    out << "####################################"          << std::endl;
	    out << "# Columns = (x, y, r, g, b)        #"          << std::endl;
	    out << "####################################"          << std::endl;
	    out                                                    << std::endl;
	    out << "gnuplot <<EOF"                                 << std::endl;
	    out << "set terminal x11 persist 1"                    << std::endl;
	    out << "set xrange ["                                  << min_col;
	    out << ":"                                             << max_col;
	    out << "]"                                             << std::endl;
	    out << "set yrange [-"                                 << max_row;
	    out << ":"                                             << min_row;
	    out << "]"                                             << std::endl;
	    out << "plot '-' using 2:(-\\$1):3:4:5 with rgbimage"  << std::endl;

	    // Output data.
	    for_all(p)
	    {
	      out << p.row()        << " ";
	      out << p.col()        << " ";
	      out << img(p).red()   << " ";
	      out << img(p).green() << " ";
	      out << img(p).blue()  << std::endl;
	    }

	    // Close gnuplot data stream.
	    out << "e"                                             << std::endl;
	    out << "EOF"                                           << std::endl;

	    out.close();
	  }
	  else
	  {
	    std::cerr << "ERROR[mln::io::plot::save_image_sh]:"    << filename;
	    std::cerr << " couldn't be opened !!"                  << std::endl;
	  }

	  trace::exiting("mln::io::plot::impl::save_image_sh_image2d_rgb");
	  return result;
	}

	//----------------------------------------------------------------------
	// save_image_sh_image2d_hsl(const image2d<hsl_<T,T,T>>&, const string&)
	//----------------------------------------------------------------------

	template <typename T>
	inline
	bool save_image_sh_image2d_hsl(const image2d<value::hsl_<T,T,T> >& img,
				       const std::string&              filename)
	{
	  trace::entering("mln::io::plot::impl::save_image_sh_image2d_hsl");
	  mln_precondition(img.is_valid());

	  std::ofstream           out(filename.c_str());
	  bool                    result  = !out.fail();
	  unsigned                min_row = geom::min_row(img);
	  unsigned                max_row = geom::max_row(img);
	  unsigned                min_col = geom::min_col(img);
	  unsigned                max_col = geom::max_col(img);

	  typedef mln::value::hsl_<T,T,T> hsl;

	  if (result)
	  {
	    mln_piter(image2d< hsl >) p(img.domain());

	    // Output data prelude (terminal X11, image).
	    out << "#!/bin/sh"                                     << std::endl;
	    out << "####################################"          << std::endl;
	    out << "# Columns = (x, y, val)            #"          << std::endl;
	    out << "####################################"          << std::endl;
	    out                                                    << std::endl;
	    out << "gnuplot <<EOF"                                 << std::endl;
	    out << "q(l,s)     = (l < 0.5)? (l*(1+s)):(l+s-(l*s))" << std::endl;
	    out << "p(l,s)     = (2.0 * l - q(l,s))"               << std::endl;
	    out << "n(x)       = (x < 0)?(x+1):(x > 1)?(x-1) : (x)"<< std::endl;
	    out                                                    << std::endl;
	    out << "c(p,q,t)   = (t <(1.0/6.0))?(p+(q-p)*6.0*t):\\"<< std::endl;
	    out << "             (t <(1.0/2.0))?(q) :\\"           << std::endl;
	    out << " (t<(2.0/3.0))?(p+(q-p)*6.0*((2.0/3.0)-t)):\\" << std::endl;
	    out << "            (p)"				   << std::endl;
	    out                                                    << std::endl;
	    out << "r(h,s,l) = c(p(l,s),q(l,s),n(h/360.0+1.0/3.0))"<< std::endl;
	    out << "g(h,s,l) = c(p(l,s),q(l,s),n(h/360.0))"        << std::endl;
	    out << "b(h,s,l) = c(p(l,s),q(l,s),n(h/360.0-1.0/3.0))"<< std::endl;
	    out                                                    << std::endl;
	    out << "set terminal x11 persist 1"                    << std::endl;
	    out << "set palette gray"                              << std::endl;
	    out << "set xrange ["                                  << min_col;
	    out << ":"                                             << max_col;
	    out << "]"                                             << std::endl;
	    out << "set yrange [-"                                 << max_row;
	    out << ":"                                             << min_row;
	    out << "]"                                             << std::endl;
	    out << "plot '-' using 2:(-\\$1):\\"                   << std::endl;
	    out << "           (r(\\$3,\\$4,\\$5)):\\"             << std::endl;
	    out << "           (g(\\$3,\\$4,\\$5)):\\"             << std::endl;
	    out << "           (b(\\$3,\\$4,\\$5)) with rgbimage"  << std::endl;

	    // Output data.
	    for_all(p)
	    {
	      out << p.row()      << " ";
	      out << p.col()      << " ";
	      out << img(p).hue() << " ";
	      out << img(p).sat() << " ";
	      out << img(p).lum() << std::endl;
	    }

	    // Close gnuplot data stream.
	    out << "e"                                             << std::endl;
	    out << "EOF"                                           << std::endl;

	    out.close();
	  }
	  else
	  {
	    std::cerr << "ERROR[mln::io::plot::save_image_sh]:"    << filename;
	    std::cerr << " couldn't be opened !!"                  << std::endl;
	  }

	  trace::exiting("mln::io::plot::impl::save_image_sh_image2d_hsl");
	  return result;
	}

	//----------------------------------------------------------------------
	// save_image_sh_image2d_hsi(const image2d<hsi_<T,T,T>>&, const string&)
	//----------------------------------------------------------------------

	template <typename T>
	inline
	bool save_image_sh_image2d_hsi(const image2d<value::hsi_<T,T,T> >& img,
				       const std::string&              filename)
	{
	  trace::entering("mln::io::plot::impl::save_image_sh_image2d_hsi");
	  mln_precondition(img.is_valid());

	  std::ofstream           out(filename.c_str());
	  bool                    result  = !out.fail();
	  unsigned                min_row = geom::min_row(img);
	  unsigned                max_row = geom::max_row(img);
	  unsigned                min_col = geom::min_col(img);
	  unsigned                max_col = geom::max_col(img);

	  typedef mln::value::hsi_<T,T,T> hsi;

	  if (result)
	  {
	    mln_piter(image2d< hsi >) p(img.domain());

	    // Output data prelude (terminal X11, image).
	    out << "#!/bin/sh"                                     << std::endl;
	    out << "####################################"          << std::endl;
	    out << "# Columns = (x, y, val)            #"          << std::endl;
	    out << "####################################"          << std::endl;
	    out                                                    << std::endl;
	    out << "gnuplot <<EOF"                                 << std::endl;
	    out << "teta(h)    = (h/180.0) * pi"                   << std::endl;
	    out << "alpha(s,h) = s*cos(teta(h))"                   << std::endl;
	    out << "beta(s,h)  = s*sin(teta(h))"                   << std::endl;
	    out << "n(x)       = (x < 0)?(x+1) : (x > 1)?(x-1):(x)"<< std::endl;
	    out                                                    << std::endl;
	    out << "c(p,q,t)   = (t <(1.0/6.0))?(p+(q-p)*6.0*t):\\"<< std::endl;
	    out << "             (t <(1.0/2.0))?(q) :\\"           << std::endl;
	    out << " (t <(2.0/3.0))?(p+(q-p)*6.0*((2.0/3.0)-t)):\\"<< std::endl;
	    out << "             (p)"				   << std::endl;
	    out                                                    << std::endl;
	    out << "r(h,s,i)   = (sqrt(3.0)/3.0) * i \\"           << std::endl;
	    out << "           + (2.0/(sqrt(6.0))) * beta(s,h)"    << std::endl;
	    out << "g(h,s,i)   = (sqrt(3.0)/3.0) * i \\"           << std::endl;
	    out << "           + (2.0/(sqrt(2.0))) * alpha(s,h)\\" << std::endl;
	    out << "             - (1.0/(sqrt(6.0))) * beta(s,h)"  << std::endl;
	    out << "b(h,s,i)   = (sqrt(3.0)/3.0) * i \\"           << std::endl;
	    out << "           - (2.0/(sqrt(2.0))) * alpha(s,h) \\"<< std::endl;
	    out << "           - (1.0/(sqrt(6.0))) * beta(s,h)"    << std::endl;
	    out                                                    << std::endl;
	    out << "set terminal x11 persist 1"                    << std::endl;
	    out << "set palette gray"                              << std::endl;
	    out << "set xrange ["                                  << min_col;
	    out << ":"                                             << max_col;
	    out << "]"                                             << std::endl;
	    out << "set yrange [-"                                 << max_row;
	    out << ":"                                             << min_row;
	    out << "]"                                             << std::endl;
	    out << "plot '-' using 2:(-\\$1):\\"                   << std::endl;
	    out << "           (r(\\$3,\\$4,\\$5)):\\"             << std::endl;
	    out << "           (g(\\$3,\\$4,\\$5)):\\"             << std::endl;
	    out << "           (b(\\$3,\\$4,\\$5)) with rgbimage"  << std::endl;

	    // Output data.
	    for_all(p)
	    {
	      out << p.row()       << " ";
	      out << p.col()       << " ";
	      out << img(p).hue()  << " ";
	      out << img(p).sat()  << " ";
	      out << img(p).inty() << std::endl;
	    }

	    // Close gnuplot data stream.
	    out << "e"                                             << std::endl;
	    out << "EOF"                                           << std::endl;

	    out.close();
	  }
	  else
	  {
	    std::cerr << "ERROR[mln::io::plot::save_image_sh]:"    << filename;
	    std::cerr << " couldn't be opened !!"                  << std::endl;
	  }

	  trace::exiting("mln::io::plot::impl::save_image_sh_image2d_hsi");
	  return result;
	}

	//----------------------------------------------------------------------
	// save_image_sh_image2d(const image2d<I>&, const string&)
	//----------------------------------------------------------------------

	template <typename I>
	inline
	bool save_image_sh_image2d(const image2d<I>&  img,
				   const std::string& filename)
	{
	  trace::entering("mln::io::plot::impl::save_image_sh_image2d");
	  mln_precondition(img.is_valid());

	  std::ofstream           out(filename.c_str());
	  bool                    result  = !out.fail();
	  unsigned                min_row = geom::min_row(img);
	  unsigned                max_row = geom::max_row(img);
	  unsigned                min_col = geom::min_col(img);
	  unsigned                max_col = geom::max_col(img);

	  if (result)
	  {
	    mln_piter(image2d<I>) p(img.domain());

	    // Output data prelude (terminal X11, image).
	    out << "#!/bin/sh"                                 << std::endl;
	    out << "####################################"      << std::endl;
	    out << "# Columns = (x, y, val)            #"      << std::endl;
	    out << "####################################"      << std::endl;
	    out                                                << std::endl;
	    out << "gnuplot <<EOF"                             << std::endl;
	    out << "set terminal x11 persist 1"                << std::endl;
	    out << "set palette gray"                          << std::endl;
	    out << "set xrange ["                              << min_col;
	    out << ":"                                         << max_col;
	    out << "]"                                         << std::endl;
	    out << "set yrange [-"                             << max_row;
	    out << ":"                                         << min_row;
	    out << "]"                                         << std::endl;
	    out << "plot '-' using 2:(-\\$1):3 with image"     <<std::endl;

	    // Output data.
	    for_all(p)
	    {
	      out << p.row()   << " ";
	      out << p.col()   << " ";
	      out << img(p)    << std::endl;
	    }

	    // Close gnuplot data stream.
	    out << "e"                                         << std::endl;
	    out << "EOF"                                       << std::endl;

	    out.close();
	  }
	  else
	  {
	    std::cerr << "ERROR[mln::io::plot::save_image_sh]:"<< filename;
	    std::cerr << " couldn't be opened !!"              << std::endl;
	  }

	  trace::exiting("mln::io::plot::impl::save_image_sh_image2d");
	  return result;
	}

	//----------------------------------------------------------------------
	// save_image_sh_image1d(const image1d<I>&, const string&)
	//----------------------------------------------------------------------

	template <typename I>
	inline
	bool save_image_sh_image1d(const image1d<I>&  img,
				   const std::string& filename)
	{
	  trace::entering("mln::io::plot::impl::save_image_sh_image1d");
	  mln_precondition(img.is_valid());

	  std::ofstream           out(filename.c_str());
	  bool                    result = !out.fail();

	  if (result)
	  {
	    mln_piter(image1d<I>) p(img.domain());

	    // Output data prelude (terminal X11, impulse).
	    out << "#!/bin/sh"                                  << std::endl;
	    out << "##########################"                 << std::endl;
	    out << "# Two columns = (x, val) #"                 << std::endl;
	    out << "##########################"                 << std::endl;
	    out                                                 << std::endl;
	    out << "gnuplot <<EOF"                              << std::endl;
	    out << "set terminal x11 persist 1"                 << std::endl;
	    out << "plot '-' with impulse"                      << std::endl;

	    // Output data.
	    for_all(p)
	    {
	      out << p.ind() << " ";
	      out << img(p)  << std::endl;
	    }

	    // Close gnuplot data stream.
	    out << "e"                                         << std::endl;
	    out << "EOF"                                       << std::endl;

	    out.close();
	  }
	  else
	  {
	    std::cerr << "ERROR[mln::io::plot::save_image_sh]:"<< filename;
	    std::cerr << " couldn't be opened !!"              << std::endl;
	  }

	  trace::exiting("mln::io::plot::impl::save_image_sh_image1d");
	  return result;
	}

	//----------------------------------------------------------------------
	// save_image_sh_image3d(const image3d<I>&, const string&)
	//----------------------------------------------------------------------


	template <typename I>
	inline
	bool save_image_sh_image3d(const image3d<I>&  img,
				   const std::string& filename)
	{
	  trace::entering("mln::io::plot::impl::save_image_sh_image3d");
	  mln_precondition(img.is_valid());

	  std::ofstream           out(filename.c_str());
	  bool                    result = !out.fail();

	  if (result)
	  {
	    mln_piter(image3d<I>) p(img.domain());

	    // Output data prelude (terminal X11, pointtype 0).
	    out << "#!/bin/sh"                                 << std::endl;
	    out << "####################################"      << std::endl;
	    out << "# Columns = (x, y, z, val)         #"      << std::endl;
	    out << "####################################"      << std::endl;
	    out                                                << std::endl;
	    out << "gnuplot <<EOF"                             << std::endl;
	    out << "set terminal x11 persist 1"                << std::endl;
	    out << "splot '-' with points palette pointtype 7" << std::endl;

	    // Output data.
	    for_all(p)
	    {
	      out << p.row()   << " ";
	      out << p.col()   << " ";
	      out << p.sli()   << " ";
	      out << img(p)    << std::endl;
	    }

	    // Close gnuplot data stream.
	    out << "e"                                         << std::endl;
	    out << "EOF"                                       << std::endl;

	    out.close();
	  }
	  else
	  {
	    std::cerr << "ERROR[mln::io::plot::save_image_sh]:"<< filename;
	    std::cerr << " couldn't be opened !!"              << std::endl;
	  }

	  trace::exiting("mln::io::plot::impl::save_image_sh_image3d");
	  return result;
	}

      } // end of namespace impl


      //------------------------------------------------------------------------
      // Internal.
      //------------------------------------------------------------------------

      namespace internal
      {
	template <typename I>
	inline
	bool save_image_sh_dispatch(const util::array< image1d<I> >& stack,
				    const std::string&             filename)
	{
	  return impl::save_image_sh_array_image1d(stack, filename);
	}

	template <typename I>
	inline
	bool save_image_sh_dispatch(const util::array<
				    util::array <image1d<I> > >& stack,
				    const std::string&           filename)
	{
	  return impl::save_image_sh_array_array_image1d(stack, filename);
	}

	template <typename T>
	inline
	bool save_image_sh_dispatch(
	  const util::array<util::array<image1d<algebra::vec<3,T> > > >& stack,
	  const std::string&                                           filename)
	{
	  return impl::save_image_sh_array_array_image1d_vec3(stack, filename);
	}

	template <typename T>
	inline
	bool save_image_sh_dispatch(
	  const util::array<util::array<image1d<algebra::vec<2,T> > > >& stack,
	  const std::string&                                           filename)
	{
	  return impl::save_image_sh_array_array_image1d_vec2(stack, filename);
	}

	template <unsigned n>
	inline
	bool save_image_sh_dispatch(const image2d<value::rgb<n> >& img,
				    const std::string&             filename)
	{
	  return impl::save_image_sh_image2d_rgb(img, filename);
	}

	template <typename T>
	inline
	bool save_image_sh_dispatch(const image2d<value::hsl_<T,T,T> >& img,
				    const std::string&                 filename)
	{
	  return impl::save_image_sh_image2d_hsl(img, filename);
	}

	template <typename T>
	inline
	bool save_image_sh_dispatch(const image2d<value::hsi_<T,T,T> >& img,
				    const std::string&                 filename)
	{
	  return impl::save_image_sh_image2d_hsi(img, filename);
	}


	template <typename I>
	inline
	bool save_image_sh_dispatch(const image2d<I>&  img,
				    const std::string& filename)
	{
	  return impl::save_image_sh_image2d(img, filename);
	}

	template <typename I>
	inline
	bool save_image_sh_dispatch(const image1d<I>&  img,
				    const std::string& filename)
	{
	  return impl::save_image_sh_image1d(img, filename);
	}

	template <typename I>
	inline
	bool save_image_sh_dispatch(const image3d<I>&  img,
				    const std::string& filename)
	{
	  return impl::save_image_sh_image3d(img, filename);
	}

	template <typename I>
	inline
	bool save_image_sh_dispatch(const Image<I>&    img,
				    const std::string& filename)
	{
	  return save_image_sh_dispatch(exact(img), filename);
	}

      } // end of namespace mln::io::plot::internal

      //------------------------------------------------------------------------
      // Facade.
      //------------------------------------------------------------------------

      template <typename I>
      inline
      bool save_image_sh(const Image<I>& img, const std::string& filename)
      {
	trace::entering("mln::io::plot::save_image_sh");

	bool result = internal::save_image_sh_dispatch(img, filename);

	trace::exiting("mln::io::plot::save_image_sh");
	return result;
      }

      template <typename I>
      inline
      bool save_image_sh(const util::array< image1d<I> >& stack,
			 const std::string& filename)
      {
	trace::entering("mln::io::plot::save_image_sh");

	bool result = internal::save_image_sh_dispatch(stack, filename);

	trace::exiting("mln::io::plot::save_image_sh");
	return result;
      }

      template <typename I>
      inline
      bool save_image_sh(const util::array< util::array< image1d<I> > >& stack,
			 const std::string& filename)
      {
	trace::entering("mln::io::plot::save_image_sh");

	bool result = internal::save_image_sh_dispatch(stack, filename);

	trace::exiting("mln::io::plot::save_image_sh");
	return result;
      }


#endif // ! MLN_INCLUDE_ONLY


    } // end of namespace mln::io::plot

  } // end of namespace mln::io

} // end of namespace mln

#endif // ! MLN_IO_PLOT_SAVE_IMAGE_SH_HH
