#include <mln/essential/2d.hh>
#include <mln/essential/3d.hh>
#include <mln/registration/registration.hh>
#include <mln/io/cloud/all.hh>
#include <mln/debug/slices_2d.hh>

int main(int, char *argv[])
{
  using namespace mln;

  typedef p_array<point3d> arr_t;

  arr_t in;
  io::cloud::load(in, argv[1]);

  arr_t ref;
  io::cloud::load(ref, argv[2]);

  box3d bbox = geom::bbox(ref);
  bbox.enlarge(1, bbox.nrows() / 2);
  bbox.enlarge(2, bbox.ncols() / 2);
  bbox.enlarge(0, bbox.nslis() / 2);


  // Starting registration.
  util::timer t;
  t.start();
  typedef fun::x2x::rotation<3u,float> rot_t;
  typedef fun::x2x::translation<3u,float> trans_t;
  fun::x2x::composed<trans_t,rot_t> qk = registration::registration2(bbox, in, ref);

  std::cout << "igr.cc - Registration - " << t << "s" << std::endl;

  image3d<value::rgb8> ima_in(bbox);
  data::fill(ima_in, literal::black);
  data::fill((ima_in | in).rw(), literal::green);

  mln_VAR(ext_ima, extend(ima_in, pw::cst(value::rgb8(literal::black))));
  mln_VAR(trima, transposed_image(bbox, ext_ima, qk));

  p_array<point3d> arr;
  mln_VAR(tmp, (ima_in | in));
  mln_piter_(tmp_t) p(tmp.domain());
  for_all(p)
    arr.insert(qk(p));
  std::cout << arr.nsites() << std::endl;
  io::cloud::save(arr, "registered.txt");

  image3d<value::rgb8> ima_ref(geom::bbox(ref));
  mln_VAR(ext_ima_ref, extend(ima_ref, pw::cst(value::rgb8(literal::black))));
  data::fill(ext_ima_ref, literal::black);
  data::fill((ext_ima_ref | ref).rw(), literal::white);
  data::fill((ext_ima_ref | pw::value(trima) == pw::cst(literal::green)).rw(), literal::green);

//  io::cloud::save(trima, "registered.txt");

//  io::ppm::save(debug::slices_2d(ima_ref, 5, 3, literal::black), "registration_3d.ppm");
}
