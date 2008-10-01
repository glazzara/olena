#!/usr/bin/env perl

# while (<>) {
#     print if /\# ifndef MLN_INCLUDE_ONLY/ .. /\# endif \/\/ ! MLN_INCLUDE_ONLY/
# }
# perl -0777 -ne 'print "$1\n" while /ifndef MLN_INCLUDE_ONLY(.*?)# endif/gs'

sub process($)
{
    $file = shift;

    open(IN, '<', $file) or die "Failed to open `$file' for reading: $!";
    $_ = join('', <IN>);
    close(IN) or die "Failed to close `$file': $!";

    print  if /\# ifndef MLN_INCLUDE_ONLY/ .. /\# endif \/\/ ! MLN_INCLUDE_ONLY/;

    #open(OUT, '>', $file) or die "Failed to open `$file' for writing: $!";
    #print OUT;
    #close(OUT) or die "Failed to close `$file': $!";
}

my @input_files = @ARGV; #parse_options();
foreach my $file (@input_files)
{
    process $file;
}
