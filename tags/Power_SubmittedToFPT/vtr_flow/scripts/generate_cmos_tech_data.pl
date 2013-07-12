#!/usr/bin/perl

use strict;
use POSIX;
use File::Basename;

sub get_trans_properties;
sub get_optimal_sizing;
sub get_riset_fallt_diff;
sub get_Vth;

my $hspice = "hspice";


my $number_arguments = @ARGV;
if ($number_arguments < 4)
{
	print("usage: generate_cmos_tech_data.pl <tech_file> <tech_size> <Vdd> <temp>\n");
	exit(-1);
}

my $tech_file = shift(@ARGV);
my $tech_size = shift(@ARGV);
my $Vdd = shift(@ARGV);
my $temp = shift(@ARGV);
my $tech_file_name = basename($tech_file);

-r $tech_file or die "Cannot open tech file ($tech_file)";

my $spice_file = "vtr_auto_trans_info.sp";
my $spice_out = "vtr_auto_trans_info.out";

my $leakage;
my $C_gate_cmos;
my $C_source_cmos;
my $C_drain_cmos;
my $C_gate_pass;
my $C_source_pass;
my $C_drain_pass;

my $full = 1;

my @sizes;
my @long_size = (1, 2);
my @transistor_types = ("nmos", "pmos");

if (! $full) {
	my @transistor_types = ("pmos");
}




my $optimal_p_to_n;
if ($full) {
	$optimal_p_to_n =  get_optimal_sizing();
} else {
	$optimal_p_to_n = 1.0;
}

if ($full) {
	my $size = 1.0;
	while ($size < 1000) {
		my $size_rounded = (sprintf "%.2f", $size);
		push (@sizes, [$size_rounded, 1]);
		$size = $size * 1.02;
	}
}
else
{
	push(@sizes, [1, 1]);
}

print "<technology file=\"$tech_file_name\" size=\"$tech_size\">\n";

print "\t<operating_point temperature=\"$temp\" Vdd=\"$Vdd\"/>\n";
print "\t<p_to_n ratio=\"" . $optimal_p_to_n . "\"/>\n"; 

foreach my $type (@transistor_types) {
	my $Vth = get_Vth($type);
	print "\t<transistor type=\"$type\" Vth=\"$Vth\">\n";
		
	($leakage, $C_gate_cmos, $C_drain_cmos, $C_gate_pass, $C_source_pass, $C_drain_pass) = get_trans_properties($type, $long_size[0], $long_size[1]);
	print "\t\t<long_size W=\"". $long_size[0] . "\" L=\"" . $long_size[1] . "\">\n";
	print "\t\t\t<current leakage=\"$leakage\"/>\n";
	print "\t\t\t<capacitance gate_cmos=\"$C_gate_cmos\" drain_cmos=\"$C_drain_cmos\" gate_pass=\"$C_gate_pass\" source_pass=\"$C_source_pass\" drain_pass=\"$C_drain_pass\"/>\n";
	print "\t\t</long_size>\n";
	
	foreach my $size_ref (@sizes) {
		my @size = @$size_ref;
		($leakage, $C_gate_cmos, $C_drain_cmos, $C_gate_pass, $C_source_pass, $C_drain_pass) = get_trans_properties($type, $size[0], $size[1]);
		print "\t\t<size W=\"". $size[0] . "\" L=\"" . $size[1] . "\">\n";
		print "\t\t\t<current leakage=\"$leakage\"/>\n";
		print "\t\t\t<capacitance gate_cmos=\"$C_gate_cmos\" drain_cmos=\"$C_drain_cmos\" gate_pass=\"$C_gate_pass\" source_pass=\"$C_source_pass\" drain_pass=\"$C_drain_pass\"/>\n";
		print "\t\t</size>\n";
	}
	print "\t</transistor>\n";
}

print "</technology>\n";

sub get_optimal_sizing {
	my @sizes;
	my $opt_size;
	my $opt_percent;
	
	for (my $size = 0.5; $size < 4; $size += 0.05) {
		push (@sizes, $size);
	}
	
	$opt_percent = 10000; # large number
	foreach my $size (@sizes) {
		my $diff = get_riset_fallt_diff($size);
		if ($diff < $opt_percent) {
			$opt_size = $size;
			$opt_percent = $diff;
		}
	}
	
	return $opt_size;
}

