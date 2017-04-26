#! /bin/awk

#a tiny AWK script to move the DCH chamber (through a SED script)
#absolute placement edition

BEGIN { print "#! /bin/sed" };
$1 ~ "___[XYZ][12]___" {
	if( $3 == "pDch1x" ) nb = ENVIRON["__DX1"];
	else if( $3 == "pDch1y" ) nb = ENVIRON["__DY1"];
	else if( $3 == "pDch1z" ) nb = ENVIRON["__DZ1"];
	else if( $3 == "pDch2x" ) nb = ENVIRON["__DX2"];
	else if( $3 == "pDch2y" ) nb = ENVIRON["__DY2"];
	else nb = ENVIRON["__DZ2"];
	print NR "c", $1, $2, $3, $4, nb ";" };
$1 ~ "___R_[XYZ][12]___" {
	if( $2 == "rx1" ) nb = ENVIRON["__RX1"];
	else if( $2 == "ry1" ) nb = ENVIRON["__RY1"];
	else if( $2 == "rz1" ) nb = ENVIRON["__RZ1"];
	else if( $2 == "rx2" ) nb = ENVIRON["__RX2"];
	else if( $2 == "ry2" ) nb = ENVIRON["__RY2"];
	else nb = ENVIRON["__RZ2"];
	print NR "c", $1, $2, $3, nb ";" };
