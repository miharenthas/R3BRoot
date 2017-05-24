/*******************************************************
//Author: L. Zanetti (lzanetti@ikp.tu-darmstadt.de)
//Date: 22.05.2017
//Description: This class defines a NeuLAND plane that can
//             be placed at runtime. As for now, the
//             orientation of the paddles will be
//             JUST VERTICAL.
//Usage: This class inherits from the Rattleplane, and can
//       be placed in the same way --except the measures
//       of the detector and the material will be ignored.
*******************************************************/

#ifndef R3BMVNEULANDPLANE__H
#define R3BMVNEULANDPLANE__H

//define some geometry-related constants
#define GPADDLESPERPLANE 50
#define GPADDLEDISTANCE 2.5
#define GPADDLEBASELENGTH 125.0
#define GPADDLECONELENGTH 5.0
#define GBC408CONERADIUS 1.2
#define GBC408THICKNESS 2.4
#define GALTHICKNESS 0.02
#define GTAPETHICKNESS 0.05

#include <string>

//unique name generation
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "TClonesArray.h"
#include "TLorentzVector.h"
#include "TGeoMatrix.h"
#include "TGeoBBox.h"
#include "TGeoCone.h"
#include "TGeoShape.h"
#include "TGeoCompositeShape.h"
#include "TVirtualMC.h" //for gMC
#include "TGeoManager.h" //for gGeoManager

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairGeoMedia.h"
#include "FairGeoBuilder.h"

#include "R3BMCStack.h" //for R3BStack
#include "R3BSegmentedRattlePlane.h" //The rattleplane, from which this
                                     //inherits
#include "R3BRPHit.h" //will also produces rattles.
#include "R3BNeulandPoint.h" //This should produce neuland points, at least.
#include "R3BTargetAssemblyMedia.h" //This is here to provide materials, even
                                    //if this is not strictly its intended
                                    //application. Still, should work.


class R3BMvNeuLANDPlane : public R3BSegmentedRattlePlane {
	public:
		//NOTE: the "handy structure" rp_specs
		//      should be inherited as well
		
		//ctors, dtor.
		//As mentioned above, if not default constructed, this class
		//should be given an affine transformation to place the plane.
		R3BMvNeuLANDPlane();
		//parametric constructor:
		// -- specs: the spect structure containig the
		//           placement of the plane.
		// -- name: a string to name the thing. "Nebuchadnezzar" is the default.
		// -- active: a flag to switch on and off the detector
		R3BMvNeuLANDPlane( rp_specs &specs, const char *the_name, bool active=true );

		virtual ~R3BMvNeuLANDPlane();
		
		//These specializations are requires: one is the
		//geometry constructor, another is the hit processor
		//which will be as close as possible as the one from the R3BNeuland class.
		//The last ones are the data retriever, which has to be slightly modified,
		//and the register method, which has to register also the neulan ponts.
		//NOTE: ProcessHits is backed by the Rattleplane's infrastructure
		//      to extract interesting information about the incoming particle:
		//      this feature is KEPT.
		virtual Bool_t ProcessHits( FairVolume *fair_vol = NULL );
		virtual void ConstructGeometry();
		virtual TClonesArray *GetCollection( Int_t iColl ) const; //retrieve the data
		                                                          // -- iColl: is the
		                                                          //    "index" of the
		                                                          //    collection (data
		                                                          //    structure)
		                                                          // 0: NeuLAND points
		                                                          // 1: Rattle hits.
		virtual void Register();
		virtual void EndOfEvent();
		
		//this is new: a method to get the light yield of a neuland paddle
		virtual inline double GetLightYield( int charge, double length, double E );
		
		//This has to be ported, eventually:
		//TODO: PostTrack();
		
		//interpreter junk (irrelevant: these classes won't work with it).
		ClassDef( R3BMvNeuLANDPlane, 1 );
	protected:
		TClonesArray *_neuland_hits; //the pointer to the neuland points
		std::vector<R3BNeulandPoint*> _neuland_points; //the sane container for the points.
		
		//This is a structure that holds the neuland status
		//that is not yet represented by the RPHit.
		struct _neuland_status_info {
			int paddle_id;
			double light_yield, t_length;
		} _nsi;
		
		//some geometry-related helper functions (lifted straight
		//from the helper_neuland_geometry.C script)
		virtual TGeoShape *BuildPaddleShape( const char *name,
		                                     const void *measures );
		virtual TGeoVolume *BuildPaddleVolume();
		
		//the rest of the data members are inherited.
	private:
		R3BMvNeuLANDPlane( const R3BMvNeuLANDPlane &given ) {};
		R3BMvNeuLANDPlane &operator=( R3BMvNeuLANDPlane &right ) {};
};

#endif
