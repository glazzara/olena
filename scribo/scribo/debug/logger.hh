// Copyright (C) 2011, 2012, 2013 EPITA Research and Development
// Laboratory (LRDE)
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

/// \file
///
/// \brief Logger class used to save debug images.

#ifndef SCRIBO_CORE_LOGGER_HH
# define SCRIBO_CORE_LOGGER_HH

# include <iostream>
# include <ctype.h>
# include <mln/core/concept/image.hh>
# include <mln/data/wrap.hh>
# include <mln/value/int_u8.hh>
# include <mln/debug/filename.hh>
# include <mln/io/pbm/save.hh>
# include <mln/io/pgm/save.hh>
# include <mln/io/ppm/save.hh>
# include <mln/util/timer.hh>

namespace scribo
{

  namespace debug
  {

    /*! \brief Enum defining different level of image logging

      According to the debug level set in #logger_ the image may
      be saved or not.
    */
    enum Level
    {
      None = 0,
      Special, // Reserved
      Results,
      AuxiliaryResults,
      All,
      InvalidLevel // Reserved
    };

    /*! \brief Enum defining different modes of text logging

      According to the verbose mode set in #logger_ the text may
      be logged or not.
    */
    enum VerboseMode
    {
      Mute = 0,
      UserDebug, // Reserved
      Time,
      Low,
      Medium,
      Full,
      Invalid, // Reserved
    };


    /*! \brief returns the corresponding verboseMode from its name.

      \internal
      \warning Don't forget to update this function if a VerboseMode
      value is added.
      \endinternal
     */
    VerboseMode txt_to_verbose_mode(const std::string& name);



    namespace internal
    {
      using namespace mln;

      class logger_
      {
      public:
	static logger_& instance();

	/// Text Logging
	/// @{
	bool is_verbose() const;
	bool is_at_verbose_mode(VerboseMode mode) const;

	/// The default verbose mode used while logging with
	/// #operator<<.
	bool set_default_verbose_mode(VerboseMode mode);
	VerboseMode default_verbose_mode() const;

	/// Set the current verbose mode, filtering the debug output
	/// logged through this object.
	bool set_verbose_mode(VerboseMode mode);
	VerboseMode verbose_mode() const;

	void set_verbose_prefix(const std::string& prefix);

	void log(VerboseMode mode, const std::string& text);
	/// @}


	/// Image Logging
	/// @{
	bool is_enabled() const;
	bool is_at_level(Level level) const;

	void set_level(Level level);
	Level level() const;

	void set_filename_prefix(const char *name);
	const char *filename_prefix() const;

	template <typename I>
	void log_image(Level dbg_level,
		       const Image<I>& ima, const char *name);
	/// @}

	/*! \brief Time Logging

	  This class provides two timers in order to performs
	  benchmarks inside a program.

	  Local time logging is meant to be used for benchmarking a
	  small portion of code, whereas global time logging is meant
	  to benchmark a whole program.

	  They can be used both at the same time.

	  Stopping time logging will output the computed time if
	  Verbose mode is higher or equal to #Time.
	 */
	/// @{
	void start_local_time_logging();
	void stop_local_time_logging(const std::string& time_title);

	void start_global_time_logging();
	void stop_global_time_logging(const std::string& time_title);
	/// @}


	/*! \brief Quickly logs text.
	  It uses the default log mode to know if the text
	*/
	template <typename V>
	logger_& operator<<(const V&v);
	logger_& operator<<(std::ostream& (*f)(std::ostream&));

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

	VerboseMode verbose_mode_;
	VerboseMode default_verbose_mode_;
	std::string verbose_prefix_;
	std::ostream& stream_;

	mln::util::timer t_local_;
	mln::util::timer t_global_;
      };

    } // end of namespace scribo::debug::internal


    /*! \brief Return a reference to the logger.

      \ingroup grpalgodebug
     */
    scribo::debug::internal::logger_& logger();


# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {

