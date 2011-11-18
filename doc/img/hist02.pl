use strict;
use warnings;

use File::Temp qw/tempfile/;
use List::Util qw/sum/;

my $term = "pngcairo size 800,300 font 'terminus,9' rounded";
#my $term = "png size 800,300";
my $line_style = "pt 1 ps 1 lt 1 lw 1";
my @line_colors = qw(0099ff ff0033 66cc66 9966ff 666600 ff66ff);

my @src;

for (<data4/*.out.*>) {
	open FH, $_ or die $!;
	my @time = map { (split)[1] } <FH>;
	close FH;
	/(\d+)$/;
	push @src, [int $1, sum(@time)] if @time;
}
die "no data" unless @src;

my ($fh, $fname) = tempfile;
my $scale = (shift @src)->[1];

for (map { [$_->[0], $scale / $_->[1]] } @src) {
	print $fh "@{$_}\n";
}

close $fh or die $!;

print "set grid y\n";
#print "set log xy\n";
#print "set style data hist\n";
print "set boxwidth 0.25\n";
print "set style fill solid border -1\n";
print "set term $term\n";
print "set key l under\n";
print "set tics scale 0\n";
print "set style increment user\n";
print "set yr [0.5:]\n";
print "set ytics format '%gx'\n";
print "set style line $_ lc rgb '#$line_colors[$_-1]' $line_style\n" for 1..6;
print "plot '$fname' u 2:xtic(1) w boxes notitle\n";
