/*******************************************************
//Author: L. Zanetti (lzanetti@ikp.tu-darmstadt.de)
//Date: 16.02.2017
//Descripion: This class represents a hit in the
//            rattle plane (R3BRattlePlane).
//Usage: It's content is:
//       -The time of arrival
//       -The momentum and position of arrival
//       -The momentum and position of departure (or death)
//       -The track number and the number of its parent.
//       -The energy deposited into the rattle plane.
//       Deez quantities are directly accessible.
//NOTE: this is a class only because it has to inherit from
//      FairMCPoint (?), but it is a structure, really.
*******************************************************/

#ifndef R3BRPHIT__H
#define R3BRPHIT__H	

#include "TLorentzVector.h"

#include "FairMCPoint.h"

class R3BRPHit : public FairMCPoint {
	public:
		R3BRPHit():
			_e_loss( 0 ),
			_toa( 0 ),
			_tod( 0 ),
			_track_id( 0 ),
			_parent_id( 0 ) {};
		virtual ~R3BRPHit(){};
	
		double _e_loss;
		double _toa, _tod; //time of arrival and departure
		int _track_id, _parent_id; //hierarchy info
		TLorentzVector _poe, _pod; //position of arrival and departure
		TLorentzVector _mae, _mad; //moment at arrival and departure
		
		ClassDef( R3BRPHit, 1 );
};

#endif
