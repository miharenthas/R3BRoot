/*******************************************************
//Author: L. Zanetti (lzanetti@ikp.tu-darmstadt.de)
//Date: 23.05.2017
//Description: Implementation of the class
//             R3BsegmentedRattlePlane.h
*******************************************************/

//------------------------------------------------------------------------------------
//NOTE: proper implementation coming soon
//      this will suffice for now.

//------------------------------------------------------------------------------------
//Get/make the material
TGeoMedium *R3BSegmentedRattlePlane::BuildMaterial( const char *material,
                                              FairGeoMedia *geoMedia,
                                              FairGeoBuilder *geoBuild )
{
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
