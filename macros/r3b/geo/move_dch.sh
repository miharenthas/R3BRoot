#! /bin/bash

#A script to move the DCH wall around the simulation.

#a flag to control whether the placement is absolute or relative
abs_flag=0 #default: false

#parse the input
while [ -n "$1" ]; do
	case $1 in
		dx1 )
			shift
			export __DX1=$1
			;;
		dy1 )
			shift
			export __DY1=$1
			;;
		dz1 )
			shift
			export __DZ1=$1
			;;
		dx2 )
			shift
			export __DX2=$1
			;;
		dy2 )
			shift
			export __DY2=$1
			;;
		dz2 )
			shift
			export __DZ2=$1
			;;
		rx1 )
			shift
			export __RX1=$1
			;;
		ry1 )
			shift
			export __RY1=$1
			;;
		rz1 )
			shift
			export __RZ1=$1
			;;
		rx2 )
			shift
			export __RX2=$1
			;;
		ry2 )
			shift
			export __RY2=$1
			;;
		rz2 )
			shift
			export __RZ2=$1
			;;
		reset )
			abs_flag=1
			export __DX1=-123.219446
			export __DY1=3.597104
			export __DZ1=444.126271
			export __DX2=-167.015888
			export __DY2=1.016917
			export __DZ2=535.093884
			export __RX1=0
			export __RY1=-31.0
			export __RZ1=-8.880000
			export __RX2=0
			export __RY2=-31.0
			export __RZ2=9.35
			shift
			;;
		--absolute )
			abs_flag=1
			;;
		
		*)
			shift
			;;
	esac
done

#prepare the pipe for se
if ! [ -p ".sedpipe" ]; then
	mkfifo .sedpipe
fi

#run awk
if [ $abs_flag -eq 0 ]; then
	awk -f move_dch.awk create_dch_geo.C > .sedpipe &
else
	awk -f move_dch_abs.awk create_dch_geo.C > .sedpipe &
fi

#run SED
sed -i -f .sedpipe create_dch_geo.C
rm -f .sedpipe

#run ROOT
root -l -q "create_dch_geo.C( \"v13a\" )"
