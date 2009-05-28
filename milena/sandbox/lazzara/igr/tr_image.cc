#include <mln/essential/2d.hh>
#include <mln/essential/3d.hh>
#include <mln/fun/x2x/composed.hh>
#include <mln/fun/x2x/rotation.hh>
#include <mln/fun/x2x/translation.hh>
#include <mln/core/image/imorph/interpolated.hh>
#include <mln/fun/x2v/trilinear.hh>
#include <mln/fun/x2v/bilinear.hh>
#include <mln/core/image/imorph/tr_image.hh>

#include <mln/io/cloud/save.hh>
#include <mln/debug/slices_2d.hh>

int main(int, char *argv[])
{
  using namespace mln;
  using namespace fun::x2x;
  using value::rgb8;
  using value::int_u8;
  using value::label_16;

  //Load image
  typedef image2d<bool> J;
  typedef image3d<rgb8> K;
  typedef image2d<rgb8> I;

  I P;
  io::ppm::load(P, argv[1]);
  K P_3d = make::image3d(P);


  {
    typedef rotation<3u,float> rot_t;
    rot_t rot(1.57f, make::vec(0,0, 1));
    typedef translation<3u,float> trans_t;
    trans_t trans(make::vec(150, 50, 0));
    typedef composed<trans_t, rot_t> transf_t;
    transf_t qk(trans, rot);

//    std::cout << rot.mat() << std::endl;
//
    point3d p10_3d(0,1,0);
    std::cout << "point " << p10_3d << " == vec " << p10_3d.to_vec() << std::endl;
//    point3d p11_3d(0,1,1);
//    algebra::vec<4u,float> p10 = make::vec(0,1,0,1);
//    algebra::vec<4u,float> p11 = make::vec(0.f,1.f,1.f,1.f);
//    algebra::vec<4u,float> p01 = make::vec(0.f,0.f,1.f,1.f);
//    algebra::vec<4u,float> p_01 = make::vec(0,0,-1,1);
//
//    std::cout << "qk(p10_3d.to_vec) = " << qk(p10_3d.to_vec()) << std::endl;
//    std::cout << "rot(p10) = " << (rot.mat() * p10) << std::endl;
//    std::cout << "rot(p10) + trans = "  << (qk.mat() * p10) << std::endl << std::endl << std::endl;
//
//
//    std::cout << "rot.inv()(p10) = " << (rot.inv().mat() * p10) << std::endl;
//    std::cout << "rot.inv()(p_01) = " << (rot.inv().mat() * p_01) << std::endl;
//    std::cout << "rot.inv()(p01) = " << (rot.inv().mat() * p01) << std::endl;
//    std::cout << "qk.inv()(p11_3d) = "  << qk.inv()(p11_3d.to_vec()) << std::endl;
//    std::cout << "qk.inv().mat() * p11 = "  << (qk.inv().mat() * p11) << std::endl;
//    std::cout << "rot(p).inv() (p11 + trans.inv) = " << rot.inv().mat() * (trans.inv().mat() * p11) << std::endl;
//    std::cout << "trans.inv()(p01) = " << trans.inv().mat() * p01 << std::endl;
//
//    std::cout << std::endl << "trans.mat" << std::endl;
//    std::cout << trans.mat() << std::endl;
//    std::cout << "trans.mat.inv" << std::endl;
//    std::cout << trans.inv().mat() << std::endl;
//
//
//    std::cout << std::endl << "rot.mat" << std::endl;
//    std::cout << rot.mat() << std::endl;
//    std::cout << "rot.mat.inv" << std::endl;
//    std::cout << rot.inv().mat() << std::endl;
//
//    std::cout << "qk.mat" << std::endl;
//    std::cout  << qk.mat() << std::endl;
//    std::cout << "qk.inv().mat()" << std::endl;
//    std::cout << qk.inv().mat() << std::endl;

    typedef extension_fun<K,pw::cst_<mln_value_(I)> > ext_P_t;
    ext_P_t ext_P(P_3d, pw::cst(value::rgb8(literal::black)));

    typedef interpolated<ext_P_t, fun::x2v::trilinear> inter_t;
    inter_t inter(ext_P);

    std::cout << std::endl << "DEBUG TR_IMAGE" << std::endl;
    tr_image<box3d, inter_t, transf_t> trima(P_3d.domain(), inter, qk);

    io::ppm::save(slice(trima, 0), "trima_3d2.ppm");

  }

//  {
//    typedef rotation<3u,float> rot_t;
//
//    algebra::quat q;
//    q[0] = 0
//    q[3] = cos(1.57f / 2);
//    rot_t rot(1.57f, make::vec(0, 0, 1));
//    typedef translation<3u,float> trans_t;
//    trans_t trans(make::vec(150, 50, 0));
//    typedef composed<trans_t, rot_t> transf_t;
//    transf_t qk(trans, rot);
//
//    typedef extension_fun<K,pw::cst_<mln_value_(I)> > ext_P_t;
//    ext_P_t ext_P(P_3d, pw::cst(value::rgb8(literal::black)));
//
//    typedef interpolated<ext_P_t, fun::x2v::trilinear> inter_t;
//    inter_t inter(ext_P);
//
//    std::cout << std::endl << "DEBUG TR_IMAGE" << std::endl;
//    tr_image<box3d, inter_t, transf_t> trima(P_3d.domain(), inter, qk);
//
//    io::ppm::save(slice(trima, 0), "trima_3d3.ppm");
//
//  }


  {

    typedef rotation<2u,float> rot_t;
    rot_t rot(1.57f, make::vec(1, 0));

    typedef translation<2u,float> trans_t;
    trans_t trans(make::vec(150, 50));
    typedef composed<trans_t, rot_t> transf_t;
    transf_t qk(trans, rot);

    std::cout << rot.mat() << std::endl;

    typedef extension_fun<I,pw::cst_<mln_value_(I)> > ext_P_t;
    ext_P_t ext_P(P, pw::cst(value::rgb8(literal::black)));
    typedef interpolated<ext_P_t, fun::x2v::bilinear> inter_t;
    inter_t inter(ext_P);
    tr_image<box2d, inter_t, transf_t> trima(P.domain(), inter, qk);
    io::ppm::save(trima, "trima_2d.ppm");

  }
/*
  {
    typedef rotation<3u,float> rot_t;
    rot_t rot(1.57f, make::vec(0, 0, 1));
    typedef translation<3u,float> trans_t;
    trans_t trans(make::vec(160, 60, 0));
    typedef composed<trans_t, rot_t> transf_t;
    transf_t qk(trans, rot);

    std::cout << rot.mat() << std::endl;

    typedef extension_fun<K,pw::cst_<mln_value_(I)> > ext_P_t;
    ext_P_t ext_P(P_3d, pw::cst(value::rgb8(literal::black)));
    typedef interpolated<ext_P_t, fun::x2v::bilinear> inter_t;
    inter_t inter(ext_P);
    tr_image<box3d, inter_t, transf_t> trima(P_3d.domain(), inter, qk);
    io::ppm::save(slice(trima, 0), "trima_3d.ppm");

    std::cout << std::endl << " points: " << std::endl;
    unsigned i = 0;
    mln_piter_(box3d) p (P_3d.domain());
    for_all(p)
    {
      algebra::vec<4,float> tmp;
      for (unsigned j = 0; j < 3; ++j)
	tmp[j] = p.to_vec()[j];
      tmp[3] = 1;
      tmp = (qk.mat() * tmp);
      algebra::vec<3,float> res;
      for (unsigned j = 0; j < 3; ++j)
	res[j] = tmp[j];
      point3d ptmp = res;
      std::cout << p << " - " << res << " - " << point3d(res) << " - " << ptmp << std::endl;
      if (i >10)
	break;
      ++i;
    }
  }
*/
//  K reg(b);
//  data::fill(reg, literal::black);
//  mln_piter_(K) p(P_3d.domain());
//  for_all(p)
//    if (reg.domain().has(qk(p.to_site().to_vec())))
//      reg(qk(p.to_site().to_vec())) = P_3d(p);
//  io::ppm::save(slice(reg,0), "trima_mine.ppm");

}
