  image2d<rgb8> lena;
  io::ppm::load(lena, MLN_IMG_DIR "/small.ppm");
  box2d bbox_enlarged = lena.domain();
  bbox_enlarged.enlarge(border::thickness);
  mln_VAR(ima_roi, lena | fun::p2b::big_chess<box2d>(lena.domain(), 10));
