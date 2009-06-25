  image2d<int> ima1(box2d(2, 3));
  image2d<int> ima2;
  point2d p(1,2);

  ima2 = ima1; // ima1.id() == ima2.id()
  // and both point to the same memory area.

  ima2(p) = 2; // ima1 is modified as well.

  // prints "2 - 2"
  std::cout << ima2(p) << " - " << ima1(p) << std::endl;
  // prints "true"
  std::cout << (ima2.id_() == ima1.id_()) << std::endl;
