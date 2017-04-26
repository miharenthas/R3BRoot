#! /bin/awk

#a tiny AWK script to generate a SED script to edit the dtof macro
#absolute edition

BEGIN { print "#! /bin/sed" };
$1 ~ "___[XYZ]___" {
	if( $2 == "dx" ) nb = ENVIRON["__DX"];
	else if( $2 ~ "dy" ) nb = ENVIRON["__DY"];
	else nb = ENVIRON["__DZ"];
	print NR "c", $1, $2, $3, nb ";" };
$1 ~ "___R_[XYZ]__" {
	if( $2 == "thx" ){
		th = ENVIRON["__THX"]; ph = ENVIRON["__PHX"];
	}
	else if( $2 == "thx" ){
		th = ENVIRON["__THY"]; ph = ENVIRON["__PHY"];
	}
	else{
		th = ENVIRON["__THZ"]; ph = ENVIRON["__PHZ"];
	}
	print NR "c", $1, $2, $3, th ";", $5, $6, ph ";" };
