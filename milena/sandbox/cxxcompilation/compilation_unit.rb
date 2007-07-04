#! /usr/bin/ruby

#Create a compilation unit file from a .cc file

#USAGE: ./compilation_unit.rb file.cc


class CompilationUnit
  attr_reader :sourceFileName, :name, :namespaceName

  public
  def initialize(sourceFileName)


    @sourceFileName = sourceFileName
    @name = sourceFileName.gsub(/.cc\z/, ".unit.cc")
    @namespaceName = "__instanciator_code"

    if not File.exist?(@name) then
        self.createCompilationUnitFile
    end
  end

  def update
    createCompilationUnitFile
  end

  #Create UnitCompilationfile from source
  def createCompilationUnitFile
    sourceFile = File.new("#{@sourceFileName}", "r")
    destinationFile = File.new("#{@name}", "w")

   # Copy usefull include files into cache file.
    sourceFile.each_line do |line|
      destinationFile.puts(line) if line =~ /#include/
    end

    # trash namespace
    destinationFile.puts("namespace #{@namespaceName} {")

    # puts code line into trash namespace
    sourceFile.close
    sourceFile = File.new("#{@sourceFileName}", "r")
    sourceFile.each_line do |line|
      destinationFile.puts(line) if not (line =~ /#include/)
    end

    # end of namespace
    destinationFile.puts("}")

    sourceFile.close
    destinationFile.close
  end
end

unit = CompilationUnit.new(ARGV[0])
