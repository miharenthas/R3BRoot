/*******************************************************
//Author: L. Zanetti (lzanetti@ikp.tu-darmstadt.de)
//Date: 04.01.2017
//Description: This class defines some media to be used
//             in the target assemblies. It does not
//             actually construct any geometry, nor
//             represents any actual detector --hence
//             it doesn't inherit from anything and
//             all its methods are static.
//NOTE: original implementation of these materials
//      has been done by J. Isaak (j.isaak@gsi.de)
*******************************************************/

#ifndef R3BTARGETASSEMBLYMEDIA__H
#define R3BTARGETASSEMBLYMEDIA__H

//ROOT includes
#include "TGeoManager.h"
#include "TGeoMedium.h"
#include "TGeoMixture.h"

//stl includes
#include <string.h> //memset

class R3BTargetAssemblyMedia {
	public:
		//Ctor, dtor: academic
		R3BTargetAssemblyMedia() {};
		~R3BTargetAssemblyMedia() {};
	
		//collection of methods that will
		//return the needed medium
		static TGeoMedium Air(); //returns air
		static TGeoMedium Vacuum(); //returns a common cranium's content...
		static TGeoMedium Silicon(); //returns silicon
		static TGeoMedium Copper(); //returns copper
		static TGeoMedium Aluminium(); //returns Aluminium, since we're in Europe
		static TGeoMedium Iron(); //returns irom
		static TGeoMedium Lead(); //returns -Zeppelin
		static TGeoMedium Fiberglass(); //returns the stuff glass fibers are made of
};

//this is a shorthand, since the typename is rather long
typedef R3BTargetAssemblyMedia R3BTAM;

#endif
	
	
