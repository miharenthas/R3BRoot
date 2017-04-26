#! /bin/awk

#a tiny AWK script to generate a SED script to edit the dtof macro

BEGIN { print "#! /bin/sed" };
$1 ~ "___[XYZ]___" {
	if( $2 == "dx" ) nb = $4 + ENVIRON["__DX"];
	else if( $2 ~ "dy" ) nb = $4 + ENVIRON["__DY"];
	else nb = $4 + ENVIRON["__DZ"];
	print NR "c", $1, $2, $3, nb ";" };
$1 ~ "___R_[XYZ]__" {
	if( $2 == "thx" ){
		th = $4 + ENVIRON["__THX"]; ph = $7 + ENVIRON["__PHX"];
	}
	else if( $2 == "thx" ){
		th = $4 + ENVIRON["__THY"]; ph = $7 + ENVIRON["__PHY"];
	}
	else{
		th = $4 + ENVIRON["__THZ"]; ph = $7 + ENVIRON["__PHZ"];
	}
	print NR "c", $1, $2, $3, th ";", $5, $6, ph ";" };
