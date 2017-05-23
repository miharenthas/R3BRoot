/*******************************************************
//Author: L. Zanetti (lzanetti@ikp.tu-darmstadt.de)
//Date: 23.05.2017
//Description: This class defines a segmented rattleplane
//             that will be used to parent movable TOF walls.
//Usage: This class inherits from the Rattleplane, and can
//       be placed in the same way.
*******************************************************/

#ifndef R3BSEGMENTEDRATTLEPLANE__H
#define R3BSEGMENTEDRATTLEPLANE__H

#include <string>

//unique name generation
#include <stdlib.h>
#include <string.h>

#include "TClonesArray.h"
#include "TLorentzVector.h"
#include "TGeoMatrix.h"
#include "TGeoBBox.h"
#include "TVirtualMC.h" //for gMC
#include "TGeoManager.h" //for gGeoManager

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairGeoMedia.h"
#include "FairGeoBuilder.h"

#include "R3BMCStack.h" //for R3BStack
#include "R3BRattlePlane.h" //The rattleplane, from which this
                            //inherits
#include "R3BRPHit.h" //will also produces rattles.
#include "R3BTargetAssemblyMedia.h" //This is here to provide materials, even
                                    //if this is not strictly its intended
                                    //application. Still, should work.


//NOTE: this class hasn't been implemented yet, it's just here
//      to provide intellectual consistency to the RP's family tree
//      BUT it WILL be implemented! So watch out!
class R3BSegmentedRattlePlane : public R3BRattlePlane {
	public:
		//A data structure to hold the
		//segments' spec
		typedef struct _segmented_rattle_plane_specs {
			int div_x, div_y, div_z;
			double spc_x, spc_y, spx_z;
		} sg_specs;
		
		//ctors, dtor.
		R3BSegmentedRattlePlane(): R3BRattlePlane() {};
		//parametric constructor:
		// -- specs: the spect structure containig the measures and
		//           placement of the rattleplane.
		// -- segments: the structure characteristic of this class
		//              containing the division specifications.
		// -- name: a string to name the thing. "Nebuchadnezzar" is the default.
		// -- active: a flag to switch on and off the detector.
		R3BSegmentedRattlePlane( rp_specs specs, sg_specs segments,
		                         const char *name, bool active = true ):
			R3BRattlePlane( specs, name, active ) {};
		
		virtual ~R3BSegmentedRattlePlane();
		
		virtual void ConstructGeometry() = 0; //let's make it abstract for now...
		
		ClassDef( R3BSegmentedRattlePlane, 1 );
	protected:
		sg_specs _segment; //the info over the segment.
	
		//some geometry-related helper functions
		//NOTE: this is the only bit I'm implementing right now,
		//      just because it's 0-cost.
		virtual TGeoMedium *BuildMaterial( const char *material,
		                                   FairGeoMedia *geoMedia,
		                                   FairGeoBuilder *geoBuild ); 
		virtual TGeoShape *BuildPaddleShape( const char *name,
		                                     const void *measures ) = 0;
		virtual TGeoVolume *BuildPaddleVolume() = 0;
	private:
		R3BSegmentedRattlePlane( const R3BSegmentedRattlePlane &given );
		R3BSegmentedRattlePlane &operator=( R3BSegmentedRattlePlane &right );
};

#endif
