#!/usr/bin/perl
use GD::Graph::lines;
#use strict;

#my @data = ( 
#    ["1st","2nd","3rd","4th","5th","6th","7th", "8th", "9th"],
#    [    5,   12,   24,   33,   19,undef,    6,    15,    21],
#    [    1,    2,    5,    6,    3,  1.5,    1,     3,     4]
#);
my $datafile = $ARGV[0]; 
my $safefile = $ARGV[1]; 
my $maxvalue = $ARGV[2]; 
my @data =  read_data_from_csv($datafile)
	or die "Cannot read data from datafile";

my @names = $safefile;

my $graph = GD::Graph::lines->new(650, 400);

{
    my $name = shift @names;
    print STDERR "Processing $name\n";

    $graph->set( 
	bgclr => 'white',
        'dclrs'            => [ qw(lbrown lgreen orange) ], 
	'y_label'          => "%",
	'x_ticks'          => 0,
	'x_label_position' => .5,
	'y_label_position' => .5,
	y_max_value => $maxvalue,
	y_tick_number => 5,
	y_label_skip => 1,
	line_types => [ 1, 1, 1 ],
	#line_type_scale => 8,
	#legend_marker_width => 12,
	line_width => 3,
	#accent_treshold => 41,
	transparent => 1,
	long_ticks      => 1,
        y_number_format  => '%u',
        'zero_axis'        => 1,
        bar_spacing     => 3,
        #shadow_depth    => 3,
        'x_labels_vertical'=> 1,
        accent_treshold => 100_000,
    );

    $graph->set_legend( 'rate', '','' );
    $graph->plot(\@data);
    save_chart($graph, $name);
}



sub save_chart
{
	my $chart = shift or die "Need a chart!";
	my $name = shift or die "Need a name!";
	local(*OUT);

  my $ext = $chart->export_format;

	open(OUT, ">$name") or 
		die "Cannot open $name.$ext for write: $!";
	binmode OUT;
	print OUT $chart->gd->$ext();
	close OUT;
}


sub read_data_from_csv
{
	my $fn = shift;
	my @d = ();

	open(ZZZ, $fn) || return ();

	while (<ZZZ>)
	{
		chomp;
		# you might want Text::CSV here
		my @row = split /,/;

		for (my $i = 0; $i <= $#row; $i++)
		{
			undef $row[$i] if ($row[$i] eq 'undef');
			push @{$d[$i]}, $row[$i];
		}
	}

	close (ZZZ);

	return @d;
}

