# Copyright (C) 2005, 2009 EPITA Research and Development Laboratory (LRDE).
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

DYN_DATADIR = Pathname.new(__FILE__).dirname.parent + '_build/data' # FIXME

def compile ( cxx, identifier, name, cflags, ldflags )
  repository = Pathname.new('repository')
  unless repository.exist?
    repository.mkpath
    (repository + 'Makefile').make_symlink(DYN_DATADIR + 'Makefile.repository')
  end
  dir = repository + identifier
  unless dir.exist?
    dir.mkpath
    makefile = (DYN_DATADIR + 'Makefile.template').read
    makefile.gsub!(/libdyn_function\.la/, "libdyn_#{identifier}.la")
    (dir + 'Makefile').open('w') { |f| f.puts makefile }
    file = dir + "function.cc"
    (dir + '.deps').mkpath
    (dir + '.deps' + 'libdyn_function_la-function.Plo').open('w')
    file.open('w') do |f|
      f.puts cxx
    end
    (dir + 'Makefile').open('a') do |f|
      f.puts "CXXFLAGS += #{cflags}"
      f.puts "LDFLAGS += #{ldflags}"
    end
  end
  out = dir + 'make.out'
  cmd = "cd #{dir} && make > make.out 2>&1"
  if system cmd
    out.unlink if out.exist?
  else
    STDERR.puts 'JIT: Error when compiling this code'
    STDERR.puts cxx
    STDERR.puts cmd
    STDERR.puts out.read
    exit! 1
  end
end
