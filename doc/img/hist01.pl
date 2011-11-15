use strict;
use warnings;

use File::Temp qw/tempfile/;
use List::Util qw/sum/;

my $term = "pngcairo size 800,400 font 'terminus,9'";
my $line_style = "pt 1 ps 1 lt 1 lw 1";
my @line_colors = qw(6ff0033 0099ff 6cc66 9966ff 666600 ff66ff);

#my $dir = $ARGV[0];
#-d $dir || die "source must be a directory";

#my @files = <data?> or die "no data";

my @time;

for my $F (glob "data{6,4,7,8}/*.out.*") {
	open F, $F or die $!;

	push @time, sum map { (split)[1] } <F>;

	close F;
}

print "set style data hist\n";
print "set grid y\n";
#print "set log xy\n";
print "set term $term\n";
#print "set key l\n";
print "set style increment user\n";
print "set style line $_ lc rgb '#$line_colors[$_-1]' $line_style\n" for 1..6;
print "set style fill solid border -1\n";

my ($fh, $fn) = tempfile;

for (my $i = 0; $i < 6; ++$i) {
	for (my $j = 0; $j < @time / 6; ++$j) {
		print $fh $time[$j * 6 + $i], " ";
	}

	print $fh "\n";

}

close $fh or die $!;
print "set xtics ('1' 0, '4' 1, '8' 2, '16' 3, '35' 4, '40' 5)\n";
print "plot '$fn' u 1 t 'Размер порции: 10', '' u 2 t 'Размер порции: 20', '' u 3 t 'Размер порции: 30', '' u 4 t 'Размер порции: 50'\n";