sub get_Vth {
	my $type = shift(@_);
	
	my $spice_string = "";
	my $Vth;
	
	$spice_string = $spice_string . "HSpice Simulation of an $type transistor.\n";
	$spice_string = $spice_string . ".include \"$tech_file\"\n";
	$spice_string = $spice_string . ".param tech = $tech_size\n";
	$spice_string = $spice_string . ".param Vol = $Vdd\n";	
	$spice_string = $spice_string . ".param size = 1.0\n";	
	
	# Add voltage source
	$spice_string = $spice_string . "Vdd Vdd 0 Vol\n";
	
	# Transistors
	if ($type eq "nmos") {
	$spice_string = $spice_string . "M0 Vdd 0 0 0 nmos L='tech' W='size*tech' AS='size*tech*2.5*tech' PS='2*2.5*tech+size*tech' AD='size*tech*2.5*tech' PD='2*2.5*tech+size*tech'\n";
	} else {
	$spice_string = $spice_string . "M0 0 Vdd Vdd Vdd pmos L='tech' W='size*tech' AS='size*tech*2.5*tech' PS='2*2.5*tech+size*tech' AD='size*tech*2.5*tech' PD='2*2.5*tech+size*tech'\n";
	}
		
	$spice_string = $spice_string . ".TEMP $temp\n";
	$spice_string = $spice_string . ".OP\n";
	$spice_string = $spice_string . ".OPTIONS LIST NODE POST\n";
	
	$spice_string = $spice_string . ".tran 1p 50n\n";
	
	
	$spice_string = $spice_string . ".measure tran vth avg vth(M0)\n";
	$spice_string = $spice_string . ".end\n\n";
	
	open (SP_FILE, "> $spice_file");
	print SP_FILE $spice_string;
	close (SP_FILE);
	
	system ("$hspice $spice_file 1> $spice_out 2> /dev/null");
	
	my $spice_data;
	{
		local $/=undef;
		open (SP_FILE, "$spice_out");
		$spice_data = <SP_FILE>;
		close SP_FILE;
	}
	
	
	if ($spice_data =~ /transient analysis.*?vth\s*=\s*(\S+)/s) {
		$Vth = $1;	
	} else {
		die "Could not find threshold voltage in spice output.\n";
	}
	
	return $Vth;
}


sub get_riset_fallt_diff {
	my $size = shift(@_);
	
	my $spice_string = "";
	my $rise;
	my $fall;
	
	$spice_string = $spice_string . "HSpice Simulation of an inverter.\n";
	$spice_string = $spice_string . ".include \"$tech_file\"\n";
	$spice_string = $spice_string . ".param tech = $tech_size\n";
	$spice_string = $spice_string . ".param Vol = $Vdd\n";	
	$spice_string = $spice_string . ".param psize = '2*$size'\n";
	$spice_string = $spice_string . ".param nsize = '2.0'\n";	
	$spice_string = $spice_string . ".param Wn = 'nsize*tech'\n";
	$spice_string = $spice_string . ".param Wp = 'psize*tech'\n";
	$spice_string = $spice_string . ".param L = 'tech'\n";
	$spice_string = $spice_string . ".param D = '2.5*tech'\n";
	
	# Add voltage source
	$spice_string = $spice_string . "Vdd Vdd 0 Vol\n";
	
	# Input Voltage
	$spice_string = $spice_string . "Vin gate 0 PULSE(0 Vol 10n 0.01n 0.01n 10n 20n)\n";
	
	# Transistors
	$spice_string = $spice_string . "M1 drain gate nsource nbody nmos L='L' W='Wn' AS='Wn*D' PS='2*D+Wn' AD='Wn*D' PD='2*D+Wn'\n";
	$spice_string = $spice_string . "M2 drain gate psource pbody pmos L='L' W='Wp' AS='Wp*D' PS='2*D+Wp' AD='Wp*D' PD='2*D+Wp'\n";
	
	# Connect Sources
	$spice_string = $spice_string . "R1 nsource 0 0\n";
	$spice_string = $spice_string . "R2 psource Vdd 0 0\n";
	
	# Connect Body to source
	$spice_string = $spice_string . "R3 nbody nsource 0\n";
	$spice_string = $spice_string . "R4 pbody psource 0\n";
	
	# Drain to output
	$spice_string = $spice_string . "R5 drain out 0\n";
		
	$spice_string = $spice_string . ".TEMP $temp\n";
	$spice_string = $spice_string . ".OP\n";
	$spice_string = $spice_string . ".OPTIONS LIST NODE POST\n";
	
	$spice_string = $spice_string . ".tran 1f 30n\n";
	
	
	$spice_string = $spice_string . ".measure tran fallt TRIG V(gate) VAL = '0.5*Vol' TD = 9ns RISE = 1 TARG V(out) VAL = '0.5*Vol' FALL = 1\n";
	$spice_string = $spice_string . ".measure tran riset TRIG V(gate) VAL = '0.5*Vol' TD = 19ns FALL = 1 TARG V(out) VAL = '0.5*Vol' RISE = 1\n";
	$spice_string = $spice_string . ".end\n\n";
	
	open (SP_FILE, "> $spice_file");
	print SP_FILE $spice_string;
	close (SP_FILE);
	
	system ("$hspice $spice_file 1> $spice_out 2> /dev/null");
	
	my $spice_data;
	{
		local $/=undef;
		open (SP_FILE, "$spice_out");
		$spice_data = <SP_FILE>;
		close SP_FILE;
	}
	
	
	if ($spice_data =~ /transient analysis.*?riset\s*=\s*(\S+)/s) {
		$rise = $1;	
	} else {
		die "Could not find rise time in spice output.\n";
	}
	
	if ($spice_data =~ /transient analysis.*?fallt\s*=\s*(\S+)/s) {
		$fall = $1;	
	} else {
		die "Could not find fall time in spice output.\n";
	}
	
	return abs($rise - $fall) / $rise;
}

