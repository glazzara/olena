  image2d<value::int_u8> img2a(2, 3);
  image2d<value::int_u8> img2b;

  initialize(img2b, img2a);
  data::fill(img2b, img2a);
