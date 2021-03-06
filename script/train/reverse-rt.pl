#!/usr/bin/perl

use strict;
use utf8;
use Getopt::Long;
use List::Util qw(sum min max shuffle);
binmode STDIN, ":utf8";
binmode STDOUT, ":utf8";
binmode STDERR, ":utf8";

if(@ARGV != 0) {
    print STDERR "Usage: $0 < INPUT > OUTPUT\n";
    exit 1;
}

while(<STDIN>) {
    chomp;
    my @arr = split(/ \|\|\| /);
    die "bad line $_" if(@arr != 3);
    print "$arr[1] ||| $arr[0] ||| $arr[2]\n";
}
