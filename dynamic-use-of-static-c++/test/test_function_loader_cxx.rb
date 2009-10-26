require 'pathname'
require 'yaml'
test = Pathname.new(__FILE__).dirname
root = test.parent
fixtures = test + 'fixtures'
src = root + 'src'
$: << src
require 'function_loader'

FunctionLoader.include_dir fixtures
FunctionLoader.include_dir src

funs = []

funs << FunctionLoader.call(:proc, 'foo1', [], 'my_lib/lib.hh')
funs << FunctionLoader.call(:fun, 'foo2', [], 'my_lib/lib.hh')
funs << FunctionLoader.call(:fun, 'foo3',
                            ['double', 'const double', 'const double&'],
                            'my_lib/lib.hh')
funs << FunctionLoader.call(:fun, 'my_lib::x::foo4',
                            ['u<float>', 't<t<char, char>, u<const int> >'],
                            'my_lib/lib.hh')
funs << FunctionLoader.call(:fun, 'foo3', ['t<int, char>',
                            'const t<int, char>', 'const t<int, char>&'],
                            'my_lib/lib.hh')