sub get_trans_properties
{
	my $type = shift(@_);
	my $width = shift(@_);
	my $length = shift(@_);
	
	my $spice_string = "";
	my $leakage;
	my $C_gate;
	my $C_source;
	my $C_drain;
	my $sim_time = "600u";
	
	$spice_string = $spice_string . "HSpice Simulation of a $type transistor.\n";
	$spice_string = $spice_string . ".include \"$tech_file\"\n";
	$spice_string = $spice_string . ".param tech = $tech_size\n";
	$spice_string = $spice_string . ".param Vol = $Vdd\n";
	$spice_string = $spice_string . ".param W = '" . $width . "*tech'\n";
	$spice_string = $spice_string . ".param L = '" . $length . "*tech'\n";
	$spice_string = $spice_string . ".param D = '2.5*tech'\n";
	$spice_string = $spice_string . ".param simt = $sim_time\n";
	$spice_string = $spice_string . ".param rise = 'simt/100'\n";
	
	
	
	# Gate, Drain, Source Inputs
	# Time	NMOS		PMOS	Valid For:
	# 		G S D		G S D
	# 0		1 0 0		0 1 1	CMOS, Pass-Logic
	# 1		0 0 1     	1 1 0	CMOS, Pass-Logic, Subthreshold Leakage
	# 2		0 0 0     	1 1 1	Pass-Logic
	# 3		1 1 1     	0 0 0	Pass-Logic
	# 4		0 1 1     	1 0 0	Pass-Logic
	# 5		0 1 0	    1 0 1	Pass-Logic
	
	
	$spice_string = $spice_string . ".param t0s = '0*simt/6+rise'\n";	
	$spice_string = $spice_string . ".param t0e = '1*simt/6'\n";	
	$spice_string = $spice_string . ".param t1s = '1*simt/6+rise'\n";	
	$spice_string = $spice_string . ".param t1e = '2*simt/6'\n";	
	$spice_string = $spice_string . ".param t2s = '2*simt/6+rise'\n";	
	$spice_string = $spice_string . ".param t2e = '3*simt/6'\n";	
	$spice_string = $spice_string . ".param t3s = '3*simt/6+rise'\n";	
	$spice_string = $spice_string . ".param t3e = '4*simt/6'\n";	
	$spice_string = $spice_string . ".param t4s = '4*simt/6+rise'\n";	
	$spice_string = $spice_string . ".param t4e = '5*simt/6'\n";	
	$spice_string = $spice_string . ".param t5s = '5*simt/6+rise'\n";	
	$spice_string = $spice_string . ".param t5e = '6*simt/6'\n";	
	
	
	$spice_string = $spice_string . ".param leakagestart = '1*simt/6+rise'\n";	
	$spice_string = $spice_string . ".param leakageend = '2*simt/6'\n";
	$spice_string = $spice_string . ".param cmosstart = '0*simt/6+rise'\n";	
	$spice_string = $spice_string . ".param cmosend = '1*simt/6'\n";
	$spice_string = $spice_string . ".param passstart = '0*simt/6+rise'\n";	
	$spice_string = $spice_string . ".param passend = '6*simt/6'\n";
	
	if ($type eq "nmos")
	{	
		$spice_string = $spice_string . "Vgate gate 0 PWL(0 Vol '1*simt/6' Vol '1*simt/6+rise' 0 '3*simt/6' 0 '3*simt/6+rise' Vol '4*simt/6' Vol '4*simt/6+rise' 0)\n";
		$spice_string = $spice_string . "Vsource source 0 PWL(0 0 '3*simt/6' 0 '3*simt/6+rise' Vol)\n";
		$spice_string = $spice_string . "Vdrain drain 0 PWL(0 0 '1*simt/6' 0 '1*simt/6+rise' Vol '2*simt/6' Vol '2*simt/6+rise' 0 '3*simt/6' 0 '3*simt/6+rise' Vol '5*simt/6' Vol '5*simt/6+rise' 0)\n";

	} else {		
		$spice_string = $spice_string . "Vgate gate 0 PWL(0 0 '1*simt/6' 0 '1*simt/6+rise' Vol '3*simt/6' Vol '3*simt/6+rise' 0 '4*simt/6' 0 '4*simt/6+rise' Vol)\n";
		$spice_string = $spice_string . "Vsource source 0 PWL(0 Vol '3*simt/6' Vol '3*simt/6+rise' 0)\n";
		$spice_string = $spice_string . "Vdrain drain 0 PWL(0 Vol '1*simt/6' Vol '1*simt/6+rise' 0 '2*simt/6' 0 '2*simt/6+rise' Vol '3*simt/6' Vol '3*simt/6+rise' 0 '5*simt/6' 0 '5*simt/6+rise' Vol)\n";
	}	
	
	# Transistor
	$spice_string = $spice_string . "M1 drain gate source source $type L='L' W='W' AS='W*D' PS='2*D+W' AD='W*D' PD='2*D+W'\n";

	
	
	$spice_string = $spice_string . ".TEMP $temp\n";
	$spice_string = $spice_string . ".OP\n";
	$spice_string = $spice_string . ".OPTIONS LIST NODE POST CAPTAB\n";
	
	$spice_string = $spice_string . ".tran 'simt/1000' simt\n";
	$spice_string = $spice_string . ".print tran cap(gate)\n";
	$spice_string = $spice_string . ".print tran cap(drain)\n";
	$spice_string = $spice_string . ".print tran cap(source)\n";
	
	$spice_string = $spice_string . ".measure tran leakage_current avg I(Vdrain) FROM = t1s TO = t1e\n";
	
	if ($type eq "nmos")
	{	
		$spice_string = $spice_string . ".measure tran c_gate_cmos avg cap(gate) FROM = t0s TO = t0e\n";
		$spice_string = $spice_string . ".measure tran c_drain_cmos avg cap(drain) FROM = t1s TO = t1e\n";
		$spice_string = $spice_string . ".measure tran c_gate_pass1 avg cap(gate) FROM = t0s TO = t0e\n";
		$spice_string = $spice_string . ".measure tran c_gate_pass2 avg cap(gate) FROM = t3s TO = t3e\n";
		$spice_string = $spice_string . ".measure tran c_gate_pass Param=('(c_gate_pass1 + c_gate_pass2)/2')\n";
		$spice_string = $spice_string . ".measure tran c_source_pass avg cap(source) FROM = t3s TO = t5e\n";
		$spice_string = $spice_string . ".measure tran c_drain_pass1 avg cap(drain) FROM = t1s TO = t1e\n";
		$spice_string = $spice_string . ".measure tran c_drain_pass2 avg cap(drain) FROM = t3s TO = t4e\n";
		$spice_string = $spice_string . ".measure tran c_drain_pass Param=('(c_drain_pass1 + 2*c_drain_pass2)/3')\n";

	} else {		
		$spice_string = $spice_string . ".measure tran c_gate_cmos avg cap(gate) FROM = t1s TO = t1e\n";
		$spice_string = $spice_string . ".measure tran c_drain_cmos avg cap(drain) FROM = t0s TO = t0e\n";
		$spice_string = $spice_string . ".measure tran c_gate_pass1 avg cap(gate) FROM = t1s TO = t2e\n";
		$spice_string = $spice_string . ".measure tran c_gate_pass2 avg cap(gate) FROM = t4s TO = t5e\n";
		$spice_string = $spice_string . ".measure tran c_gate_pass Param=('(c_gate_pass1 + c_gate_pass2)/2')\n";
		$spice_string = $spice_string . ".measure tran c_source_pass avg cap(source) FROM = t0s TO = t2e\n";
		$spice_string = $spice_string . ".measure tran c_drain_pass1 avg cap(drain) FROM = t0s TO = t1e\n";
		$spice_string = $spice_string . ".measure tran c_drain_pass2 avg cap(drain) FROM = t2s TO = t2e\n";
		$spice_string = $spice_string . ".measure tran c_drain_pass3 avg cap(drain) FROM = t5s TO = t5e\n";
		$spice_string = $spice_string . ".measure tran c_drain_pass Param=('(c_drain_pass1 + c_drain_pass2 + c_drain_pass3)/3')\n";
	}
	
	#$spice_string = $spice_string . ".measure tran c_gate_cmos avg cap(gate) FROM = cmosstart TO = cmosend\n";
	#$spice_string = $spice_string . ".measure tran c_source_cmos avg cap(source) FROM = cmosstart TO = cmosend\n";
	#$spice_string = $spice_string . ".measure tran c_drain_cmos avg cap(drain) FROM = cmosstart TO = cmosend\n";
	#$spice_string = $spice_string . ".measure tran c_gate_pass avg cap(gate) FROM = passstart TO = passend\n";
	#$spice_string = $spice_string . ".measure tran c_source_pass avg cap(source) FROM = passstart TO = passend\n";
	#$spice_string = $spice_string . ".measure tran c_drain_pass avg cap(drain) FROM = passstart TO = passend\n";
	$spice_string = $spice_string . ".end\n\n";
	
	open (SP_FILE, "> $spice_file");
	print SP_FILE $spice_string;
	close (SP_FILE);
	
	system ("$hspice $spice_file 1> $spice_out 2> /dev/null");
	
	my $spice_data;
	{
		local $/=undef;
		open (SP_FILE, "$spice_out");
		$spice_data = <SP_FILE>;
		close SP_FILE;
	}
	
	if ($spice_data =~ /transient analysis.*?leakage_current\s*=\s*[+-]*(\S+)/s) {
		$leakage = $1;	
	} else {
		die "Could not find leakage current in spice output.\n";
	}
	
	if ($spice_data =~ /transient analysis.*?c_gate_cmos\s*=\s*(\S+)/s) {
		$C_gate_cmos = $1;	
	} else {
		die "Could not find gate capacitance in spice output.\n";
	}
	
	if ($spice_data =~ /transient analysis.*?c_drain_cmos\s*=\s*(\S+)/s) {
		$C_drain_cmos = $1;	
	} else {
		die "Could not find drain capacitance in spice output.\n";
	}
	
	if ($spice_data =~ /transient analysis.*?c_gate_pass\s*=\s*(\S+)/s) {
		$C_gate_pass = $1;	
	} else {
		die "Could not find gate capacitance in spice output.\n";
	}
	
	if ($spice_data =~ /transient analysis.*?c_source_pass\s*=\s*(\S+)/s) {
		$C_source_pass = $1;	
	} else {
		die "Could not find source capacitance in spice output.\n";
	}
	
	if ($spice_data =~ /transient analysis.*?c_drain_pass\s*=\s*(\S+)/s) {
		$C_drain_pass = $1;	
	} else {
		die "Could not find drain capacitance in spice output.\n";
	}
	
	return ($leakage, $C_gate_cmos, $C_drain_cmos, $C_gate_pass, $C_source_pass, $C_drain_pass);
}