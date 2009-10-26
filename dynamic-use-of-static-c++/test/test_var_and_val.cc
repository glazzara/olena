#include "my_lib/lib.hh"

#include "dyn-all.hh"
using namespace dyn::language;

int main()
{
  dyn::include_dir(DYN_FIXTURES);
  dyn::include("my_lib/lib.hh");

  echo("Test vars and vals");
  val i = 4;
  val j = 3 + i;
  var k1(i);
  var k2 = i;
  // var l1 = "foo";
  // var l2("foo");
  echo("i: " << i);
  echo("j: " << j);
  echo("k: " << k1 << ", " << k2);

  std::string str("  youhou");
  const std::string cst_str("  const youhou");

  echo("var_str1");
  var var_str1(str);
  assert(not var_str1.is_const());
  var_str1[0] = 'a';
  assert(var_str1 == "a youhou");
  assert(str == "a youhou");

  echo("var_str2");
  var var_str2(cst_str);
  assert(not var_str2.is_const());
  var_str2[0] = 'a';
  assert(var_str2 == "a const youhou");
  assert(cst_str == "  const youhou");

  echo("var_str3");
  var var_str3(var_str1);
  assert(not var_str3.is_const());
  var_str3[0] = 'b';
  assert(var_str3 == "b youhou");
  assert(var_str1 == "b youhou");
  assert(str == "b youhou");

  echo("var_str4");
  var var_str4(var_str2);
  assert(not var_str4.is_const());
  var_str4[0] = 'b';
  assert(var_str4 == "b const youhou");
  assert(var_str2 == "a const youhou"); // here we see that we have another copy
  assert(cst_str == "  const youhou");

  echo("val_str1");
  val val_str1(str);
  assert(val_str1.is_const());
  // val_str1[0] = 'a'; // Does not compile
  assert(val_str1 == "b youhou");

  echo("val_str2");
  val val_str2(cst_str);
  assert(val_str2.is_const());
  // val_str2[0] = 'a'; // Does not compile
  assert(val_str2 == "  const youhou");

  echo("val_str3");
  val val_str3(val_str1);
  assert(val_str3.is_const());
  // val_str3[0] = 'b'; // Does not compile
  assert(val_str3 == "b youhou");

  echo("val_str4");
  val val_str4(val_str2);
  assert(val_str4.is_const());
  // val_str4[0] = 'b'; // Does not compile
  assert(val_str4 == "  const youhou");

  echo("val_str5");
  val val_str5(var_str1);
  assert(val_str5.is_const());
  // val_str5[0] = 'b'; // Does not compile
  assert(val_str5 == "b youhou");

  echo("var_str6");
  var var_str6(val_str1);
  assert(var_str6.is_const()); // FIXME should be not const
  // var_str6[0] = 'c';        // FIXME should compile
  // assert(var_str6 == "c youhou");
  assert(var_str1 == "b youhou");
  assert(str == "b youhou");

  echo("exiting");
  return 0;
}
