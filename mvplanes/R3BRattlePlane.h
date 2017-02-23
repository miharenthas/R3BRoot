/*******************************************************
//Author: L. Zanetti (lzanetti@ikp.tu-darmstadt.de)
//Date: 15.02.2017
//Description: This class defines a giant plane detector
//             that will register energy, momentum and
//             coordinates of any incoming particle.
//Usage: Unlike other detectors, this one can (and should)
//       be placed at runtime. To do so, you have to pass an
//       instace of the rp_trf structure, carrying information
//       about the position and rotation of the plane.
//       By default, the plane will be created at the origin
//       and perpendicular to the beam (Z) axis; the default
//       measures are specified by defined constants --and can
//       be modified with the transformation, of course.
//NOTE: needless to say, this is a probe for the simulation
//      not a real detector.
*******************************************************/

#ifndef R3BRATTLEPLANE__H
#define R3BRATTLEPLANE__H

//define a constant for the detector ID
#define RATTLEPLANE_DETECTOR_ID kRATTLEPLANE

#include <string>

#include "TClonesArray.h"
#include "TLorentzVector.h"
#include "TGeoMatrix.h"
#include "TGeoBBox.h"
#include "TVirtualMC.h" //for gMC
#include "TGeoManager.h" //for gGeoManager

#include "FairLogger.h"
#include "FairRootManager.h"

#include "R3BMCStack.h" //for R3BStack
#include "R3BDetector.h" //parent class
#include "R3BRPHit.h" //the rattle of the rattle plane

class R3BRattlePlane : public R3BDetector {
	public:
		//a handy structure to pass the information of the transformation
		typedef struct _r3brattleplane_specifications {
			double rot_x, rot_y, rot_z;
			double T_x, T_y, T_z;
			double width, height, depth;
		} rp_specs;

		//ctors, dtor.
		//As mentioned above, if not default constructed, this class
		//should be given an affine transformation to place the plane.
		R3BRattlePlane();
		//parametric constructor:
		// -- name: a string to name the thing. "Nebuchadnezzar" is the default.
		// -- active: a flag to switch on and off the detector.
		// -- trf: ...
		R3BRattlePlane( rp_specs &specs, const char *the_name, bool active );
		
		virtual ~R3BRattlePlane(){};
		
		//implementation of the various abstract method of the parent class
		virtual void Initialize();
		virtual Bool_t ProcessHits( FairVolume *fair_vol = NULL );
		virtual void EndOfEvent() { /*do nothing*/ };
		virtual void Register();
		virtual TClonesArray *GetCollection( Int_t iColl ) const; //retrieve the data
		                                                          // -- iColl: ???
		virtual void Print( Option_t *opt = "" ){ /*oh please*/ };
		virtual void Reset();
		virtual void ConstructGeometry(); //A very important method this one: the geometry
		                                  //of the detector is made in here and it's the
		                                  //whole point of this tatty class.
		//virtual void PostTrack(); //???
		virtual Bool_t CheckIfSensitive( std::string ){ return kTRUE; }; //it is sensitive

		//interpreter garbage
		ClassDef( R3BRattlePlane, 1 );
	protected: //i'm actually expecting to derive single neuLAND planes from this,
	           //because of bone-eyed lazyness. Stay tuned.
		TClonesArray *_rattle_hits; //it's a container for the event passing
		std::vector<R3BRPHit*> _hits; //the SANE container for the hits.
		rp_specs _specs; //the specs
		std::string _name; //keep track of the name

		//event awareness
		bool _is_new_event; //new event toggle (useful)
	private:
		R3BRattlePlane( const R3BRattlePlane &given ); //since we are burly programmers,
		                                               //this will support copy construction.
		//and, sure enough, we also do an assignment operator, shall we?
		R3BRattlePlane &operator=( R3BRattlePlane &right );
};

#endif
		
		
		

		
