use strict;
use warnings;

use List::Util qw/sum/;

my $t;

sub print_m
{
	my ($r, $c, $s, $a, $b, $m) = @_;
	my @s = map { $s = ($s * $a + $b) % $m } (1..$r * $c);
	my $mean = int sum(@s) / ($r * $c) + 0.5;

	printf "Case #%d:\n", ++$t;

	while (@s) {
		printf "%3d ", shift(@s) - $mean for 1..$c;
		print "\n";
	}
}

sub gen
{
	my ($c) = split '\s+', <>;
	print_m split('\s+', <>) for 1..$c;
}

eval { gen };
die 'bad format' if $@;
