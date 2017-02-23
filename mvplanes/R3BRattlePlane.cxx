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
	_rattle_hits( new TClonesArray( "R3BRPHit" ) ),
	_is_new_event( true )
{}

//------------------------------------------------------------------------------------
//parametric:
R3BRattlePlane::R3BRattlePlane( rp_specs &specs, const char *the_name, bool active ):
	R3BDetector( the_name, active, RATTLEPLANE_DETECTOR_ID ),
	_rattle_hits( new TClonesArray( "R3BRPHit" ) ),
	_is_new_event( true )
{
	//copy the given transformation
	_specs = specs;
}

//------------------------------------------------------------------------------------
//copy:
R3BRattlePlane::R3BRattlePlane( const R3BRattlePlane &given ):
	R3BDetector( given._name.c_str(), true, RATTLEPLANE_DETECTOR_ID ),
	_rattle_hits( new TClonesArray( *given._rattle_hits ) ),
	_is_new_event( given._is_new_event )
{
	_specs = given._specs;
}

//------------------------------------------------------------------------------------
//operators:

//------------------------------------------------------------------------------------
//assignment. Note that the TClonesArray doesn't just copy the pointers, but
//also duplicates its contents. So it's OK to do this and we don't get tangled.
R3BRattlePlane &R3BRattlePlane::operator=( R3BRattlePlane &right ){
	_rattle_hits = right._rattle_hits;
	_specs = right._specs;

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
	if( gMC->IsTrackEntering() || (gMC->IsTrackInside() && _is_new_event) ){
		if( !_is_new_event ){ //the previous event never closed
			_hits.back()->Finish();
		}
		
		//set our handy pointer to a new hit.
		current_hit = new R3BRPHit;
		_is_new_event = false; //toggle the event status
		
		current_hit->_e_loss = gMC->Edep(); //init the energy loss into the rattleplane
		current_hit->_toa = gMC->TrackTime(); //time of arrival
		gMC->TrackPosition( current_hit->_poe ); //point of entry
		gMC->TrackMomentum( current_hit->_mae ); //momentum at entry
		current_hit->_particle = *gMC->GetStack()->GetCurrentTrack(); //download the full beef
		
		current_hit->SetTrackID( gMC->GetStack()->GetCurrentTrackNumber() );
		current_hit->SetDetectorID( RATTLEPLANE_DETECTOR_ID );
		current_hit->SetEventID( gMC->CurrentEvent() );
		
		//retrieve (immediately) the hierarchy info
		current_hit->_parent_id = gMC->GetStack()->GetCurrentParentTrackNumber();
		
		//save 
		_hits.push_back( current_hit );

	//else if the track is leaving the rattle plane, or is dead in it
	} else if( gMC->IsTrackExiting() || gMC->IsTrackStop() || gMC->IsTrackDisappeared() ) {
		current_hit = _hits.back();
		
		current_hit->_tod = gMC->TrackTime(); //time of departure
		gMC->TrackPosition( current_hit->_pod ); //position of departure
		gMC->TrackMomentum( current_hit->_mad ); //momentum at departure
		current_hit->SetLength( gMC->TrackLength() ); //the length of the track...
		
		//set the FairMCPoint parent object
		current_hit->Finish();

		//do something to the stack
		((R3BStack*)gMC->GetStack())->AddPoint( RATTLEPLANE_DETECTOR_ID );
		_is_new_event = true; //toggle the event status
	}
	
	//if it's just in it, just increment the energy
	current_hit = _hits.back();
		
	current_hit->_e_loss += gMC->Edep();
	return kTRUE;
}

/*------------------------------------------------------------------------------------
//The PostTrack method! Yay! What does it do?
void R3BRattlePlane::PostTrack(){
	ProcessHits( NULL ); //just redirect the call and do the hitting
}*/
	

//------------------------------------------------------------------------------------
//Register: make the FairRootManager aware that we exist
void R3BRattlePlane::Register() {
	//Just this...
	FairRootManager::Instance()->Register( "Rattles", GetName(), _rattle_hits, kTRUE );
}

//------------------------------------------------------------------------------------
//GetCollection: returns a pointer to a TClonesArray. 
TClonesArray *R3BRattlePlane::GetCollection( Int_t iColl ) const {
	//copy the hits
	for( int i=0; i < _hits.size(); ++i ) (*_rattle_hits)[i] = _hits[i];

	if( !iColl ) return _rattle_hits;
	else return NULL;
}

//------------------------------------------------------------------------------------
//Reset: possibly, make ready for the next event.
void R3BRattlePlane::Reset(){
	for( int i=0; i < _hits.size(); ++i ) delete _hits[i];
	_hits.clear();
	
	_rattle_hits->Clear(); //NOTE: this seems to actually clear out also the
	                       //      elements of the array. Which is safe, in this case
	                       //      because we sent out a copy of it.
	_is_new_event = true;
}

//------------------------------------------------------------------------------------
//ConstructGeometry: the big deal of this class.
//A plane is created, RATTLEPLANE_THICKNESS cm thick, and transformed according to
//what's in _specs.
//NOTE: So you may be wondering, why are we defining everything in here and then just
//      leaving everything as an unnamed place in memory?
//      First: ROOT destroys the concept of scope, effectively, so the various tranforms
//             are still known and anyway when the program exits everything is released.
//      Second: it's very unelegant but, in order to make the macro DisplayEvent work
//              we need things like this, otherwise we stumble on a double free, because
//              ROOT tries to deallocate pointers enclosed in the class.
void R3BRattlePlane::ConstructGeometry(){
	//make the transformation. I'm given to understand that the order is respected,
	//so this should work --note: rotations and translaions are relative to the origin.
	TGeoRotation *global_Rot = new TGeoRotation( "Hugh",
	                                             _specs.rot_x,
	                                             _specs.rot_y,
	                                             _specs.rot_z );
	TGeoTranslation *global_Trans = new TGeoTranslation( "Steven", 
	                                                     _specs.T_x,
	                                                     _specs.T_y,
	                                                     _specs.T_z );
	global_Rot->RegisterYourself();
	global_Trans->RegisterYourself();
	
	TGeoCombiTrans *reference_trf = new TGeoCombiTrans( "Gerald" );
	reference_trf->SetRotation( *global_Rot ); //first, hopefully, rotation
	reference_trf->SetTranslation( *global_Trans ); //then, hopefully, translation
	reference_trf->RegisterYourself();
	
	//create the volume
	TGeoBBox *rp = new TGeoBBox( "Woodrow",
	                              _specs.width,
	                              _specs.height,
	                              _specs.depth );
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
		                                     19.87 );
		Pu->SetIndex( 999 );
		p_med_Pu = new TGeoMedium( "Plutonium", 999, Pu, par );
	}
	
	//make the volume                             
	TGeoVolume *rp_volume = new TGeoVolume( "Woodrow_volume", rp, p_med_Pu );
	
	//and add it as sensitive
	//NOTE: this method is local and inherited form FariModule
	//      and seems to be the missing link between TGeo stuff and
	//      FairVolume stuff.
	AddSensitiveVolume( rp_volume );
	

	//register it in the wolrd
	TGeoVolume *p_world = gGeoManager->GetTopVolume();
	p_world->SetVisLeaves( kTRUE );

	p_world->AddNode( rp_volume, 1, GetGlobalPosition( reference_trf ) );
}

//interpreter garbage
ClassImp( R3BRattlePlane );
