use strict;
use warnings;

my $term = "pngcairo size 800,300 font 'terminus,9'";
my $line_style = "pt 5 ps 0.5 lt 1 lw 1.5";
my @line_colors = qw(ff0033 0099ff 66cc66 9966ff 666600 ff66ff);

my $dir = $ARGV[0];
-d $dir || die "source must be a directory";

my @src;
push @src, $_ for <$dir/*.out.*>;
die "no data" unless @src;

print "set grid y\n";
print "set log xy\n";
print "set term $term\n";
print "set key l under\n";
print "set style increment user\n";
print "set style line $_ lc rgb '#$line_colors[$_-1]' $line_style\n" for 1..6;
print "plot ", join(', ',
	map { /0?(\d+)$/; sprintf "'$_' u 1:2 w lp t 'Количество потоков: %2d'", $1 } @src), "\n";
