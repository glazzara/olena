# Copyright (C) 2005 EPITA Research and Development Laboratory (LRDE).
#
# This file is part of Olena.
#
# Olena is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free
# Software Foundation, version 2 of the License.
#
# Olena is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Olena.  If not, see <http://www.gnu.org/licenses/>.

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
