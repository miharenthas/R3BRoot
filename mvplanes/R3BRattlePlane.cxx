/*******************************************************
//Author: L. Zanetti (lzanetti@ikp.tu-darmstadt.de)
//Date: 15.02.2017
//Description: Implementation of the class
//             R3BRattlePlane
*******************************************************/

#include "R3BRattlePlane.h"

//------------------------------------------------------------------------------------
//ctors:

//------------------------------------------------------------------------------------
//default:
R3BRattlePlane::R3BRattlePlane():
	R3BDetector( "Nebuchadnezzar", true, RATTLEPLANE_DETECTOR_ID ),
	_rattle_hits( "R3BRPHit" )
{;}

//------------------------------------------------------------------------------------
//parametric:
R3BRattlePlane::R3BRattlePlane( rp_trf trf, const char *the_name, bool active ):
	R3BDetector( the_name, active, RATTLEPLANE_DETECTOR_ID ),
	_rattle_hits( "R3BRPHit" )
{
	//copy the given transformation
	_trf = trf;
}

//------------------------------------------------------------------------------------
//copy:
R3BRattlePlane::R3BRattlePlane( const R3BRattlePlane &given ):
	R3BDetector( given._name.c_str(), true, RATTLEPLANE_DETECTOR_ID ),
	_rattle_hits( given._rattle_hits )
{
	_trf = given._trf;
}

//------------------------------------------------------------------------------------
//operators:

//------------------------------------------------------------------------------------
//assignment. Note that the TClonesArray doesn't just copy the pointers, but
//also duplicates its contents. So it's OK to do this and we don't get tangled.
R3BRattlePlane &R3BRattlePlane::operator=( R3BRattlePlane &right ){
	_rattle_hits = right._rattle_hits;
	_trf = right._trf;

	return *this;
}

//------------------------------------------------------------------------------------
//methods:

//------------------------------------------------------------------------------------
//Init the detector (basically, call some fair functions and put a line in the log).
void R3BRattlePlane::Initialize(){
	LOG( INFO ) << "R3BRattlePlane \"" << _name
	            << "\" is being itialized..." << FairLogger::endl;
	
	FairDetector::Initialize();
}

//------------------------------------------------------------------------------------
//ProcessHit method: does the thing necessary when the plane is stroke by a particle.
//To the best of my understanding, this gets called by the FairRunSim thingie.
Bool_t R3BRattlePlane::ProcessHits( FairVolume *the_volume ){
	//if the track is entering, set the data
	//which is what we are actually interested in.
	R3BRPHit *current_hit;
	if( gMC->IsTrackEntering() ){
		//create a new hit at the end of the hit array and
		//set our handy pointer to it.
		current_hit = new R3BRPHit;
		
		current_hit->_e_loss = 0; //init the energy loss into the rattleplane
		current_hit->_toa = gMC->TrackTime(); //time of arrival
		gMC->TrackPosition( current_hit->_poe ); //point of entry
		gMC->TrackMomentum( current_hit->_mae ); //momentum at entry
		
		//retrieve (immediately) the hierarchy info
		current_hit->_track_id = gMC->GetStack()->GetCurrentTrackNumber();
		current_hit->_parent_id = gMC->GetStack()->GetCurrentParentTrackNumber();

		//add the new hit
		_rattle_hits.AddLast( current_hit );
	//else if the track is leaving the rattle plane, or is dead in it
	} else if( gMC->IsTrackExiting() || gMC->IsTrackStop() || gMC->IsTrackDisappeared() ) {
		current_hit = (R3BRPHit*)_rattle_hits[ _rattle_hits.GetLast() ];
		
		current_hit->_tod = gMC->TrackTime(); //time of departure
		gMC->TrackPosition( current_hit->_pod ); //position of departure
		gMC->TrackMomentum( current_hit->_mad ); //momentum at departure
		
		//do something to the stack
		((R3BStack*)gMC->GetStack())->AddPoint( RATTLEPLANE_DETECTOR_ID );
	} else { //if it's just in it, just increment the energy
		current_hit = (R3BRPHit*)_rattle_hits[ _rattle_hits.GetLast() ];
		
		current_hit->_e_loss += gMC->Edep();
	}
	return kTRUE;
}

