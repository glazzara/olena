/*!
** \file	main.cc
** \author	Vivien Delmon
**
** \brief	Implementation of Globally Minimal Surfaces
**		by Continuous Maximal Flows
*/

#include <mln/core/var.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/routine/extend.hh>
#include <mln/io/pgm/all.hh>
#include <mln/io/pbm/all.hh>
#include <mln/value/all.hh>
#include <mln/morpho/gradient.hh>
#include <algorithm>
#define DELTA 0.7f // DELTA must be inferior to 1/sqrt(dimension)
#define GAMMA 0.03f
#define MU 0.96f

/*!
** \author	Vivien Delmon
** \brief	Return true if p represent a vertical edge and false otherwise
**
** \param	p [mln::point2d]
**
** \retval	bool
*/
inline
bool
is_vedge(const mln::point2d& p)
{
  return (!(p.row() % 2) && p.col() % 2);
}

/*!
** \author	Vivien Delmon
** \brief	Return true if p represent an horizontal edge and false otherwise
**
** \param	p [mln::point2d]
**
** \retval	bool
*/
inline
bool
is_hedge(const mln::point2d& p)
{
  return (p.row() % 2 && !(p.col() % 2));
}

/*!
** \author	Vivien Delmon
** \brief	Return true if p represent a potential
**
** \param	p [mln::point2d]
**
** \retval	bool
*/
inline
bool
is_potential(const mln::point2d& p)
{
  return (p.row() % 2 && p.col() % 2);
}


/*!
** \author	Vivien Delmon
** \brief	Return true if f is inf GAMMA or sup 1 - GAMMA
**
** \param	f
**
** \retval	bool
*/
inline
bool
is_stable(float f)
{
  return (f <= GAMMA || f >= 1.f - GAMMA);
}

/*!
** \author	Vivien Delmon
** \brief	Return the euclidean distance between two point2d
**
** \param	a
** \param	b
**
** \retval	float
*/
float distance(const mln::point2d& a,
	       const mln::point2d& b)
{
  int row = a.row() - b.row();
  int col = a.col() - b.col();
  return (std::sqrt(row * row - col * col));
}

/*!
** \author	Vivien Delmon
** \struct	g_functor : mln::Function_v2v< g_functor >
** \brief	Functor that inverts an int_u8 and return a float
*/
struct g_functor : mln::Function_v2v< g_functor >
{
  typedef float result;

  g_functor(result epsilon)
    : epsilon_(epsilon)
  {
  }

  result operator()(mln::value::int_u8 i) const
  {
    return 1.0f / (1 + i) + epsilon_;
  }

  private:
  result epsilon_;
};

/*!
** \author	Vivien Delmon
** \struct	seeds_functor : mln::Function_vv2v< seeds_functor >
** \brief	Functor that returns true the 2 values are not equal
*/
struct seeds_functor : mln::Function_vv2v< seeds_functor >
{
  typedef bool result;

  result operator()(mln::value::int_u8 a, mln::value::int_u8 b) const
  {
    return a != b;
  }
};

int
main (int argc, char** argv)
{
  using namespace mln;

  // Useful dpoint2d
  dpoint2d up(-1, 0);
  dpoint2d right(0, 1);
  dpoint2d down(1, 0);
  dpoint2d left(0, -1);

  if (argc != 4)
  {
    std::cerr << "Usage: " << argv[0] << "image.pgm image_seed.pgm binarised.pbm" << std::endl;
    return 1;
  }
  std::string file_in   = argv[1];
  std::string file_seeds  = argv[2];
  std::string file_out  = argv[3];

  image2d<value::int_u8> in;
  io::pgm::load (in, file_in);

  image2d<float> g;
  g = data::transform(morpho::gradient(in, win_c4p()), g_functor(0));

  image2d<value::int_u8> fseeds;
  io::pgm::load (fseeds, file_seeds);

  image2d<bool> seeds = data::transform(in, fseeds, seeds_functor());
  //image2d<point2d> closest = transform::closest_point_geodesic(seeds, c4(),
  //							       value:: int_u8(std::max(in.nrows(),
  //										       in.ncols())));

  /*
  **  Image that contains:
  **  Potential P in odd and odd coordinates
  **  Flows F in even xor odd coordinates
  **  Nothing even and even coordinates
  */
  image2d<float> flows_(in.nrows() * 2 + 1, in.ncols() * 2 + 1);
  mln_VAR(flows, extend(flows_, 0.0f));
  mln_VAR(potential, flows | is_potential);
  mln_piter_(potential_t) po(potential.domain());
  mln_VAR(vedge, flows | is_vedge);
  mln_piter_(vedge_t) pv(vedge.domain());
  mln_VAR(hedge, flows | is_hedge);
  mln_piter_(hedge_t) ph(hedge.domain());
  data::fill(potential, GAMMA + 0.01f);

  // Initialize seed potentials
  int nb_po_stable = 0;
  mln_piter_(image2d<bool>) p(seeds.domain());
  for_all (p)
    if (seeds(p))
    {
      flows(point2d(2 * p.row() + 1, 2 * p.col() + 1)) = 1;
      nb_po_stable++;
    }

  int nb_po_need = in.nrows() * in.ncols() * MU;
  for (int i = 0; nb_po_stable < nb_po_need; i++)
  {
    // Update potential
    for_all (po)
    {
      if (!seeds(point2d(po.row() / 2, po.col() / 2)))
      {
	float before = flows(po);
	float after = flows(po) -= DELTA * (flows(po + right) - flows(po + left) +
					    flows(po + down) - flows(po + up));
	if (after < 0)
	  after = flows(po) = 0;
	if (after > 1)
	  after = flows(po) = 1;
	if (is_stable(before))
	{
	  if (!is_stable(after))
	    nb_po_stable--;
	}
	else
	  if (is_stable(after))
	    nb_po_stable++;
      }
    }

    // Update vedges
    for_all (pv)
      flows(pv) -= DELTA * (flows(pv + down) - flows(pv + up));

    // Update hedges
    for_all (ph)
      flows(ph) -= DELTA * (flows(ph + right) - flows(ph + left));

    // Apply constraints on flows around potentials
    for_all (po)
    {
      float vflow = std::max(-flows(po + up), flows(po + down));
      if (vflow < 0)
	vflow = 0;
      float hflow = std::max(-flows(po + left), flows(po + right));
      if (hflow < 0)
	hflow = 0;
      float velocity = std::sqrt(vflow * vflow + hflow * hflow);
      float g_po = g(point2d(po.row() / 2, po.col() / 2));
      if (velocity > g_po)
      {
	vflow *= g_po / velocity;
	hflow *= g_po / velocity;
	flows(po + left) = std::max(flows(po + left), -hflow);
	flows(po + right) = std::min(flows(po + right), hflow);
	flows(po + up) = std::max(flows(po + up), -vflow);
	flows(po + down) = std::min(flows(po + down), vflow);
      }
    }
    std::cout << i << " : " << nb_po_stable << "/" << nb_po_need << std::endl;
  }
  // Output
#if 1
  image2d<bool> out;
  initialize(out, in);

  for_all (po)
    out(point2d(po.row() / 2, po.col() / 2)) = flows(po) > GAMMA;

  mln::io::pbm::save (out, file_out);
#else
  image2d<value::int_u8> out;
  initialize(out, in);

  for_all (po)
    out(point2d(po.row() / 2, po.col() / 2)) = flows(po) * 255;

  mln::io::pgm::save (out, file_out);
#endif
}
