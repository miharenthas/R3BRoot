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
//This is somewhat lited from the create_neuland_demo_geo.C script
void R3BMvNeuLANDPlane::ConstructGeometry(){
	FairGeoLoader *fgl = FairGeoLoader::Instance(); //retrieve the geoloader
	
	TGeoVolume *paddle = BuildPaddleVolume(); //make the volume (active bit already registered)
	TGeoRotation *rot90 = new TGeoRotation();
	rot90->RotateZ( 90 ); //make a 90 rotation.
	
	TGeoVolume *volNeuland = new TGeoVolumeAssembly( "volNeuland" ); //NOTE: unique naming?
	
	int paddle_idx = 0; //paddle index...
	//loop-add the 50 paddles to the neuland volume.
	//NOTE: this creates a single plane, oriented for XZ angual
	//      resolution. Control structure may be added in the future.
	for( double a = -GPADDLESPERPLANE*GPADDLEDISTANCE + GPADDLEDISTANCE;
	     a < GPADDLEDISTANCE*GPADDLESPERPLANE;
	     a += GPADDLEDISTANCE*2 ){
	     	++paddle_idx;
	     	volNeuland->AddNode( paddle, paddle_idx, TGeoTranslation( a, 0, 
	     	                                                          -2*GPADDLEDISTANCE,
	     	                                                          rot90 );
	}
	
	//make the neuland plane transformation
	//code lifted straight from R3BRattlePlane::ConstructGeometry()
	char name_buf[128];
	
	//Names for the things are generated so that they are unique
	//this way, conflicts should be avoided in case we want more
	//than one rattleplane.
	strcpy( name_buf, "Hugh_" ); //make the template name
	R3BRattlePlane::mk_unique_name( name_buf ); //make the temp name.
	TGeoRotation *global_Rot = new TGeoRotation( name_buf );
	global_Rot->RotateX( _specs.rot_x );
	global_Rot->RotateY( _specs.rot_y );
	global_Rot->RotateZ( _specs.rot_z );

	strcpy( name_buf, "Steven_" ); //make the template name
	R3BRattlePlane::mk_unique_name( name_buf ); //make the temp name.
	TGeoTranslation *global_Trans = new TGeoTranslation( name_buf, 
	                                                     _specs.T_x,
	                                                     _specs.T_y,
	                                                     _specs.T_z );
	global_Rot->RegisterYourself();
	global_Trans->RegisterYourself();

	strcpy( name_buf, "Gerald_" ); //make the template name
	R3BRattlePlane::mk_unique_name( name_buf ); //make the temp name.
	TGeoCombiTrans *reference_trf = new TGeoCombiTrans( name_buf );
	reference_trf->SetRotation( *global_Rot ); //first, hopefully, rotation
	reference_trf->SetTranslation( *global_Trans ); //then, hopefully, translation
	reference_trf->RegisterYourself();
	
	TGeoVolume *p_world = gGeoManager->GetTopVolume(); //this should be the cave
	p_world->SetVisLeaves( kTRUE ); //set all visible
	p_world->AddNode( volNeuland, 1, GetGlobalPosition( reference_trf ) ); //add a neuland plane.
	
}

//------------------------------------------------------------------------------------
//geometry helper functions:

//------------------------------------------------------------------------------------
//Make the shape of one paddle:
TGeoShape *R3BMvNeuLANDPlane::BuildPaddleShape( const char *the_name,
                                                const void *measures )
{
	//NOTE: this method of retrieving the arguments is insecure
	//      but I can't be bothered to do it properly right now.
	//      in the future, proper checks on the buffer passed will
	//      be performed
	double length = *(double*)measures;
	double width = *(double*)measures + 1;
	double coneRadius = *(double*)measures + 2;
	double coneLength = *(double*)measures + 3;
	
	TString name( the_name );
	
	new TGeoBBox( name + "Box", length, width, width);
	new TGeoCone( name + "Cone",
	              coneLength + 0.001, 0.,
	              coneRadius, 0., width * sqrt(2.) );
	new TGeoBBox( name + "Conebox", width, width, coneLength);
	TGeoShape *shape = new TGeoCompositeShape( name,
	                                           name + "Box + ((" +
	                                           name + "Conebox*" +
	                                           name + "Cone):trc1) + ((" +
	                                           name + "Conebox*" +
	                                           name + "Cone):trc2)"	);
	return shape;
}

//------------------------------------------------------------------------------------
//make the volume of the paddle:
TGeoVolume *R3BMvNeuLANDPlane::BuildPaddleVolume(){
	// Load Interfaces to build materials
	FairGeoLoader *geoLoad = FairGeoLoader::Instance();
	FairGeoInterface *geoFace = geoLoad->getGeoInterface();
	geoFace->setMediaFile( TString( gSystem->Getenv("VMCWORKDIR") ) + "/geometry/media_r3b.geo");
	geoFace->readMedia();
	FairGeoBuilder *geoBuild = geoLoad->getGeoBuilder();
	FairGeoMedia *geoMedia = geoFace->getMedia();

	TGeoMedium *medBC408 = BuildMaterial( "BC408", geoMedia, geoBuild );
	TGeoMedium *medCH2 = BuildMaterial( "CH2", geoMedia, geoBuild );
	TGeoMedium *medAl = BuildMaterial( "aluminium", geoMedia, geoBuild );

	// Prepare Transformations for cones
	//NOTE: I don't actually know what's the ownership polocy here
	//      if ROOT's were sane, this would lead to a memory leak.
	//      But nothing about ROOT is sane, so if it ain't broken...
	TGeoRotation *r1 = new TGeoRotation();
	r1->RotateY( 90 );
	TGeoCombiTrans *trc1 = new TGeoCombiTrans( TGeoTranslation( -( GPADDLEBASELENGTH +
	                                                               GPADDLECONELENGTH ),
	                                                            0., 0.),
	                                           *r1 );
	trc1->SetName( "trc1" ); //NOTE: probably some unique naming will be needed here.
	trc1->RegisterYourself();

	//NOTE: see above.
	TGeoRotation *r2 = new TGeoRotation();
	r2->RotateY( -90 );
	TGeoCombiTrans *trc2 = new TGeoCombiTrans( TGeoTranslation( +( GPADDLEBASELENGTH +
	                                                               GPADDLECONELENGTH ),
	                                                            0., 0.),
	                                           *r2 );
	trc2->SetName( "trc2" ); //NOTE: probably, more unique naming...
	trc2->RegisterYourself();

	// Build shapes
	TGeoShape *shapeBC408 = BuildPaddleShape( "shapeBC408",
	                                           GPADDLEBASELENGTH,
	                                           GBC408THICKNESS,
	                                           GBC408CONERADIUS,
	                                           GPADDLECONELENGTH );

	TGeoShape *shapeAlWrapping = BuildPaddleShape( "shapeAlWrappingSolid",
                                                    GPADDLEBASELENGTH,
                                                    GBC408THICKNESS + GALTHICKNESS,
                                                    GBC408CONERADIUS +  GALTHICKNESS,
                                                    GPADDLECONELENGTH );
		          
	TGeoShape *shapeAlWrapping = new TGeoCompositeShape( "shapeAlWrapping",
	                                                      "shapeAlWrappingSolid - shapeBC408" );

	TGeoShape *shapeTapeWrapping = new BuildPaddleShape( "shapeTapeWrappingSolid",
	                                                     GPADDLEBASELENGTH,
	                                                     GBC408THICKNESS +
	                                                     GALTHICKNESS +
	                                                     GTAPETHICKNESS,
	                                                     GBC408CONERADIUS +
	                                                     GALTHICKNESS +
	                                                     GTAPETHICKNESS,
	                                                     GPADDLECONELENGTH );
	TGeoShape *shapeTapeWrapping = new TGeoCompositeShape( "shapeTapeWrapping",
	                                                       "shapeTapeWrappingSolid -\
	                                                        shapeAlWrappingSolid");

	// Build Volumes. Note that the volume names are important and reappear in R3BNeuland()
	//This is the active volume, and as such it should be registered
	//NOTE: unclear if unique naming should be used here...
	TGeoVolume *volBC408 = new TGeoVolume( "volBC408", shapeBC408, medBC408 );
	AddSensitiveVolume( volBC408 ); //adding this as active volume
	
	//These are passive.
	TGeoVolume *volAlWrapping = new TGeoVolume("volAlWrapping", shapeAlWrapping, medAl);
	TGeoVolume *volTapeWrapping = new TGeoVolume("volTapeWrapping", shapeTapeWrapping, medCH2);

	/Make the elementary assembly
	TGeoVolume *volPaddle = new TGeoVolumeAssembly("volPaddle");
	volPaddle->AddNode(volBC408, 1);
	volPaddle->AddNode(volAlWrapping, 1);
	volPaddle->AddNode(volTapeWrapping, 1);

	return volPaddle;
}
