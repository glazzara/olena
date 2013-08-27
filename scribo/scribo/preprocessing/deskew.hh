// Copyright (C) 2010, 2013 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_PREPROCESSING_DESKEW_HH
# define SCRIBO_PREPROCESSING_DESKEW_HH

/// \file
///
/// Deskew an image.

/// FIXME: provide a version for binary images.


# include <queue>

# include <mln/core/image/image2d.hh>
# include <mln/math/pi.hh>
# include <mln/geom/rotate.hh>
# include <mln/value/int_u8.hh>

# define PI 3.1415926535897932384

namespace scribo
{

  namespace preprocessing
  {

    using namespace mln;

    namespace internal
    {

      class Hough
      {
      public:
	Hough(int width, int height);

	~Hough();

	void look_up_table();

	int width() const;

	int height() const;

	double mtheta() const;
	double mrho() const;

	int mrhoi() const;
	int mthetai() const;

	double get_cos(int index) const;
	double get_sin(int index) const;

	image2d<unsigned>& acc();

      private:
	int width_;
	int height_;

	double max_rho_;
	double max_theta_;

	int max_rho_index_;
	int max_theta_index_;

	double* cos_;
	double* sin_;

	image2d<unsigned> acc_;
      };


      struct s_angle
      {
	int pos;
	unsigned max;
      };


      struct QCompare
      {
	bool operator()(const s_angle&	s1, const s_angle& s2);
      };

    } // end of namespace scribo::preprocessing::internal



# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {

      inline
      bool
      QCompare::operator()(const s_angle& s1, const s_angle& s2)
      {
	return (s1.max > s2.max);
      }


      inline
      Hough::Hough(int	width, int height)
	: width_(width / 2),
	  height_(height / 2),
	  max_rho_(sqrt((width * width) + (height * height))),
	  max_theta_(math::pi),
	  max_rho_index_(int(this->max_rho_) + 1),
	  max_theta_index_(500),
	  acc_(this->max_rho_index_, this->max_theta_index_)
      {
	look_up_table();
      }


      inline
      Hough::~Hough()
      {
	delete[] this->cos_;
	delete[] this->sin_;
      }


      inline
      void	Hough::look_up_table()
      {
	this->cos_ = new double[this->max_theta_index_];
	this->sin_ = new double[this->max_theta_index_];

	for (int i = 0; i < this->max_theta_index_; ++i)
	{
	  double	i_val = (i + 650) * this->max_theta_ / 1800.0f;

	  this->cos_[i] = cos(i_val);
	  this->sin_[i] = sin(i_val);
	}
      }

      inline
      int	Hough::width() const
      {
	return this->width_;
      }

      inline
      int	Hough::height() const
      {
	return this->height_;
      }

      inline
      double	Hough::mtheta() const
      {
	return this->max_theta_;
      }

      inline
      double	Hough::mrho() const
      {
	return this->max_rho_;
      }

      inline
      int	Hough::mrhoi() const
      {
	return this->max_rho_index_;
      }

      inline
      int	Hough::mthetai() const
      {
	return this->max_theta_index_;
      }

      inline
      double	Hough::get_cos(int	index) const
      {
	return this->cos_[index];
      }

      inline
      double	Hough::get_sin(int	index) const
      {
	return this->sin_[index];
      }

      inline
      image2d<unsigned>&	Hough::acc()
      {
	return this->acc_;
      }

      inline
      static
      void
      vote(int x, int y, Hough& hough, int theta)
      {
	int	theta_min = std::max(theta - 25, 0);
	int	theta_max = std::min(theta + 25, hough.mthetai());

	x -= hough.width();
	y -= hough.height();

	for (int i = theta_min; i < theta_max; ++i)
	{
	  double rho = x * hough.get_cos(i) + y * hough.get_sin(i);
	  double rho_index = (0.5 + (rho / hough.mrho() + 0.5)
			      * hough.mrhoi());

	  ++(opt::at(hough.acc(),
		     static_cast<mln::def::coord>(rho_index), i));
	}
      }


      inline
      static
      void
      init_hist(Hough&	hough, int hist[500],
		std::priority_queue<s_angle, std::vector<s_angle>, QCompare>& q,
		int nb_elm)
      {
	int max_rho = hough.mrhoi();
	int max_theta = hough.mthetai();
	unsigned max_elm = (nb_elm > max_rho) ? (nb_elm / max_rho) << 5 : 1;

	for (int j = 0; j < max_theta; ++j)
	{
	  hist[j] = 0;

	  if (q.size() < max_elm)
	  {
	    if (opt::at(hough.acc(), 0, j) > 0)
	    {
	      s_angle s;

	      s.max = opt::at(hough.acc(), 0, j);
	      s.pos = j;

	      q.push(s);
	    }
	  }
	  else if (opt::at(hough.acc(), 0, j) > q.top().max)
	  {
	    s_angle s;

	    s.max = opt::at(hough.acc(), 0, j);
	    s.pos = j;

	    q.pop();
	    q.push(s);
	  }
	}
      }


