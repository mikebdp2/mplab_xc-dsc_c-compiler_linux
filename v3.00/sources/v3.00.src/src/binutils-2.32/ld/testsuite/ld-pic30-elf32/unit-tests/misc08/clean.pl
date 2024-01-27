#!/usr/bin/perl
use strict;
use warnings;

# strip file offset from objdump output
while (<>) {
  if (/(.*)\s+\w+\s+2\*\*1$/) {print "$1\n"}
  else {print $_}
}
