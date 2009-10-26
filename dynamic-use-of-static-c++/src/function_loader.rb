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