      inline
      static
      double
      get_max(Hough& hough, int hist[500],
	      std::priority_queue<s_angle, std::vector<s_angle>, QCompare>& q,
	      int nb_elm)
      {
	int max = 0;
	int h_value = 0;
	int max_rho = hough.mrhoi();
	int max_theta = hough.mthetai();
	double pos = 0.f;
	unsigned max_elm = (nb_elm > max_rho) ? (nb_elm / max_rho) << 5 : 1;


	for (int i = 1; i < max_rho; ++i)
	{
	  for (int j = 0; j < max_theta; ++j)
	  {
	    if (q.size() < max_elm)
	    {
	      if (opt::at(hough.acc(), i, j) > 0)
	      {
		s_angle s;

		s.max = opt::at(hough.acc(), i, j);
		s.pos = j;

		q.push(s);
	      }
	    }
	    else if (opt::at(hough.acc(), i, j) > q.top().max)
	    {
	      s_angle s;

	      s.max = opt::at(hough.acc(), i, j);
	      s.pos = j;

	      q.pop();
	      q.push(s);
	    }
	  }
	}

	while (!q.empty())
	{
	  hist[q.top().pos] += q.top().max;
	  h_value = hist[q.top().pos];

	  if (h_value > max)
	  {
	    max = h_value;
	    pos = q.top().pos;
	  }

	  q.pop();
	}

	return pos;
      }


      inline
      double
      perform_deskew(const image2d<value::int_u8>& gray)
      {
	Hough hough(gray.ncols(), gray.nrows());
	std::priority_queue<s_angle, std::vector<s_angle>, QCompare> q;
	int hist[500];
	int nb_elm = 0;

	for (unsigned i = 0; i < gray.nrows() - 1; ++i)
	{
	  for (unsigned j = 1; j < gray.ncols() - 1; ++j)
	  {
	    unsigned up = 1;
	    unsigned down = 1;
	    unsigned mean = ((opt::at(gray, i, j) * opt::at(gray, i + 1, j))) >> 8;

	    for (unsigned k = j - 1; k <= j + 1; ++k)
	    {
	      up *= opt::at(gray, i, k);
	      down *= opt::at(gray, i + 1, k);
	    }

	    up = 255 - (up >>  16);
	    down = down >> 16;

	    if (up > down && down > mean && down > 130)
	    {

	      ++nb_elm;
	      double gy = opt::at(gray, i - 1, j - 1) + 2 * opt::at(gray, i - 1, j) +
		opt::at(gray, i - 1, j + 1);
	      gy += -opt::at(gray, i + 1, j - 1) - 2 * opt::at(gray, i + 1, j) -
		opt::at(gray, i + 1, j + 1);

	      double gx = opt::at(gray, i - 1, j - 1) + 2 * opt::at(gray, i, j - 1) +
		opt::at(gray, i + 1, j - 1);
	      gx += -opt::at(gray, i - 1, j + 1) - 2 * opt::at(gray, i, j + 1) -
		opt::at(gray, i + 1, j + 1);

	      double tanv = (PI / 2.0 - atan(gy / gx)) * 180.0 / PI;

	      if (tanv <= 25.0 || tanv >= 155.0)
	      {
		++nb_elm;
		vote(j, i, hough, int((tanv <= 25.0 ? 250.0 - tanv * 10.0 :
				       (180.0 - tanv) * 10.0 + 250.0)));
	      }
	    }
	  }
	}

	init_hist(hough, hist, q, nb_elm);

	return 90 - (get_max(hough, hist, q, nb_elm) + 650) / 10;
      }



    } // end of namespace scribo::preprocessing::internal



    template <typename I>
    mln_concrete(I)
    deskew(const Image<I>& input_gl_)
    {
      const I& input_gl = exact(input_gl_);

      mln_trace("scribo::preprocessing::deskew");
      mln_assertion(input_gl.is_valid());
      mlc_is(mln_domain(I), box2d)::check();
      mlc_is_not(mln_value(I), bool)::check();
      mlc_is_not_a(mln_value(I), value::Vectorial)::check();

      double angle = internal::perform_deskew(input_gl);

      mln_concrete(I) output = input_gl;

      // FIXME: trick to make this routine faster for really small
      // angles (no impact on the results)
      if (angle > 0.5 || angle < -0.5)
	output = geom::rotate(input_gl, - angle,
			      //mln_max(mln_value(I)),
			      extend(input_gl, mln_min(mln_value(I))),
			      mln::make::box2d(input_gl.nrows(),
					       input_gl.ncols()));

      return output;
    }


# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace scribo::preprocessing

} // end of namespace scribo


# endif // SCRIBO_PREPROCESSING_DESKEW_HH