//------------------------------------------------------------------------------------
//Register: make the FairRootManager aware that we exist
void R3BRattlePlane::Register() {
	//Just this...
	//FairRootManager::Instance()->Register( "Rattles", GetName(), &_rattle_hits, true );
}

//------------------------------------------------------------------------------------
//GetCollection: returns a pointer to a TClonesArray. Since we are storing this locally
//because WHAT IS THE POINT of private variables if then you send out their address, we
//actually copy the array.
//NOTE: this will actually copy the elements of the array, which is a different behavior
//      than what I've seen around. If the deallocation doesn't happen elsewhere, then
//      this will lead to a memory leak. Will check on it and add facilities to remedy.
TClonesArray *R3BRattlePlane::GetCollection( Int_t iColl ) const {
	return new TClonesArray( _rattle_hits );
}

//------------------------------------------------------------------------------------
//Reset: possibly, make ready for the next event.
void R3BRattlePlane::Reset(){
	_rattle_hits.Clear(); //NOTE: this seems to actually clear out also the
	                      //      elements of the array. Which is safe, in this case
	                      //      because we sent out a copy of it.
}

//------------------------------------------------------------------------------------
//ConstructGeometry: the big deal of this class.
//A plane is created, RATTLEPLANE_THICKNESS cm thick, and transformed according to
//what's in _trf.
void R3BRattlePlane::ConstructGeometry(){
	//make the transformation. I'm given to understand that the order is respected,
	//so this should work --note: rotations and translaions are relative to the origin.
	fGlobalRot = new TGeoRotation( "Hugh", _trf.rot_x, _trf.rot_y, _trf.rot_z );
	fGlobalTrans = new TGeoTranslation( "Steven", _trf.T_x, _trf.T_y, _trf.T_z );
	
	fRefRot = new TGeoCombiTrans( "Gerald" );
	fRefRot->SetRotation( fGlobalRot ); //first, hopefully, rotation
	fRefRot->SetTranslation( *fGlobalTrans ); //then, hopefully, translation
	fRefRot->RegisterYourself();
	
	//create the volume
	_rp = new TGeoBBox( "Woodrow",
	                    RATTLEPLANE_SIDE,
	                    RATTLEPLANE_SIDE,
	                    RATTLEPLANE_THICKNESS );
	//create the medium
	//and it kinda had to be it, really...
	double par[20] = { 0, 0, 0, 0, 0, 0, 1e-4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	TGeoMedium *p_med_Pu = NULL;
	if( gGeoManager->GetMedium( "Plutonum" ) ){
		p_med_Pu = gGeoManager->GetMedium( "Plutonium" );
	} else {
		TGeoMaterial *Pu = new TGeoMaterial( "Plutonium",
		                                     244.,
		                                     93.,
		                                     19.816 );
		Pu->SetIndex( 999 );
		p_med_Pu = new TGeoMedium( "Plutonium", 999, Pu, par );
	}
	
	//finally, make the volume                             
	_rp_volume = new TGeoVolume( "Woodrow_volume", _rp, p_med_Pu );
	
	//register it in the wolrd
	TGeoRotation *zeroRot = new TGeoRotation; //zero rotation
	TGeoCombiTrans *tZero = new TGeoCombiTrans( "tZero", 0., 0., 0., zeroRot );
	TGeoVolume *p_world = gGeoManager->GetTopVolume();
	p_world->SetVisLeaves( kTRUE );

	p_world->AddNode( _rp_volume, 1, GetGlobalPosition( tZero ) );
}

//interpreter garbage
ClassImp( R3BRattlePlane );