      inline
      std::string to_upper(const std::string& s)
      {
	std::string out(s);
	for (size_t i = 0; i < s.size(); ++i)
	  out[i] = toupper(s[i]);
	return out;
      }

      inline
      logger_&
      logger_::instance()
      {
	static logger_ log;
	return log;
      }


      inline
      logger_::logger_()
	: level_(None),
	  verbose_mode_(Mute),
	  default_verbose_mode_(Low),
	  verbose_prefix_("LOG: "),
	  stream_(std::cerr)
      {
	// Magick::InitializeMagick(0);
      }


      inline
      logger_::logger_(const logger_&)
	: stream_(std::cerr)
      {
	abort();
      }


      inline
      bool
      logger_::is_verbose() const
      {
	return verbose_mode_ > Mute;
      }


      inline
      bool
      logger_::is_at_verbose_mode(VerboseMode mode) const
      {
	return mode == verbose_mode_;
      }


      inline
      bool
      logger_::set_default_verbose_mode(VerboseMode mode)
      {
	if (mode != Invalid)
	{
	  default_verbose_mode_ = mode;
	  return true;
	}
	return false;
      }


      inline
      VerboseMode
      logger_::default_verbose_mode() const
      {
	return default_verbose_mode_;
      }


      inline
      bool
      logger_::set_verbose_mode(VerboseMode mode)
      {
	if (mode != Invalid)
	{
	  verbose_mode_ = mode;
	  return true;
	}
	return false;
      }


      inline
      VerboseMode
      logger_::verbose_mode() const
      {
	return verbose_mode_;
      }


      inline
      void
      logger_::set_verbose_prefix(const std::string& prefix)
      {
	verbose_prefix_ = prefix;
      }


      inline
      void
      logger_::log(VerboseMode mode, const std::string& text)
      {
# ifndef SCRIBO_NDEBUG
	if (verbose_mode_ < mode)
	  return;

	std::cerr << verbose_prefix_ << text << std::endl;
# endif // ! SCRIBO_NDEBUG
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


      inline
      void
      logger_::start_local_time_logging()
      {
	t_local_.restart();
      }


      inline
      void
      logger_::stop_local_time_logging(const std::string& time_title)
      {
	t_local_.stop();
	if (verbose_mode_ >= Time)
	  std::cerr << time_title << " "  << t_local_ << "s" << std::endl;
      }


      inline
      void
      logger_::start_global_time_logging()
      {
	t_global_.restart();
      }


      inline
      void
      logger_::stop_global_time_logging(const std::string& time_title)
      {
	t_global_.stop();
	if (verbose_mode_ >= Time)
	  std::cerr << time_title << " "  << t_global_ << "s" << std::endl;
      }


      // Private dispatch

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


      template <typename V>
      logger_&
      logger_::operator<<(const V& v)
      {
	if (verbose_mode_ >= default_verbose_mode_)
	  stream_ << v;
	return *this;
      }


      inline
      logger_&
      logger_::operator<<(std::ostream& (*f)(std::ostream&))
      {
	if (verbose_mode_ >= default_verbose_mode_)
	  f(stream_);
	return *this;
      }


    } // end of namespace scribo::debug::internal


    inline
    scribo::debug::internal::logger_&
    logger()
    {
      return scribo::debug::internal::logger_::instance();
    }


    inline
    VerboseMode txt_to_verbose_mode(const std::string& name)
    {
      struct mode_name
      {
	const char *name;
	VerboseMode mode;
      };
      static const mode_name mode[] = {
	{ "MUTE", Mute },
	{ "USERDEBUG", UserDebug },
	{ "TIME", Time },
	{ "LOW", Low },
	{ "MEDIUM", Medium },
	{ "FULL", Full },
	{ "INVALID", Invalid }
      };

      unsigned i;
      std::string name_ = internal::to_upper(name);
      for (i = 0; mode[i].mode != Invalid; ++i)
	if (mode[i].name == name_)
	  break;

      return mode[i].mode;
    };

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::debug

} // end of namespace scribo

#endif // ! SCRIBO_CORE_LOGGER_HH
