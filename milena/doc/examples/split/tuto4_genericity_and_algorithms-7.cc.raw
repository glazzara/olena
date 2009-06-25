  image2d<bool> mask;
  initialize(mask, lena);
  data::fill(mask, false);
  data::fill((mask | make::box2d(10, 10, 14, 14)).rw(), true);
  data::fill((mask | make::box2d(25, 15, 29, 18)).rw(), true);
  data::fill((mask | make::box2d(50, 50, 54, 54)).rw(), true);
