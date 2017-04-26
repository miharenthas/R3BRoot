#! /bin/bash

#A script to move the mTOF wall around the simulation.

#a flag to control whether the placement is absolute or relative
abs_flag=0 #default: false

#parse the input
while [ -n "$1" ]; do
	case $1 in
		dx )
			shift
			export __DX=$1
			;;
		dy )
			shift
			export __DY=$1
			;;
		dz )
			shift
			export __DZ=$1
			;;
		#this is a rotation along Y
		#the only interesting one.
		rot )
			shift
			export __THX=$((90+$1))
			export __THZ=$1
			;;
		reset )
			abs_flag=1
			export __DX=-155.824045
			export __DY=0.523976
			export __DZ=761.87
			export __THX=-106.7
			export __THZ=-16.7
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
	awk -f move_Xtof.awk create_mtof_geo.C > .sedpipe &
else
	awk -f move_Xtof_abs.awk create_mtof_geo.C > .sedpipe &
fi

#run SED
sed -i -f .sedpipe create_mtof_geo.C
rm -f .sedpipe

#run ROOT
root -l -q "create_mtof_geo.C( \"v13a\" )"




