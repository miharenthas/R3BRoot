#! /bin/awk

#a tiny AWK script to move the DCH chamber (through a SED script)

BEGIN { print "#! /bin/sed" };
$1 ~ "___[XYZ][12]___" {
	if( $3 == "pDch1x" ) nb = $5 + ENVIRON["__DX1"];
	else if( $3 == "pDch1y" ) nb = $5 + ENVIRON["__DY1"];
	else if( $3 == "pDch1z" ) nb = $5 + ENVIRON["__DZ1"];
	else if( $3 == "pDch2x" ) nb = $5 + ENVIRON["__DX2"];
	else if( $3 == "pDch2y" ) nb = $5 + ENVIRON["__DY2"];
	else nb = $5 + ENVIRON["__DZ2"];
	print NR "c", $1, $2, $3, $4, nb ";" };
$1 ~ "___R_[XYZ][12]___" {
	if( $2 == "rx1" ) nb = $5 + ENVIRON["__RX1"];
	else if( $2 == "ry1" ) nb = $4 + ENVIRON["__RY1"];
	else if( $2 == "rz1" ) nb = $4 + ENVIRON["__RZ1"];
	else if( $2 == "rx2" ) nb = $4 + ENVIRON["__RX2"];
	else if( $2 == "ry2" ) nb = $4 + ENVIRON["__RY2"];
	else nb = $4 + ENVIRON["__RZ2"];
	print NR "c", $1, $2, $3, nb ";" };
