/*******************************************************
//Author: L. Zanetti (lzanetti@ikp.tu-darmstadt.de)
//Date: 23.05.2017
//Description: Implementation of the class
//             R3BsegmentedRattlePlane.h
*******************************************************/

#include "R3BSegmentedRattlePlane.h"

//------------------------------------------------------------------------------------
//NOTE: proper implementation coming soon
//      this will suffice for now.

//------------------------------------------------------------------------------------
//Get/make the material
TGeoMedium *R3BSegmentedRattlePlane::BuildMaterial( const char *material )
{
	FairGeoLoader *geoLoad = FairGeoLoader::Instance();
	FairGeoInterface *geoFace = geoLoad->getGeoInterface();
	geoFace->setMediaFile( TString( gSystem->Getenv("VMCWORKDIR") ) + "/geometry/media_r3b.geo");
	geoFace->readMedia();
	FairGeoBuilder *geoBuild = geoLoad->getGeoBuilder();
	FairGeoMedia *geoMedia = geoFace->getMedia();

	FairGeoMedium *fairMedium = geoMedia->getMedium( material );
	if( !fairMedium ) {
		std::cerr << "FairGeoMedium " << material << " not found" << std::endl;
	}

	geoBuild->createMedium( fairMedium );
	TGeoMedium *med = gGeoManager->GetMedium( material );
	if( !med ){
		std::cerr << "TGeoMedium " << material << " not found" << std::endl;
	}
return med;
}
