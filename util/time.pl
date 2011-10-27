use strict;
use warnings;

use List::Util 'sum';

my @time = map {/\w+\s+(.*?)m(.*?)s/; $1 + $2 / 60} <STDIN>;

open T, $ARGV[0] or die $!;
<T> =~ /(\d+)/ or die 'invalid test format';
my $n = $1;
my $s = 0;

while (<T>) {
	/(\d+)\s+(\d+)/ or die 'invalid test format';
	$s += $1 * $2;
}

close T or die $!;

print "$s @time\n";
