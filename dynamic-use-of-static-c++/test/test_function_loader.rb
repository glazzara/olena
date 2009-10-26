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

Pathname.glob(fixtures + '*.yml.cc').each do |file|
  YAML.load(file.read).each do |identifier, ref|
    fun = FunctionLoader.new identifier
    puts fun
    puts ref
    puts fun.to_cxx
    fun_ptr = fun.get_function
    puts fun_ptr
    puts '-' * 79
  end
end
