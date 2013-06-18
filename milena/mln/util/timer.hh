// Copyright (C) 2008, 2009, 2011, 2013 EPITA Research and Development
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

#ifndef MLN_UTIL_TIMER_HH
# define MLN_UTIL_TIMER_HH

/// \file
///
/// Definition of a timer.

# include <mln/core/concept/proxy.hh>
# include <ctime>


namespace mln
{

  namespace util
  {

    /// Timer structure.
    class timer : public Proxy< timer >,
		  public mln::internal::proxy_impl<float, timer>
    {
    public:

      timer();

      ~timer();

      void start();

      float stop();

      void resume();

      void reset();

      // Restart is equivalent to reset then start.
      void restart();

      float read() const;

      // As a proxy:
      float subj_();

//       void print() const
//       {
// 	std::cout << running_ << ' '
// 		  << start_ << ' '
// 		  << std::clock() << ' '
// 		  << float(std::clock()) / CLOCKS_PER_SEC << ' '
// 		  << time_ << std::endl;
//       }

      double ms() const
      {
	return double(std::clock()) * 1000.f / CLOCKS_PER_SEC;
      }

    private:

      bool running_;
      float start_;
      float time_;
    };


# ifndef MLN_INCLUDE_ONLY

    inline
    timer::timer()
    {
      reset();
    }

    inline
    timer::~timer()
    {
      reset();
    }

    inline
    void
    timer::start()
    {
      mln_precondition(running_ == false);
      start_ = float(std::clock()) / CLOCKS_PER_SEC;
      time_ = 0;
      running_ = true;
    }

    inline
    float
    timer::stop()
    {
      mln_precondition(running_ == true);
      time_ += float(std::clock()) / CLOCKS_PER_SEC - start_;
      running_ = false;
      return time_;
    }

    inline
    void
    timer::resume()
    {
      mln_precondition(running_ == false);
      start_ = float(std::clock()) / CLOCKS_PER_SEC;
      running_ = true;
    }

    inline
    void
    timer::reset()
    {
      running_ = false;
      start_ = -1;
      time_ = 0;
    }

    inline
    void
    timer::restart()
    {
      reset();
      start();
    }


    inline
    float
    timer::read() const
    {
      mln_precondition(start_ != -1);
      return running_ ?
	time_ + float(std::clock()) / CLOCKS_PER_SEC - start_ :
	time_;
    }

    inline
    float
    timer::subj_()
    {
      mln_precondition(start_ != -1);
      return read();
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::util

} // end of namespace mln


#endif // ! MLN_UTIL_TIMER_HH
