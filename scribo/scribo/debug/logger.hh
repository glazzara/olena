// Copyright (C) 2011 EPITA Research and Development Laboratory
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

#ifndef SCRIBO_CORE_LOGGER_HH
# define SCRIBO_CORE_LOGGER_HH

# include <mln/core/concept/image.hh>
# include <mln/data/wrap.hh>
# include <mln/value/int_u8.hh>
# include <mln/debug/filename.hh>
# include <mln/io/pbm/save.hh>
# include <mln/io/pgm/save.hh>
# include <mln/io/ppm/save.hh>

namespace scribo
{

  namespace debug
  {

    enum Level
    {
      None = 0,
      Special, // Reserved
      Results,
      AuxiliaryResults,
      All
    };


    namespace internal
    {
      using namespace mln;

      class logger_
      {
      public:
	static logger_& instance();

	bool is_enabled() const;
	bool is_at_level(Level level) const;

	void set_level(Level level);
	Level level() const;

	void set_filename_prefix(const char *name);
	const char *filename_prefix() const;

	template <typename I>
	void log_image(Level dbg_level,
		       const Image<I>& ima, const char *name);

      private: // Methods
	logger_();
	logger_(const logger_&);

	template <unsigned n, typename I>
	void log_image_dispatch(const value::int_u<n>&,
				const Image<I>& ima, const char *name);

	template <unsigned n, typename I>
	void
	log_image_dispatch(const value::label<n>&,
			   const Image<I>& ima, const char *name);

	template <typename I>
	void log_image_dispatch(const mln_value(I)&,
				const Image<I>& ima, const char *name);

	template <typename I>
	void log_image_dispatch(const value::rgb8&,
				const Image<I>& ima, const char *name);

	template <typename I>
	void log_image_dispatch(const bool&,
				const Image<I>& ima, const char *name);

	template <typename I>
	void log_image_rgb(const I& ima, const std::string& name);

	template <typename I>
	void log_image_bool(const I& ima, const std::string& name);

	template <typename I>
	void log_image_grayscale(const I& ima, const std::string& name);

      private: // Attributes
	Level level_;

      };

    } // end of namespace scribo::debug::internal


    // Return a reference to the logger.
    scribo::debug::internal::logger_& logger();


# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {

      inline
      logger_&
      logger_::instance()
      {
	static logger_ log;
	return log;
      }


      inline
      logger_::logger_()
	: level_(None)
      {
	// Magick::InitializeMagick(0);
      }


      inline
      logger_::logger_(const logger_&)
      {
	abort();
      }


      inline
      bool
      logger_::is_enabled() const
      {
	return level_ != None;
      }


      inline
      bool
      logger_::is_at_level(Level level) const
      {
	return level_ >= level;
      }


      inline
      void
      logger_::set_filename_prefix(const char *name)
      {
	mln::debug::internal::filename_prefix = name;
      }


      inline
      const char *
      logger_::filename_prefix() const
      {
	return mln::debug::internal::filename_prefix.c_str();
      }


      inline
      void logger_::set_level(Level level)
      {
	level_ = level;
      }


      inline
      Level logger_::level() const
      {
	return level_;
      }


      template <typename I>
      void
      logger_::log_image(Level dbg_level,
			const Image<I>& ima, const char *name)
      {
# ifndef SCRIBO_NDEBUG
	if (level_ < dbg_level)
	  return;

	std::cerr << "Logger - Saving image " << name << std::endl;
	mln_assertion(exact(ima).is_valid());

	typedef mln_value(I) V;
	log_image_dispatch(V(), ima, name);
# endif // ! SCRIBO_NDEBUG
      }


      template <unsigned n, typename I>
      void
      logger_::log_image_dispatch(const value::int_u<n>&, const Image<I>& ima, const char *name)
      {
	log_image_grayscale(exact(ima), name);
      }

      template <unsigned n, typename I>
      void
      logger_::log_image_dispatch(const value::label<n>&, const Image<I>& ima, const char *name)
      {
	log_image_grayscale(exact(ima), name);
      }


      template <typename I>
      void
      logger_::log_image_dispatch(const value::rgb8&, const Image<I>& ima, const char *name)
      {
	log_image_rgb(exact(ima), name);
      }

      template <typename I>
      void
      logger_::log_image_dispatch(const bool&, const Image<I>& ima, const char *name)
      {
	log_image_bool(exact(ima), name);
      }


      // template <typename I>
      // void
      // logger_::log_image_generic(const I& ima, const std::string& name)
      // {
      // 	mln::io::magick::save(ima, mln::debug::filename(name + ".png"));
      // }

      template <typename I>
      void
      logger_::log_image_rgb(const I& ima, const std::string& name)
      {
	mln::io::ppm::save(ima, mln::debug::filename(name + ".ppm"));
      }


      template <typename I>
      void
      logger_::log_image_bool(const I& ima, const std::string& name)
      {
	mln::io::pbm::save(ima, mln::debug::filename(name + ".pbm"));
      }


      template <typename I>
      void
      logger_::log_image_grayscale(const I& ima, const std::string& name)
      {
	mln::io::pgm::save(data::wrap(value::int_u8(), ima),
			   mln::debug::filename(name + ".pgm"));
      }

    } // end of namespace scribo::debug::internal


    inline
    scribo::debug::internal::logger_&
    logger()
    {
      return scribo::debug::internal::logger_::instance();
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::debug

} // end of namespace scribo

#endif // ! SCRIBO_CORE_LOGGER_HH
