/*******************************************************
//Author: L. Zanetti (lzanetti@ikp.tu-darmstadt.de)
//Date: 22.05.2017
//Description: Implementation of the class
//             R3BMvNeuLANDPlane.h
*******************************************************/

#include "R3BMvNeuLANDPlane.h"

//------------------------------------------------------------------------------------
//ctors:

//------------------------------------------------------------------------------------
//default:
R3BMvNEuLANDPlane::R3BMvNeuLANDPlane():
	R3BRattlePlane(), //default construct the rattleplane
	_neuland_hits( new TClonesArray( "R3BNeulandPoint", 1024 ) ) //this also has to
	                                                            //be initialized.
{}

//------------------------------------------------------------------------------------
//parametric:
R3BMvNeuLANDPlane::R3BMvNeuLANDPlane( rp_specs &specs, const char *the_name, bool active ):
	R3BRattlePlane( specs, the_name, active ),
	_neuland_hits( new TClonesArray( "R3BNeulandPoint", 1024 ) ) //this also has to
	                                                            //be initialized.
{}

//------------------------------------------------------------------------------------
//specialized methods:

//------------------------------------------------------------------------------------
//Register --also the neuland points
void R3BMvNeuLANDPlane::Register(){
	((R3BRattlePlane*)this)->Register(); //call the old code
	
	//make an unique name for the collection
	char name_buf[64];
	sprintf( name_buf, "NeuLAND_points_%06x", _own_index ); //can't be random here, sorry
	                                                 //at least if you want to be able
	                                                 //to conveniently join trees. 
	FairRootManager::Instance()->Register( name_buf, GetName(), _neuland_hits, kTRUE );
}	

//------------------------------------------------------------------------------------
//End of event for this as well...
void R3BMvNeuLANDPlane::EndOfEvent(){
	((R3BRattlePlane*)this)->EndOfEvent(); //call the old code

	for( int i=0; i < _neuland_points.size(); ++i ) delete _neuland_points[i];
	_neuland_points.clear();
	
	_neuland_hits->Clear(); //NOTE: this seems to actually clear out also the
	                        //      elements of the array. Which is safe, in this case
	                        //      because we sent out a copy of it.
}

//------------------------------------------------------------------------------------
//GetCollection: returns the two collections
TClonesArray *R3BMvNeuLANDPlane::GetCollection( Int_t iColl ) const {
	switch( iColl ){
		case 0:
			for( int i=0; i < _neuland_points.size(); ++i )
				(*_neuland_hits)[i] = _neuland_points[i];
			return _neuland_hits;
			break;
		case 1:
			for( int i=0; i < _hits.size(); ++i ) (*_rattle_hits)[i] = _hits[i];
			return _rattle_hits;
			break;
		default
			return NULL;
			break;
	}
}

//------------------------------------------------------------------------------------
//The tool for the light yield
inline double R3BMvNeuLANDPlane::GetLightYield( int charge, double length, double E ){
	if( charge == 0 || length <= 0 ) return 0; //exit on nonsense
	
	//handy constZ
	const double BirkdP = 1.032;
	const double BirkC1 = 0.012596899;
	const double BirkC2 = 9.013881377e-6;
	
	// Apply Birk's law (Adapted from G3BIRK/Geant3)
	double birkC1Mod = BirkC1;

	// Apply correction for higher charge states
	if( fabs(charge) >= 2 ) birkC1Mod *= 0.571428571;

	double dedxcm = 1000.*edep/length;
	double lightYield = edep/(1. + birkC1Mod*dedxcm + BirkC2*pow( dedxcm,2 ) );
	return lightYield;
}

//------------------------------------------------------------------------------------
//First big deal, the ProcessHits method
Bool_t R3BMvNeuLANDPlane::ProcessHits( FairVolume *fair_vol ){
	((R3BRattlePlane*)this)->ProcessHits(); //call the old code
	                                        //this should also guarantee that
	                                        //the last RPHit is correctly filled.
	//get hold of the last RPHit
	R3BRPHit *last_rph = _hits.back();
	
	//Do exactly what neuland does
	//NOTE: the _is_new_event toggle is operated by
	//      the old code, as many of the relevant information
	if( gMC->IsTrackEntering() ){ //the beginning of a new event
		_nsi.light_yield = 0;
		_nsi.t_length = gMC->TrackLength();
		gMC->CurrentVolOffID( 1, _nsi.paddle_id );
	}
	
	//get the light yield with a method stolen from neuland
	_nsi.light_yield = GetLightYield( gMC->TrackCharge(), gMC->TrackStep(), gMC->Edep() );		
	
	if( gMC->IsTrackExiting() || gMC->IsTrackStop() || gMC->IsTrackDisappeared() ){
		//behave as neuland and discard empty hits...
		if( last_rph->_e_loss == 0. ){
			EndOfEvent();
			return kFALSE;
		}
		
		//push back the new neuland point
		_neuland_points.push_back( new R3BNeulandPoint( last_rph->GetTrackId(),
		                                                _nsi.paddle_id,
		                                                last_rph->poe.Vect(),
		                                                last_rph->moe.Vect(),
		                                                last_rph->_toa,
		                                                _nsi.t_length,
		                                                lat_rph->_e_loss,
		                                                last_rph->GetEventId(),
		                                                _nsi.light_yield ) );
		
		//add the new point to the R3BStack
		((R3BStack*)gMC->GetStack())->AddPoint( kNEULAND );
		EndOfEvent();
	}
	
	return kTRUE;
}

//------------------------------------------------------------------------------------
//PostTrack: this method will be ported soon...
//TODO: PostTrack.

//------------------------------------------------------------------------------------
//Another big deal: the geometry constructor.
void R3BMvNeuLANDPlane::ConstructGeometry(){
	//TODO: assemble it from create_neuland_demo and helper_neuland_geometry
}
