#!/usr/local/bin/perl

$prog_name = shift;
$prog_name = "i2" if $prog_name eq "";

open (PROCESSES, "ps -ax |") || die "unable to open pipe from ps\n";

while (<PROCESSES>) {
    next if /grep/;
    next if /perl/;
    if ( /$prog_name/ ) {
	#print "looking in $_";
	if ( /^\s*(\d+)\s+/ ) {
	    #print "$1\n";
	    #exit 0;
	    close(PROCESSES);
	    exec "/usr/bin/gdb $prog_name $1 -x GDB_di";
	}
    }
}
close(PROCESSES);

print "Unable to find $prog_name running.\n";

__END__
