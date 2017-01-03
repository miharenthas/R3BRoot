// -------------------------------------------------------------------------
// -----                        R3BTra source file                     -----
// -----                  Created 26/03/09  by D.Bertini               -----
// -------------------------------------------------------------------------
#include <stdlib.h>

#include "R3BTra.h"

#include "R3BGeoTra.h"
#include "R3BTraPoint.h"
#include "R3BGeoTraPar.h"
#include "R3BMCStack.h"

#include "FairGeoInterface.h"
#include "FairGeoLoader.h"
#include "FairGeoNode.h"
#include "FairGeoRootBuilder.h"
#include "FairRootManager.h"
#include "FairRuntimeDb.h"
#include "FairRun.h"
#include "FairVolume.h"

#include "TClonesArray.h"
#include "TGeoMCGeometry.h"
#include "TParticle.h"
#include "TVirtualMC.h"
#include "TObjArray.h"

// includes for modeling
#include "TGeoManager.h"
#include "TParticle.h"
#include "TVirtualMC.h"
#include "TGeoMatrix.h"
#include "TGeoMaterial.h"
#include "TGeoMedium.h"
#include "TGeoBBox.h"
#include "TGeoPara.h"
#include "TGeoPgon.h"
#include "TGeoSphere.h"
#include "TGeoArb8.h"
#include "TGeoCone.h"
#include "TGeoTube.h"
#include "TGeoBoolNode.h"
#include "TGeoCompositeShape.h"
#include "TGeoShapeAssembly.h"

// -----   Default constructor   -------------------------------------------
R3BTra::R3BTra()
    : R3BDetector("R3BTra", kTRUE, kTRA)
{
    ResetParameters();
    fTraCollection = new TClonesArray("R3BTraPoint");
    fPosIndex = 0;
    kGeoSaved = kFALSE;
    flGeoPar = new TList();
    flGeoPar->SetName(GetName());
    fVerboseLevel = 1;
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
R3BTra::R3BTra(const char* name, Bool_t active)
    : R3BDetector(name, active, kTRA)
{
    ResetParameters();
    fTraCollection = new TClonesArray("R3BTraPoint");
    fPosIndex = 0;
    kGeoSaved = kFALSE;
    flGeoPar = new TList();
    flGeoPar->SetName(GetName());
    fVerboseLevel = 1;
}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
R3BTra::~R3BTra()
{
    if (flGeoPar)
        delete flGeoPar;
    if (fTraCollection)
    {
        fTraCollection->Delete();
        delete fTraCollection;
    }
}
// -------------------------------------------------------------------------

// ----   Initialize   -----------------------------------------------------
void R3BTra::Initialize()
{
    FairDetector::Initialize();

    LOG(INFO) << "R3BTra: initialisation" << FairLogger::endl;
    LOG(DEBUG) << "R3BTra: Sens. Vol. (McId) " << gMC->VolId("TraLog") << FairLogger::endl;
}

void R3BTra::SetSpecialPhysicsCuts()
{
    LOG(INFO) << "-I- R3BTra: Adding customized Physics cut ... " << FairLogger::endl;

    if (gGeoManager)
    {
        TGeoMedium* pSi = gGeoManager->GetMedium("silicon");
        if (pSi)
        {
            // Setting processes for Si only
            gMC->Gstpar(pSi->GetId(), "LOSS", 3);
            gMC->Gstpar(pSi->GetId(), "STRA", 1.0);
            gMC->Gstpar(pSi->GetId(), "PAIR", 1.0);
            gMC->Gstpar(pSi->GetId(), "COMP", 1.0);
            gMC->Gstpar(pSi->GetId(), "PHOT", 1.0);
            gMC->Gstpar(pSi->GetId(), "ANNI", 1.0);
            gMC->Gstpar(pSi->GetId(), "BREM", 1.0);
            gMC->Gstpar(pSi->GetId(), "HADR", 1.0);
            gMC->Gstpar(pSi->GetId(), "DRAY", 1.0);
            gMC->Gstpar(pSi->GetId(), "DCAY", 1.0);
            gMC->Gstpar(pSi->GetId(), "MULS", 1.0);
            gMC->Gstpar(pSi->GetId(), "RAYL", 1.0);

            // Setting Energy-CutOff for Si Only
            Double_t cutE = fCutE; // GeV-> 1 keV

            LOG(INFO) << "-I- R3BTra: silicon Medium Id " << pSi->GetId() << " Energy Cut-Off : " << cutE << " GeV" << FairLogger::endl;

            // Si
            gMC->Gstpar(pSi->GetId(), "CUTGAM", cutE); /** gammas (GeV)*/
            gMC->Gstpar(pSi->GetId(), "CUTELE", cutE); /** electrons (GeV)*/
            gMC->Gstpar(pSi->GetId(), "CUTNEU", cutE); /** neutral hadrons (GeV)*/
            gMC->Gstpar(pSi->GetId(), "CUTHAD", cutE); /** charged hadrons (GeV)*/
            gMC->Gstpar(pSi->GetId(), "CUTMUO", cutE); /** muons (GeV)*/
            gMC->Gstpar(pSi->GetId(), "BCUTE", cutE);  /** electron bremsstrahlung (GeV)*/
            gMC->Gstpar(pSi->GetId(), "BCUTM", cutE);  /** muon and hadron bremsstrahlung(GeV)*/
            gMC->Gstpar(pSi->GetId(), "DCUTE", cutE);  /** delta-rays by electrons (GeV)*/
            gMC->Gstpar(pSi->GetId(), "DCUTM", cutE);  /** delta-rays by muons (GeV)*/
            gMC->Gstpar(pSi->GetId(), "PPCUTM", -1.);  /** direct pair production by muons (GeV)*/
        }
        // <DB> trick to remove too much internal
        // tracking in the Aladin magnet yoke
        TGeoMedium* pFe = gGeoManager->GetMedium("iron");

        if (pFe)
        {
            Double_t cutM = 1.e-01; // 100 MeV
            gMC->Gstpar(pFe->GetId(), "CUTELE", cutM);
            gMC->Gstpar(pFe->GetId(), "DRAY", 0.0);
        }

    } //!gGeoManager
}

// -----   Public method ProcessHits  --------------------------------------
Bool_t R3BTra::ProcessHits(FairVolume* vol)
{
    if (gMC->IsTrackEntering())
    {
        fELoss = 0.;
        fTime = gMC->TrackTime() * 1.0e09;
        fLength = gMC->TrackLength();
        gMC->TrackPosition(fPosIn);
        gMC->TrackMomentum(fMomIn);
    }

    // Sum energy loss for all steps in the active volume
    fELoss += gMC->Edep();

    // Set additional parameters at exit of active volume. Create R3BTraPoint.
    if (gMC->IsTrackExiting() || gMC->IsTrackStop() || gMC->IsTrackDisappeared())
    {
        fTrackID = gMC->GetStack()->GetCurrentTrackNumber();
        fVolumeID = vol->getMCid();
        fDetCopyID = vol->getCopyNo(); // added by Marc
        gMC->TrackPosition(fPosOut);
        gMC->TrackMomentum(fMomOut);
//        if (fELoss == 0.)
//            return kFALSE;

        if (gMC->IsTrackExiting())
        {
            const Double_t* oldpos;
            const Double_t* olddirection;
            Double_t newpos[3];
            Double_t newdirection[3];
            Double_t safety;

            gGeoManager->FindNode(fPosOut.X(), fPosOut.Y(), fPosOut.Z());
            oldpos = gGeoManager->GetCurrentPoint();
            olddirection = gGeoManager->GetCurrentDirection();

            for (Int_t i = 0; i < 3; i++)
            {
                newdirection[i] = -1 * olddirection[i];
            }

            gGeoManager->SetCurrentDirection(newdirection);
            //   TGeoNode *bla = gGeoManager->FindNextBoundary(2);
            safety = gGeoManager->GetSafeDistance();

            gGeoManager->SetCurrentDirection(-newdirection[0], -newdirection[1], -newdirection[2]);

            for (Int_t i = 0; i < 3; i++)
            {
                newpos[i] = oldpos[i] - (3 * safety * olddirection[i]);
            }

            fPosOut.SetX(newpos[0]);
            fPosOut.SetY(newpos[1]);
            fPosOut.SetZ(newpos[2]);
        }

        AddHit(fTrackID,
               fVolumeID,
               fDetCopyID, // added by Marc
               TVector3(fPosIn.X(), fPosIn.Y(), fPosIn.Z()),
               TVector3(fPosOut.X(), fPosOut.Y(), fPosOut.Z()),
               TVector3(fMomIn.Px(), fMomIn.Py(), fMomIn.Pz()),
               TVector3(fMomOut.Px(), fMomOut.Py(), fMomOut.Pz()),
               fTime,
               fLength,
               fELoss);

        // Increment number of TraPoints for this track
        R3BStack* stack = (R3BStack*)gMC->GetStack();
        stack->AddPoint(kTRA);

        ResetParameters();
    }

    return kTRUE;
}

// -----   Public method EndOfEvent   -----------------------------------------
void R3BTra::BeginEvent()
{
}

// -----   Public method EndOfEvent   -----------------------------------------
void R3BTra::EndOfEvent()
{
    if (fVerboseLevel)
        Print();
    fTraCollection->Clear();

    ResetParameters();
}
// ----------------------------------------------------------------------------

// -----   Public method Register   -------------------------------------------
void R3BTra::Register()
{
    FairRootManager::Instance()->Register("TraPoint", GetName(), fTraCollection, kTRUE);
}
// ----------------------------------------------------------------------------

// -----   Public method GetCollection   --------------------------------------
TClonesArray* R3BTra::GetCollection(Int_t iColl) const
{
    if (iColl == 0)
        return fTraCollection;
    else
        return NULL;
}
// ----------------------------------------------------------------------------

// -----   Public method Print   ----------------------------------------------
void R3BTra::Print(Option_t* option) const
{
    Int_t nHits = fTraCollection->GetEntriesFast();
    LOG(INFO) << "R3BTra: " << nHits << " points registered in this event" << FairLogger::endl;
}
// ----------------------------------------------------------------------------

// -----   Public method Reset   ----------------------------------------------
void R3BTra::Reset()
{
    fTraCollection->Clear();
    ResetParameters();
}
// ----------------------------------------------------------------------------

// -----   Public method CopyClones   -----------------------------------------
void R3BTra::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset)
{
    Int_t nEntries = cl1->GetEntriesFast();
    LOG(INFO) << "R3BTra: " << nEntries << " entries to add" << FairLogger::endl;
    TClonesArray& clref = *cl2;
    R3BTraPoint* oldpoint = NULL;
    for (Int_t i = 0; i < nEntries; i++)
    {
        oldpoint = (R3BTraPoint*)cl1->At(i);
        Int_t index = oldpoint->GetTrackID() + offset;
        oldpoint->SetTrackID(index);
        new (clref[fPosIndex]) R3BTraPoint(*oldpoint);
        fPosIndex++;
    }
    LOG(INFO) << "R3BTra: " << cl2->GetEntriesFast() << " merged entries" << FairLogger::endl;
}

// -----   Private method AddHit   --------------------------------------------
R3BTraPoint* R3BTra::AddHit(Int_t trackID,
                            Int_t detID,
                            Int_t detCopyID, // added by Marc
                            TVector3 posIn,
                            TVector3 posOut,
                            TVector3 momIn,
                            TVector3 momOut,
                            Double_t time,
                            Double_t length,
                            Double_t eLoss)
{
    TClonesArray& clref = *fTraCollection;
    Int_t size = clref.GetEntriesFast();
    if (fVerboseLevel > 1)
        LOG(INFO) << "R3BTra: Adding Point at (" << posIn.X() << ", " << posIn.Y() << ", " << posIn.Z() << ") cm,  detector " << detID << ", track " << trackID
                  << ", energy loss " << eLoss * 1e06 << " keV" << FairLogger::endl;
    return new (clref[size]) R3BTraPoint(trackID,
                                         detID,
                                         detCopyID,
                                         posIn,
                                         posOut, // detCopyID added by Marc
                                         momIn,
                                         momOut,
                                         time,
                                         length,
                                         eLoss);
}

/* -----   Public method ConstructGeometry   ----------------------------------
void R3BTra::ConstructGeometry()
{
    TString fileName = GetGeometryFileName();
    if (fileName.EndsWith(".root"))
    {
        LOG(INFO) << "Constructing TRACKER geometry from ROOT file " << fileName.Data() << FairLogger::endl;
        ConstructRootGeometry();
    }
    else
    {
        LOG(FATAL) << "TRACKER Geometry file is not specified" << FairLogger::endl;
        exit(-1);
    }
}*/

Bool_t R3BTra::CheckIfSensitive(std::string name)
{
    if (TString(name).Contains("TraLog"))
    {
        return kTRUE;
    }
    return kFALSE;
}

// -----   Public method ConstructGeometry   ----------------------------------
void R3BTra::ConstructGeometry() {

	// out-of-file geometry definition
	Double_t dx,dy,dz;
	Double_t par[20];
	Double_t rmin, rmax, rmin1, rmax1, rmin2, rmax2;
	Double_t a;
	Double_t thx, phx, thy, phy, thz, phz;
	Double_t  phi1, phi2;
	Double_t  theta1, theta2; // 17.09.14 (j.isaak@gsi.de)
	Double_t z, density, radl, absl, w;
	Double_t tx,ty,tz;
	Int_t nel, numed;

	TGeoRotation * zeroRot = new TGeoRotation; //zero rotation
	TGeoCombiTrans * tZero = new TGeoCombiTrans("tZero", 0., 0., 0., zeroRot);
	tZero->RegisterYourself();

	/****************************************************************************/
	/********        Material definition         ********************************************/

	// Mixture: Air
	TGeoMedium * pMedAir=NULL;
	if (gGeoManager->GetMedium("Air") ){
		pMedAir=gGeoManager->GetMedium("Air");
	}else{
		nel     = 2;
		density = 0.001290;
		TGeoMixture*
			pMat2 = new TGeoMixture("Air", nel,density);
		a = 14.006740;   z = 7.000000;   w = 0.700000;  // N
		pMat2->DefineElement(0,a,z,w);
		a = 15.999400;   z = 8.000000;   w = 0.300000;  // O
		pMat2->DefineElement(1,a,z,w);
		pMat2->SetIndex(1);
		// Medium: Air
		numed   = 1;  // medium number
		for(int i = 0; i<8;i++){par[i] = 0.000000;} 
		pMedAir = new TGeoMedium("Air", numed,pMat2, par);
	}

	//--------------------------------------------------------------------

	// Mixture: Vacuum
	TGeoMedium * pMedVac=NULL;
	if (gGeoManager->GetMedium("Vacuum") ){
		pMedVac=gGeoManager->GetMedium("Vacuum");
	}else{
		nel     = 2;
		density = 0.0000001; //????????????????
		TGeoMixture* pMat33 = new TGeoMixture("Vacuum", nel,density);
		a = 14.006740;   z = 7.000000;   w = 0.700000;  // N
		pMat33->DefineElement(0,a,z,w);
		a = 15.999400;   z = 8.000000;   w = 0.300000;  // O
		pMat33->DefineElement(1,a,z,w);
		pMat33->SetIndex(32);
		// Medium: Air
		numed   = 1;  // medium number
		for(int i = 0; i<8;i++){par[i] = 0.000000;} 
		pMedVac = new TGeoMedium("Vacuum", numed,pMat33, par);
	}

	//-------------------------------------------------------------------

	// Fill Chamber: Vacuum or Air. Needed still: an external call interface for choosing which.
	TGeoMedium * pMedFill=NULL;
		//pMedFill = new TGeoMedium("Fill_Air", numed,pMat2, par);
		//pMedFill = (TGeoMedium*) pMedAir->Clone();
		//pMedFill->SetName("Fill_Air");
		pMedFill = (TGeoMedium*) pMedVac->Clone();
		pMedFill->SetName("Fill_Vacuum");

	//-------------------------------------------------------------------

	// Material: Silicon
	TGeoMedium * pMedSi=NULL;
	if (gGeoManager->GetMedium("Silicon") ){
		pMedSi=gGeoManager->GetMedium("Silicon");
	}else{
		a       = 28.090000;
		z       = 14.000000;
		density = 2.330000;
		radl    = 9.351106;
		absl    = 456.628489;
		TGeoMaterial*
			pMat22 = new TGeoMaterial("Silicon", a,z,density,radl,absl);

		pMat22->SetIndex(21);
		// Medium: Silicon
		numed   = 21;  // medium number

		par[0]  = 0.000000; // isvol
    par[1]  = 0.000000; // ifield
    par[2]  = 0.000000; // fieldm
    par[3]  = 0.000000; // tmaxfd
    par[4]  = 0.000000; // stemax
    par[5]  = 0.000000; // deemax
    par[6]  = 0.000100; // epsil
    par[7]  = 0.000000; // stmin

		pMedSi = new TGeoMedium("Silicon", numed,pMat22,par);
	}

	//------------------------------------------------------------------

	// Material: Copper
	TGeoMedium * pMedCu=NULL;
	if (gGeoManager->GetMedium("Copper") ){
		pMedCu=gGeoManager->GetMedium("Copper");
	}else{
		a       = 63.540000;
		z       = 29.000000;
		density = 8.960000;
		radl    = 1.435029;
		absl    = 155.874854;
		TGeoMaterial*
			pMat25 = new TGeoMaterial("Copper", a,z,density,radl,absl);
		pMat25->SetIndex(24);
		// Medium: Copper
		numed   = 24;  // medium number

     par[0]  = 0.000000; // isvol
     par[1]  = 0.000000; // ifield
     par[2]  = 0.000000; // fieldm
     par[3]  = 0.000000; // tmaxfd
     par[4]  = 0.000000; // stemax
     par[5]  = 0.000000; // deemax
     par[6]  = 0.000100; // epsil
     par[7]  = 0.000000; // stmin

		pMedCu = new TGeoMedium("Copper", numed,pMat25, par);
	}

	//------------------------------------------------------------------

	// Material: Aluminum
	TGeoMedium * pMedAl=NULL;
	if (gGeoManager->GetMedium("Aluminum") ){
		pMedAl=gGeoManager->GetMedium("Aluminum");
	}else{
		a       = 26.980000;
		z       = 13.000000;
		density = 2.700000;
		radl    = 8.875105;
		absl    = 388.793113;
		TGeoMaterial*
			pMat21 = new TGeoMaterial("Aluminum", a,z,density,radl,absl);
		pMat21->SetIndex(20);
		// Medium: Aluminum
		numed   = 20;  // medium number

		par[0]  = 0.000000; // isvol
		par[1]  = 0.000000; // ifield
		par[2]  = 0.000000; // fieldm
		par[3]  = 0.000000; // tmaxfd
		par[4]  = 0.000000; // stemax
		par[5]  = 0.000000; // deemax
		par[6]  = 0.000100; // epsil
		par[7]  = 0.000000; // stmin

		pMedAl = new TGeoMedium("Aluminum", numed,pMat21, par);
	}

	//------------------------------------------------------------------
	TGeoMedium * pMedFe=NULL;
	if (gGeoManager->GetMedium("Iron") ){
		pMedFe=gGeoManager->GetMedium("Iron");
	}else{
		a       = 55.850000;
		z       = 26.000000;
		density = 7.870000;
		radl    = 1.757717;
		absl    = 169.994418;
		TGeoMaterial*
			pMatFe = new TGeoMaterial("Iron", a,z,density,radl,absl);
		pMatFe->SetIndex(701);
		numed   = 23;  // medium number
		par[0]  = 0.000000; // isvol
		par[1]  = 0.000000; // ifield
		par[2]  = 0.000000; // fieldm
		par[3]  = 0.000000; // tmaxfd
		par[4]  = 0.000000; // stemax
		par[5]  = 0.000000; // deemax
		par[6]  = 0.000100; // epsil
		par[7]  = 0.000000; // stmin
		pMedFe = new TGeoMedium("Iron", numed,pMatFe, par);
	}

	/**********************************************************************/
	/********  16.09.14 adding lead as material (j.isaak@gsi.de)  *********/
	/**********************************************************************/

	//------------------------------------------------------------------
	TGeoMedium * pMedPb=NULL;
	if (gGeoManager->GetMedium("Lead") ){
		pMedPb=gGeoManager->GetMedium("Lead");
	}else{
		a       = 207.20000;
		z       = 82.000000;
		density = 13.340000;
//		radl    = 1.757717;
//		absl    = 169.994418;
		TGeoMaterial*
			pMatPb = new TGeoMaterial("Lead", a,z,density); //,radl,absl);
		pMatPb->SetIndex(702);
		numed   = 28;  // medium number
		par[0]  = 0.000000; // isvol
		par[1]  = 0.000000; // ifield
		par[2]  = 0.000000; // fieldm
		par[3]  = 0.000000; // tmaxfd
		par[4]  = 0.000000; // stemax
		par[5]  = 0.000000; // deemax
		par[6]  = 0.000100; // epsil
		par[7]  = 0.000000; // stmin
		pMedPb = new TGeoMedium("Lead", numed,pMatPb, par);
	}
	//------------------------------------------------------------------


	//Material fiberglass (for SSD electronics board)
	//TGeoMedium * pMedEglass=NULL;
	//if (gGeoManager->GetMedium("pMedEglass") ){
	//	pMedAir=gGeoManager->GetMedium("pMedEglass");
	//}else{
	//	nel     = 4;
	//	density = 1.850000;
	//	TGeoMixture* pMatFG = new TGeoMixture("pMedEglass", nel,density);
	//	a = 14.006740;   z = 7.000000;   w = 0.700000;  // N
	//	pMatFG->DefineElement(0,a,z,w);
	//	a = 15.999400;   z = 8.000000;   w = 0.300000;  // O
	//	pMatFG->DefineElement(1,a,z,w);
	//	pMatFG->SetIndex(1);
	//	// Medium: Air
	//	numed   = 1;  // medium number
	//	for(int i = 0; i<8;i++){par[i] = 0.000000;} 
	//	pMedAir = new TGeoMedium("Air", numed,pMat2, par);
	//}



	// Shape: World type: TGeoBBox
	TGeoVolume* pWorld = gGeoManager->GetTopVolume();
	pWorld->SetVisLeaves(kTRUE);


	/******************************************************/
	/*************      MontagePlatform  ************************/
	/*****************************************************/

	//Combi transformation:
	dx = 0.000000;
	dy = -14.090000;
	dz = 0.000000;
	// Rotation:
	thx = 90.000000;    phx = 0.000000;
	thy = 0.000000;    phy = 0.000000;
	thz = 90.000000;    phz = 270.000000;
	TGeoRotation *pMatrix15 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
	TGeoCombiTrans* pMatrix14 = new TGeoCombiTrans("", dx,dy,dz,pMatrix15);


	rmin = 2.750000;
	rmax = 18.000000;
	dz   = 0.150000;
	phi1 = 0.000000;
	phi2 = 360.000000;
	TGeoShape *pMontagePlatform = new TGeoTubeSeg("MontagePlatform",rmin,rmax,dz,phi1,phi2);
	TGeoVolume*  pMontagePlatformLog = new TGeoVolume("MontagePlatformLog",pMontagePlatform, pMedCu);

	/*********************************************************************/
	/*************** MontageRing type *************************************/
	/********************************************************************/
	dx = 0.000000;
	dy = -17.000000;
	dz = 0.000000;
	// Rotation: 
	thx = 90.000000;    phx = 0.000000;
	thy = 0.000000;    phy = 0.000000;
	thz = 90.000000;    phz = 270.000000;
	TGeoRotation *      pMatrix17 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
	TGeoCombiTrans*  pMatrix16 = new TGeoCombiTrans("", dx,dy,dz,pMatrix17);

	rmin = 12.000000;
	rmax = 15.000000;
	dz   = 0.250000;
	phi1 = 0.000000;
	phi2 = 360.000000;
	TGeoShape *  pMontageRing = new TGeoTubeSeg("MontageRing",rmin,rmax,dz,phi1,phi2);
	TGeoVolume*  pMontageRingLog = new TGeoVolume("MontageRingLog",pMontageRing, pMedAl);

	//	/*********************************************************************/
	//	/*************************** TargetWheel ******************************/
	//	/*********************************************************************/
	//	dx = -6.240000;
	//	dy = -3.900000;
	//	dz = 0.000000;
	//	// Rotation: 
	//	thx = 90.000000;    phx = 0.000000;
	//	thy = 90.000000;    phy = 90.000000;
	//	thz = 0.000000;    phz = 0.000000;
	//	TGeoRotation *      pMatrix29 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
	//	TGeoCombiTrans*  pMatrix28 = new TGeoCombiTrans("", dx,dy,dz,pMatrix29);
	//
	//
	//	rmin = 0.410000;
	//	//rmax = 5.300000;
	//	rmax = 6.300000;
	//	dz   = 0.100000;
	//	phi1 = 0.000000;
	//	phi2 = 360.000000;
	//	TGeoShape *   ptargetWheel = new TGeoTubeSeg("targetWheel",rmin,rmax,dz,phi1,phi2);
	//	TGeoVolume*  ptargetWheelLog = new TGeoVolume("targetWheelLog",ptargetWheel, pMedAl);
	//
	//	/*********** **********   TargetWheel2 ********************/
	//	dx = -6.240000;
	//	dy = -3.900000;
	//	dz = 0.000000;
	//	//Rotation: 
	//	thx = 90.000000;    phx = 0.000000;
	//	thy = 90.000000;    phy = 90.000000;
	//	thz = 0.000000;    phz = 0.000000;
	//	TGeoRotation *     pMatrix31 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
	//	TGeoCombiTrans* pMatrix30 = new TGeoCombiTrans("", dx,dy,dz,pMatrix31);
	//
	//
	//	//rmin = 9.500000;
	//	rmin = 8.500000;
	//	rmax = 10.000000;
	//	dz   = 0.100000;
	//	phi1 = 0.000000;
	//	phi2 = 360.000000;
	//	TGeoShape *   ptargetWheel2 = new TGeoTubeSeg("targetWheel2",rmin,rmax,dz,phi1,phi2);
	//	TGeoVolume*  ptargetWheel2Log = new TGeoVolume("targetWheel2Log",ptargetWheel2, pMedAl);

	//=================== Target Wheel Correct geometry =======================

	//The Air box for the target frame
	dx = 1.550000;       //s318
	dy = 1.550000;       //s318
	dz = 0.2;      
	TGeoShape * TargetFrame = new TGeoBBox("pTargetFrame", dx,dy,dz);
	//Target wheel
	rmin = 0.410000;
	rmax = 10.00000;
	dz   = 0.100000;
	phi1 = 0.000000;
	phi2 = 360.000000;
	TGeoShape *   targetWheelShape = new TGeoTubeSeg("pTargetWheelShape",rmin,rmax,dz,phi1,phi2);

	// Create non-overlapping volumes
	TGeoCombiTrans * twZero = new TGeoCombiTrans("twZero", 0., 0., 0., zeroRot);
	twZero->RegisterYourself();

	//Transformations for the frame
	//TGeoRotation * zeroRot = new TGeoRotation; //zero rotation
	TGeoCombiTrans * tShift = new TGeoCombiTrans("tShift", 6.2400, 3.900, 0.0, zeroRot);
	tShift->RegisterYourself();

	TGeoCompositeShape * tWheel = new TGeoCompositeShape("TargWheel", "pTargetWheelShape - pTargetFrame:tShift");
	TGeoVolume * pTargetWheelLog = new TGeoVolume("TargetWheelLog", tWheel, pMedAl);
	//vAlCaseLog->SetVisLeaves(kTRUE);

	//Transformation
	dx = -6.240000;
	dy = -3.900000;
	//dz = 0.100000;
	dz = 0.000000;

	thx = 90.000000;    phx = 0.000000;
	thy = 90.000000;    phy = 90.000000;
	thz = 0.000000;    phz = 0.000000;
	TGeoRotation *      pMatrix29 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
	TGeoCombiTrans*  pMatrix28 = new TGeoCombiTrans("", dx,dy,dz,pMatrix29);




	/*********** **********   Target motor ********************/
	dx = -6.240000;
	dy = -3.900000;
	dz = -4.370000;
	//Rotation: 
	thx = 90.000000;    phx = 0.000000;
	thy = 90.000000;    phy = 90.000000;
	thz = 0.000000;    phz = 0.000000;
	TGeoRotation *     pMatrix01 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
	TGeoCombiTrans* pMatrix02 = new TGeoCombiTrans("", dx,dy,dz,pMatrix01);

	//rmin = 9.500000;
	rmin = 0.1500000;
	rmax = 2.100000;
	dz   = 2.7900000;
	phi1 = 0.000000;
	phi2 = 360.000000;
	TGeoShape *   ptargetMotor = new TGeoTubeSeg("targetMotor",rmin,rmax,dz,phi1,phi2);
	//TGeoVolume*  ptargetMotorLog = new TGeoVolume("targetMotorLog",ptargetMotor, pMedAl);
	TGeoVolume*  ptargetMotorLog = new TGeoVolume("targetMotorLog",ptargetMotor, pMedFe);

	/*********** **********   Motor holder ********************/
	dx = 3.000000;
	dy = 6.100000;
	dz = 0.500000;
	TGeoShape *   pMotorHolder1 = new TGeoBBox("MotorHolder1", dx,dy,dz);
	TGeoVolume*  pMotorHolderLog1 = new TGeoVolume("MotorHolderLog1",pMotorHolder1, pMedAl);

	dx = 3.480000;
	dy = 2.300000;
	dz = 0.500000;
	TGeoShape *   pMotorHolder2 = new TGeoBBox("MotorHolder2", dx,dy,dz);
	TGeoVolume*  pMotorHolderLog2 = new TGeoVolume("MotorHolderLog2",pMotorHolder2, pMedAl); 

	dx = -13.64000;
	dy = -7.70000;
	dz = -1.05000;
	//Rotation: 
	thx = 90.000000;    phx = 0.000000;
	thy = 90.000000;    phy = 90.00000;
	thz = 0.000000;    phz = 0.000000;
	TGeoRotation *     pMatrix03 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
	TGeoCombiTrans* pMatrix04 = new TGeoCombiTrans("", dx,dy,dz,pMatrix03);

	dx = -7.14000;
	dy = -3.90000;
	dz = -1.05000;

	TGeoRotation *     pMatrix05 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
	TGeoCombiTrans* pMatrix06 = new TGeoCombiTrans("", dx,dy,dz,pMatrix05);

	/*********************************************************************/
	/***********************  SSD electronics box   ***************************/
	/*********************************************************************/

	dx = 4.050000;
	dy = 3.465000;
	dz = 0.800000;
	TGeoShape *   pinnerElectronicsBox = new TGeoBBox("innerElectronicsBox", dx,dy,dz);

	dx = 4.070000;
	dy = 3.365000;
	dz = 0.700000;

	TGeoShape *   pinnerElectronicsVac = new TGeoBBox("innerElectronicsVac", dx,dy,dz);

	//===========  Create non-overlapping volumes =================

	//TGeoRotation * zeroRot = new TGeoRotation;
	//TGeoCombiTrans * tZero = new TGeoCombiTrans("tZero", 0., 0., 0., zeroRot);
	//tZero->RegisterYourself();

	TGeoCompositeShape * vAlCase = new TGeoCompositeShape("AlCasing", "innerElectronicsBox - innerElectronicsVac:tZero");
	TGeoVolume * vAlCaseLog = new TGeoVolume("AlCasingLog", vAlCase, pMedAl);

	//vAlCaseLog->SetVisLeaves(kTRUE);

	//==============================================================

	dx = 4.090000;
	dy = 3.362000;
	dz = 0.050000;

	TGeoShape *   pinnerElectronicsBoard = new TGeoBBox("innerElectronicsBoard", dx,dy,dz);
	TGeoVolume*  pinnerElectronicsBoardLog = new TGeoVolume("innerElectronicsBoardLog",pinnerElectronicsBoard, pMedSi);

	/**************  Transformations for the electronics *************************/

	dx = 0.000000;
	dy = -8.000000;
	dz = 10.820000;
	// Rotation: 
	thx = 90.000000;    phx = 270.000000;
	thy = 90.000000;    phy = 0.000000;
	thz = 0.000000;    phz = 0.000000;
	TGeoRotation *       pMatrix19 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
	TGeoCombiTrans*   pMatrix18 = new TGeoCombiTrans("", dx,dy,dz,pMatrix19);

	//*****************************************
	dx = -2.100000;
	dy = -8.000000;
	dz = 4.470000;
	// Rotation: 
	thx = 90.000000;    phx = 90.000000;
	thy = 0.000000;    phy = 0.000000;
	thz = 90.000000;    phz = 360.000000;
	TGeoRotation *      pMatrix21 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
	TGeoCombiTrans*  pMatrix20 = new TGeoCombiTrans("", dx,dy,dz,pMatrix21);

	//********************************************
	dx = 2.100000;
	dy = 8.000000;
	dz = 4.470000;
	// Rotation: 
	thx = 90.000000;    phx = 90.000000;
	thy = 180.000000;    phy = 0.000000;
	thz = 90.000000;    phz = 180.000000;
	TGeoRotation *      pMatrix23 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
	TGeoCombiTrans*  pMatrix22 = new TGeoCombiTrans("", dx,dy,dz,pMatrix23);

	//********************************************
	dx = 8.000000;
	dy = -2.100000;
	dz = 4.470000;
	//Rotation: 
	thx = 90.000000;    phx = 0.000000;
	thy = 180.000000;    phy = 0.000000;
	thz = 90.000000;    phz = 90.000000;
	TGeoRotation *      pMatrix25 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
	TGeoCombiTrans*  pMatrix24 = new TGeoCombiTrans("", dx,dy,dz,pMatrix25);

	//*******************************************
	dx = -8.000000;
	dy = 2.100000;
	dz = 4.470000;
	// Rotation: 
	thx = 90.000000;    phx = 0.000000;
	thy = 180.000000;    phy = 0.000000;
	thz = 90.000000;    phz = 90.000000;
	TGeoRotation *       pMatrix27 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
	TGeoCombiTrans*   pMatrix26 = new TGeoCombiTrans("", dx,dy,dz,pMatrix27);

	//*********************************************

	dx = 0.000000;
	dy = 8.000000;
	dz = 13.470000;
	// Rotation: 
	thx = 90.000000;    phx = 270.000000;
	thy = 90.000000;    phy = 0.000000;
	thz = 0.000000;    phz = 0.000000;
	TGeoRotation *       pMatrix33 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
	TGeoCombiTrans*   pMatrix32 = new TGeoCombiTrans("", dx,dy,dz,pMatrix33);



	//*********************************************************************/
	//**********   Copper holding structure for the box- SSDs (upstream part)********/
	/********************************************************************/
	Double_t Hdx = 0.500000; //X-thickness
	Double_t Hdy = 0.250000;//Y-thickness

	Double_t Hz = 1.20000;// Z-positioning


	dx = Hdx;
	dy = Hdy;
	dz = 8.400000;
	TGeoShape *   pHolderBox1 = new TGeoBBox("pHolderBox1", dx,dy,dz);
	TGeoVolume*  pHolderBoxLog1 = new TGeoVolume("pHolderBoxLog1",pHolderBox1, pMedCu);

	dx = -14.1400;
	dy = -5.40000;
	dz = Hz;
	// Rotation: 
	thx = 0.000000;      phx = 0.000000;
	thy = 90.00000;      phy = 0.000000;
	thz = 90.000000;    phz = 90.000000;

	TGeoRotation *       pMatrixHold11 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
	TGeoCombiTrans*   pMatrixHold1 = new TGeoCombiTrans("", dx,dy,dz,pMatrixHold11);

	dx = -3.27000;
	dy = -5.40000;
	dz = Hz;

	TGeoRotation *       pMatrixHold21= new TGeoRotation("",thx,phx,thy,phy,thz,phz);
	TGeoCombiTrans*   pMatrixHold2 = new TGeoCombiTrans("", dx,dy,dz,pMatrixHold21);

	/**********************************************************/
	dx = Hdx;
	dy = Hdy;
	dz = 5.1500;
	TGeoShape *   pHolderBox3 = new TGeoBBox("pHolderBox3", dx,dy,dz);
	TGeoVolume*  pHolderBoxLog3 = new TGeoVolume("pHolderBoxLog3",pHolderBox3, pMedCu);

	dx = 12.1400;
	dy = -8.70;
	dz = Hz;
	// Rotation: 
	thx = 0.000000;      phx = 0.000000;
	thy = 90.00000;      phy = 0.000000;
	thz = 90.000000;    phz = 90.000000;

	TGeoRotation *       pMatrixHold31 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
	TGeoCombiTrans*   pMatrixHold3 = new TGeoCombiTrans("", dx,dy,dz,pMatrixHold31);

	/**********************************************************/
	dx = Hdx;
	dy = Hdy;
	dz = 7.4500;
	TGeoShape *   pHolderBox4 = new TGeoBBox("pHolderBox4", dx,dy,dz);
	TGeoVolume*  pHolderBoxLog4 = new TGeoVolume("pHolderBoxLog4",pHolderBox4, pMedCu);

	dx = 3.27000;
	dy = 4.4500;
	dz = Hz;
	// Rotation: 
	thx = 0.000000;      phx = 0.000000;
	thy = 90.00000;      phy = 0.000000;
	thz = 90.000000;    phz = 90.000000;

	TGeoRotation *       pMatrixHold41 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
	TGeoCombiTrans*   pMatrixHold4 = new TGeoCombiTrans("", dx,dy,dz,pMatrixHold41);

	/**********************************************************/
	dx = Hdx;
	dy = Hdy;
	dz = 8.700;
	TGeoShape *   pHolderBox5 = new TGeoBBox("pHolderBox5", dx,dy,dz);
	TGeoVolume*  pHolderBoxLog5 = new TGeoVolume("pHolderBoxLog5",pHolderBox5, pMedCu);

	dx = -5.7000;
	dy = 3.27000;
	dz = Hz;
	// Rotation: 
	thx = 180.000000;  phx = 0.000000;
	thy = 90.00000;      phy = 90.000000;
	thz = 90.000000;    phz = 0.000000;

	TGeoRotation *       pMatrixHold51 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
	TGeoCombiTrans*   pMatrixHold5 = new TGeoCombiTrans("", dx,dy,dz,pMatrixHold51); 

	/**********************************************************/
	dx = Hdx;
	dy = Hdy;
	dz = 7.7000;
	TGeoShape *   pHolderBox6 = new TGeoBBox("pHolderBox6", dx,dy,dz);
	TGeoVolume*  pHolderBoxLog6 = new TGeoVolume("pHolderBoxLog6",pHolderBox6, pMedCu);

	dx = 4.7000;
	dy = -3.27000;
	dz = Hz;
	// Rotation: 
	thx = 180.000000;  phx = 0.000000;
	thy = 90.00000;      phy = 90.000000;
	thz = 90.000000;    phz = 0.000000;

	TGeoRotation *       pMatrixHold61 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
	TGeoCombiTrans*   pMatrixHold6 = new TGeoCombiTrans("", dx,dy,dz,pMatrixHold61);


	//*********************************************************************/
	//**********   Copper holding structure for the box- SSDs (downstream part)********/
	/********************************************************************/

	Hz = 7.80000;// Z-positioning


	dx = Hdx;
	dy = Hdy;
	dz = 8.400000;
	TGeoShape *   pHolderBox7 = new TGeoBBox("pHolderBox7", dx,dy,dz);
	TGeoVolume*  pHolderBoxLog7 = new TGeoVolume("pHolderBoxLog7",pHolderBox7, pMedCu);

	dx = -14.1400;
	dy = -5.40000;
	dz = Hz;
	// Rotation: 
	thx = 0.000000;      phx = 0.000000;
	thy = 90.00000;      phy = 0.000000;
	thz = 90.000000;    phz = 90.000000;

	TGeoRotation *       pMatrixHold71 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
	TGeoCombiTrans*   pMatrixHold7 = new TGeoCombiTrans("", dx,dy,dz,pMatrixHold71);

	dx = -3.27000;
	dy = -5.40000;
	dz = Hz;

	TGeoRotation *       pMatrixHold81= new TGeoRotation("",thx,phx,thy,phy,thz,phz);
	TGeoCombiTrans*   pMatrixHold8 = new TGeoCombiTrans("", dx,dy,dz,pMatrixHold81);

	/**********************************************************/
	dx = Hdx;
	dy = Hdy;
	dz = 5.1500;
	TGeoShape *   pHolderBox9 = new TGeoBBox("pHolderBox9", dx,dy,dz);
	TGeoVolume*  pHolderBoxLog9 = new TGeoVolume("pHolderBoxLog9",pHolderBox9, pMedCu);

	dx = 12.1400;
	dy = -8.70;
	dz = Hz;
	// Rotation: 
	thx = 0.000000;      phx = 0.000000;
	thy = 90.00000;      phy = 0.000000;
	thz = 90.000000;    phz = 90.000000;

	TGeoRotation *       pMatrixHold91 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
	TGeoCombiTrans*   pMatrixHold9 = new TGeoCombiTrans("", dx,dy,dz,pMatrixHold91);

	/**********************************************************/
	dx = Hdx;
	dy = Hdy;
	dz = 7.4500;
	TGeoShape *   pHolderBox10 = new TGeoBBox("pHolderBox10", dx,dy,dz);
	TGeoVolume*  pHolderBoxLog10 = new TGeoVolume("pHolderBoxLog10",pHolderBox10, pMedCu);

	dx = 3.27000;
	dy = 4.4500;
	dz = Hz;
	// Rotation: 
	thx = 0.000000;      phx = 0.000000;
	thy = 90.00000;      phy = 0.000000;
	thz = 90.000000;    phz = 90.000000;

	TGeoRotation *       pMatrixHold101 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
	TGeoCombiTrans*   pMatrixHold10 = new TGeoCombiTrans("", dx,dy,dz,pMatrixHold101);

	/**********************************************************/
	dx = Hdx;
	dy = Hdy;
	dz = 8.700;
	TGeoShape *   pHolderBox11 = new TGeoBBox("pHolderBox11", dx,dy,dz);
	TGeoVolume*  pHolderBoxLog11 = new TGeoVolume("pHolderBoxLog11",pHolderBox11, pMedCu);

	dx = -5.7000;
	dy = 3.27000;
	dz = Hz;
	// Rotation: 
	thx = 180.000000;  phx = 0.000000;
	thy = 90.00000;      phy = 90.000000;
	thz = 90.000000;    phz = 0.000000;

	TGeoRotation *       pMatrixHold11_0 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
	TGeoCombiTrans*   pMatrixHold11_1 = new TGeoCombiTrans("", dx,dy,dz,pMatrixHold11_0); 

	/**********************************************************/
	dx = Hdx;
	dy = Hdy;
	dz = 7.7000;
	TGeoShape *   pHolderBox12 = new TGeoBBox("pHolderBox12", dx,dy,dz);
	TGeoVolume*  pHolderBoxLog12 = new TGeoVolume("pHolderBoxLog12",pHolderBox12, pMedCu);

	dx = 4.7000;
	dy = -3.27000;
	dz = Hz;
	// Rotation: 
	thx = 180.000000;  phx = 0.000000;
	thy = 90.00000;      phy = 90.000000;
	thz = 90.000000;    phz = 0.000000;

	TGeoRotation *       pMatrixHold121 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
	TGeoCombiTrans*   pMatrixHold12 = new TGeoCombiTrans("", dx,dy,dz,pMatrixHold121);

	//*********************************************************************/
	//**************   Copper holding structure for the in-beam SSDs ***************/
	/********************************************************************/

	//Long Holder
	dx = Hdx;
	dy = Hdy;
	dz = 12.50000;
	TGeoShape *   pHolderSSD1 = new TGeoBBox("pHolderSSD1", dx,dy,dz);
	TGeoVolume*  pHolderSSDLog1 = new TGeoVolume("pHolderSSDLog1",pHolderSSD1, pMedCu);

	//Short holder
	dz = 5.10000;
	TGeoShape *   pHolderSSD2 = new TGeoBBox("pHolderSSD2", dx,dy,dz);
	TGeoVolume*  pHolderSSDLog2 = new TGeoVolume("pHolderSSDLog2",pHolderSSD2, pMedCu);

	//POsitions of the long one
	dx = 3.0000;
	dy = -1.400;
	dz = 12.400;
	// Rotation: 
	thx = 90.000000;    phx = 0.000000;
	thy = 0.00000;      phy = 0.000000;
	thz = 90.000000;    phz = 270.000000;

	TGeoRotation *       pMatrixSSD11 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
	TGeoCombiTrans*   pMatrixSSD1 = new TGeoCombiTrans("", dx,dy,dz,pMatrixSSD11);

	dx = -3.0000;
	dy = -1.4000;
	dz = 12.4000;
	// Rotation: 
	thx = 90.000000;    phx = 0.000000;
	thy = 0.00000;      phy = 0.000000;
	thz = 90.000000;    phz = 270.000000;

	TGeoRotation *       pMatrixSSD21 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
	TGeoCombiTrans*   pMatrixSSD2 = new TGeoCombiTrans("", dx,dy,dz,pMatrixSSD21);

	//Positions of the short one

	dx = 2.8000;
	dy = -8.8000;
	dz = 11.8900;

	TGeoRotation *       pMatrixSSD31 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
	TGeoCombiTrans*   pMatrixSSD3 = new TGeoCombiTrans("", dx,dy,dz,pMatrixSSD31);

	dx = -2.8000;
	dy = -8.8000;
	dz = 11.8900;

	TGeoRotation *       pMatrixSSD41 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
	TGeoCombiTrans*   pMatrixSSD4 = new TGeoCombiTrans("", dx,dy,dz,pMatrixSSD41);
	//**************************************************************//
	//*********************   Si Sensors ******************************//
	//***************************************************************/

	// Si Shape & volume: TraBox type: TGeoBBox
	dx = 3.600000;
	dy = 2.070000;
	dz = 0.015000;
	// Volume: TraLog
	TGeoVolume *TraLog = gGeoManager->MakeBox("TraLog",pMedSi,dx,dy,dz);

	//TRANSFORMATION MATRICES
	dx = 0.000000;
	dy = 0.000000;
	dz = 13.50000;
	// Rotation: 
	thx = 90.000000;    phx = 0.000000;
	thy = 90.000000;    phy = 90.000000;
	thz = 0.000000;    phz = 0.000000;
	TGeoRotation *      pMatrix3 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
	TGeoCombiTrans*  pMatrix2 = new TGeoCombiTrans("", dx,dy,dz,pMatrix3);

	//Combi transformation: 
	dx = 0.000000;
	dy = 0.000000;
	dz = 10.850000;
	// Rotation: 
	thx = 90.000000;    phx = 0.000000;
	thy = 90.000000;    phy = 90.000000;
	thz = 0.000000;    phz = 0.000000;
	TGeoRotation *       pMatrix5 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
	TGeoCombiTrans*   pMatrix4 = new TGeoCombiTrans("", dx,dy,dz,pMatrix5);

	//Combi transformation: 
	dx = 0.000000;
	dy = -2.100000;
	dz = 4.470000;
	// Rotation: 
	thx = 0.000000;    phx = 0.000000;
	thy = 90.000000;    phy = 0.000000;
	thz = 90.000000;    phz = 90.000000;
	TGeoRotation *       pMatrix7 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
	TGeoCombiTrans*   pMatrix6 = new TGeoCombiTrans("", dx,dy,dz,pMatrix7);

	//Combi transformation: 
	dx = 0.000000;
	dy = 2.100000;
	dz = 4.470000;
	// Rotation: 
	thx = 180.000000;    phx = 0.000000;
	thy = 90.000000;    phy = 0.000000;
	thz = 90.000000;    phz = 270.000000;
	TGeoRotation *	    pMatrix9 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
	TGeoCombiTrans*   pMatrix8 = new TGeoCombiTrans("", dx,dy,dz,pMatrix9);

	//Combi transformation: 
	dx = 2.100000;
	dy = 0.000000;
	dz = 4.470000;
	// Rotation: 
	thx = 180.000000;    phx = 0.000000;
	thy = 90.000000;    phy = 90.000000;
	thz = 90.000000;    phz = 0.000000;
	TGeoRotation *        pMatrix11 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
	TGeoCombiTrans*   pMatrix10 = new TGeoCombiTrans("", dx,dy,dz,pMatrix11);

	//Combi transformation: 
	dx = -2.100000;
	dy = 0.000000;
	dz = 4.470000;
	// Rotation: 
	thx = 0.000000;    phx = 0.000000;
	thy = 90.000000;    phy = 90.000000;
	thz = 90.000000;    phz = 180.000000;
	TGeoRotation *	    pMatrix13 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
	TGeoCombiTrans*   pMatrix12 = new TGeoCombiTrans("", dx,dy,dz,pMatrix13);



	/************ Medium inside reaction chamber, around tracker ****************/
	TGeoShape *aTraMedium_ReactionChamber = new TGeoSphere("aTraMedium_ReactionChamber", 0., 24.250000);	//inner reaction chamber volume
	TGeoVolume *aTraMedium_ReactionChamberLog = new TGeoVolume("aTraMedium_ReactionChamberLog", aTraMedium_ReactionChamber, pMedFill);	//pMedVac or pMedAir
	
	//three conical in out and bottom beam tubes. need additional short cylinders in order to match chamber with cones.
	dz = 14.600000;
	rmin1 = 0.0;
	rmax1 = 2.730000;
	rmin2 = 0.0;
	rmax2 = 5.125000;
	phi1 = 0.000000;
	phi2 = 360.000000;
	TGeoShape *aTraMedium_ConicalTube = new TGeoConeSeg("aTraMedium_ConicalTube", dz,rmin1,rmax1,rmin2,rmax2,phi1,phi2);
	TGeoVolume* aTraMedium_ConicalTubeLog = new TGeoVolume("aTraMedium_ConicalTubeLog",aTraMedium_ConicalTube, pMedFill);
	// Combi transformation: 
	dx = 0.000000;
	dy = 0.000000;
	dz = 39.100000;
	// Rotation: 
	TGeoCombiTrans* pCombTrans1 = new TGeoCombiTrans("", dx,dy,dz, zeroRot);
	// Combi transformation: 
	dx = 0.000000;
	dy = 0.000000;
	dz = -39.100000;
	// Rotation:
	TGeoRotation *pRot1 = new TGeoRotation("", 0, 180, 0);
	TGeoCombiTrans* pCombTrans2 = new TGeoCombiTrans("", dx,dy,dz, pRot1);
	// Combi transformation:
	dx = 0.000000; 
	dy = -39.100000;
	dz = 0.000000;
	// Rotation: 
	TGeoRotation *pRot2 = new TGeoRotation("", 0, 90, 0);
	TGeoCombiTrans* pCombTrans3 = new TGeoCombiTrans("", dx,dy,dz, pRot2);
	aTraMedium_ConicalTubeLog->SetVisLeaves(kTRUE);
	aTraMedium_ConicalTubeLog->SetLineColor(33);
	aTraMedium_ReactionChamberLog->AddNode(aTraMedium_ConicalTubeLog, 0, pCombTrans1);
	aTraMedium_ReactionChamberLog->AddNode(aTraMedium_ConicalTubeLog, 1, pCombTrans2);
	aTraMedium_ReactionChamberLog->AddNode(aTraMedium_ConicalTubeLog, 2, pCombTrans3);
	
	//need in and out bridging filling cylindrical volumes; also: bottom hole volume
	//placement
	dx = 0.000000;
	dy = 0.000000;
	dz = 24.500000;	//cm, at reaction chamber edge
	TGeoCombiTrans* pCombTrans4 = new TGeoCombiTrans("", dx,dy,dz, zeroRot);
	TGeoCombiTrans* pCombTrans5 = new TGeoCombiTrans("", dx,dy,-dz, zeroRot);
	dx = 0.000000; 
	dy = -24.500000;
	dz = 0.000000;
	TGeoCombiTrans* pCombTrans6 = new TGeoCombiTrans("", dx,dy,dz, pRot2);
	//shape
	rmin = 0.0;
	rmax = 2.730000;
	dz = 1.000000;	//2cm thickness (length)
	phi1 = 0.0;
	phi2 = 360.0;
	TGeoShape *aTraMedium_BridgeChamberTube = new TGeoTubeSeg("aTraMedium_BridgeChamberTube",rmin,rmax,dz,phi1,phi2);
	TGeoVolume*  aTraMedium_BridgeChamberTubeLog = new TGeoVolume("aTraMedium_BridgeChamberTubeLog",aTraMedium_BridgeChamberTube, pMedFill);
	aTraMedium_BridgeChamberTubeLog->SetVisLeaves(kTRUE);
	aTraMedium_BridgeChamberTubeLog->SetLineColor(33);
	aTraMedium_ReactionChamberLog->AddNode(aTraMedium_BridgeChamberTubeLog, 0, pCombTrans4);
	aTraMedium_ReactionChamberLog->AddNode(aTraMedium_BridgeChamberTubeLog, 1, pCombTrans5);
	aTraMedium_ReactionChamberLog->AddNode(aTraMedium_BridgeChamberTubeLog, 2, pCombTrans6);

	//linear beam tube going downstream until the Aladin entrance window
	//placement
	dx = 0.000000;
	dy = 0.000000;
	dz = 85.600000;	//cm
	TGeoCombiTrans* pCombTrans7 = new TGeoCombiTrans("", dx,dy,dz, zeroRot);
	//shape (approximation as a cone segment, matching radii at XB_out and Aladin_in)
	rmin1 = 0.0;
	rmax1 = 5.125000;
	rmin2 = 0.0;
	rmax2 = 10.000000;
	dz = 31.900000;	//2cm thickness (length)
	phi1 = 0.0;
	phi2 = 360.0;
	TGeoShape *aTraMedium_BeamLineDownstream = new TGeoConeSeg("aTraMedium_BeamLineDownstream", dz,rmin1,rmax1,rmin2,rmax2,phi1,phi2);
	TGeoVolume* aTraMedium_BeamLineDownstreamLog = new TGeoVolume("aTraMedium_BeamLineDownstreamLog",aTraMedium_BeamLineDownstream, pMedFill);
	aTraMedium_BeamLineDownstreamLog->SetVisLeaves(kTRUE);
	aTraMedium_BeamLineDownstreamLog->SetLineColor(33);
	aTraMedium_ReactionChamberLog->AddNode(aTraMedium_BeamLineDownstreamLog, 0, pCombTrans7);








	/**********************************************************************/
	/***********  17.09.14 Lead shielding (j.isaak@gsi.de)  ***************/
	/**********************************************************************/

// the total shielding in s412 consisted of a box of
// 2mm Pb in the inside and 1mm Cu in the outside

//------------------------------------------------------------------

// make shape components


	rmin = 0.1;
	rmax = 18.0;
	dz   = 0.05;
	phi1 = 0.0;
	phi2 = 360.0;

	TGeoShape * outerCapRaw = new TGeoTubeSeg("outerCapRaw",rmin,rmax,dz,phi1,phi2);
	TGeoVolume * outerCapRawLog = new TGeoVolume("outerCapRawLog", outerCapRaw, pMedCu);


	rmin = 0.1;
	rmax = 17.85;
	dz   = 0.1;
	phi1 = 0.0;
	phi2 = 360.0;

	TGeoShape * innerCapRaw = new TGeoTubeSeg("innerCapRaw",rmin,rmax,dz,phi1,phi2);
	TGeoVolume * innerCapRawLog = new TGeoVolume("innerCapRawLog", innerCapRaw, pMedPb);



	rmin = 0.1;
	rmax = 18.0;
	dz   = 0.05;
	phi1 = 0.0;
	phi2 = 360.0;

	TGeoShape * outerBottomRaw = new TGeoTubeSeg("outerBottomRaw",rmin,rmax,dz,phi1,phi2);
	TGeoVolume * outerBottomRawLog = new TGeoVolume("outerBottomRawLog", outerBottomRaw, pMedCu);


	rmin = 0.1;
	rmax = 17.9;
	dz   = 0.1;
	phi1 = 0.0;
	phi2 = 360.0;

	TGeoShape * innerBottomRaw = new TGeoTubeSeg("innerBottomRaw",rmin,rmax,dz,phi1,phi2);
	TGeoVolume * innerBottomRawLog = new TGeoVolume("innerBottomRawLog", innerBottomRaw, pMedPb);


//------------------------------------------------------------------


	rmin = 17.9;
	rmax = 18.0;
	dz   = 10.5;
	phi1 = 0.0;
	phi2 = 360.0;

	TGeoShape * outerSurfaceRaw = new TGeoTubeSeg("outerSurfaceRaw",rmin,rmax,dz,phi1,phi2);
	TGeoVolume * outerSurfaceRawLog = new TGeoVolume("outerSurfaceRawLog", outerSurfaceRaw, pMedCu);


	rmin = 17.7;
	rmax = 17.9;
	dz   = 10.3;
	phi1 = 0.0;
	phi2 = 360.0;

	TGeoShape * innerSurfaceRaw = new TGeoTubeSeg("innerSurfaceRaw",rmin,rmax,dz,phi1,phi2);
	TGeoVolume * innerSurfaceRawLog = new TGeoVolume("innerSurfaceRawLog", innerSurfaceRaw, pMedPb);

// //------------------------------------------------------------------
// 
// 
// // part so subtract from bottom parts and surface (see technical drawing)

	TGeoShape * part1 = new TGeoBBox("part1", 0.7, 3.95, 0.1);
	TGeoShape * part2 = new TGeoBBox("part2", 0.3, 3.05, 0.1);

	TGeoShape * hole = new TGeoTubeSeg("hole", 0.0, 2.73, 30.0, 0.0, 360.0);
	TGeoVolume * holeLog = new TGeoVolume("holeLog", hole, pMedPb);


// //------------------------------------------------------------------
// 
// 
// // define some rotations and translations
// 
TGeoRotation *r1 = new TGeoRotation("r1",90,90,0);
r1->RegisterYourself();
TGeoCombiTrans *c1 = new TGeoCombiTrans("c1", 0.0,0.0,3.5,r1);
c1->RegisterYourself();



TGeoTranslation *t1 = new TGeoTranslation("t1", -1.6, -13.95, 0.0); 
t1->RegisterYourself();
TGeoTranslation *t2 = new TGeoTranslation("t2", 0.0, -6.25, 0.0); 
t2->RegisterYourself();

TGeoTranslation *trans1 = new TGeoTranslation("trans1", 0.0, 0.0, 0.15); 
trans1->RegisterYourself();

TGeoTranslation *trans2 = new TGeoTranslation("trans2", 0.0, 0.0, 10.55); 
trans2->RegisterYourself();

TGeoTranslation *trans3 = new TGeoTranslation("trans3", 0.0, 0.0, 10.55); 
trans3->RegisterYourself();

TGeoTranslation *trans4 = new TGeoTranslation("trans4", 0.0, 0.0, 20.95); 
trans4->RegisterYourself();

TGeoTranslation *trans5 = new TGeoTranslation("trans5", 0.0, 0.0, 21.1); 
trans5->RegisterYourself();
// 	//------------------------------------------------------------------
// 
// 
// // create bottom, cap and surface


//final/last rotation matrix:
//Combi transformation: 
	dx = 0.0000;
	dy = -14.0000;
	dz = 0.0000;
	//Rotation: 
	thx = 90.000000;    phx = 0.000000;
	thy = 90.000000;    phy = 90.000000;
	thz = 0.000000;    phz = 0.000000;	
	
	TGeoRotation *gRot = new TGeoRotation();
	gRot->RotateX(-90.0);
	gRot->RotateY(-90.0);
	gRot->RotateZ(0.0);
	
	TGeoRotation *	    pMatrix100 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
// 	TGeoCombiTrans*   pMatrix101 = new TGeoCombiTrans("", dx,dy,dz,pMatrix100);
	TGeoCombiTrans*   pMatrix101 = new TGeoCombiTrans("", dx,dy,dz,gRot);






TGeoCompositeShape * innerBottom = new TGeoCompositeShape("innerBottom", "(innerBottomRaw - part1:t1) - part2:t2");
TGeoVolume * innerBottomLog = new TGeoVolume("innerBottomLog", innerBottom, pMedPb);

TGeoCompositeShape *outerBottom = new TGeoCompositeShape("outerBottom","(outerBottomRaw-part1:t1)-part2:t2");
TGeoVolume *outerBottomLog = new TGeoVolume("outerBottomLog", outerBottom, pMedCu);

TGeoCompositeShape *outerSurface = new TGeoCompositeShape("outerSurface","outerSurfaceRaw-hole:c1");
TGeoVolume *outerSurfaceLog = new TGeoVolume("outerSurfaceLog",outerSurface, pMedCu);

TGeoCompositeShape *innerSurface = new TGeoCompositeShape("innerSurface","innerSurfaceRaw-hole:c1");
TGeoVolume *innerSurfaceLog = new TGeoVolume("innerSurfaceLog", innerSurface, pMedPb);

// // combine all parts together to have one single object/volume

TGeoCompositeShape *leadShielding = new TGeoCompositeShape("leadShielding","innerBottom:trans1+innerSurface:trans2+innerCapRaw:trans4");
TGeoVolume *leadShieldingLog = new TGeoVolume("leadShieldingLog", leadShielding, pMedPb);

TGeoCompositeShape *copperShielding = new TGeoCompositeShape("copperShielding","outerBottom+outerSurface:trans3+outerCapRaw:trans5");
TGeoVolume *copperShieldingLog = new TGeoVolume("copperShieldingLog", copperShielding, pMedCu);


TGeoCompositeShape *totalShielding = new TGeoCompositeShape("totalShielding","leadShielding+copperShielding");
// TGeoVolume *totalShieldingLog = new TGeoVolume("totalShieldingLog", totalShielding);
// 


	/**********************************************************************/
	/**********************************************************************/
	/**********************************************************************/







	/************ Assembling everything together ****************/
	TGeoVolume *aTra = new TGeoVolumeAssembly("ATRA");


	aTra->AddNode(pMontagePlatformLog,1,pMatrix14);
	aTra->AddNode(pMontageRingLog,1,pMatrix16);

	//aTra->AddNode(ptargetWheelLog,1,pMatrix28);
	//aTra->AddNode(ptargetWheel2Log,1,pMatrix30);

	aTra->AddNode(pTargetWheelLog,1,pMatrix28);

	aTra->AddNode(ptargetMotorLog,1,pMatrix02);
	aTra->AddNode(pMotorHolderLog1,1,pMatrix04);
	aTra->AddNode(pMotorHolderLog2,1,pMatrix06);


	// add the total shielding to the setup

// 	aTra->AddNode(totalShielding,1,pMatrix02);
	aTra->AddNode(leadShieldingLog,1,pMatrix101);
	aTra->AddNode(copperShieldingLog,1,pMatrix101);
	
// 	aTra->AddNode(totalShieldingLog,1,pMatrix02);


	//Adding  front end electronics


	/**********************************************************************/
	/*********  16.09.14 uncommented some parts (j.isaak@gsi.de)  *********/
	/**********************************************************************/

/*
	aTra->AddNode(vAlCaseLog,1,pMatrix18);
	aTra->AddNode(vAlCaseLog,2,pMatrix20);
	aTra->AddNode(vAlCaseLog,3,pMatrix22);
	aTra->AddNode(vAlCaseLog,4,pMatrix24);
	aTra->AddNode(vAlCaseLog,5,pMatrix26);
	aTra->AddNode(vAlCaseLog,6,pMatrix32);

	aTra->AddNode(pinnerElectronicsBoardLog,1,pMatrix18);
	aTra->AddNode(pinnerElectronicsBoardLog,2,pMatrix20);
	aTra->AddNode(pinnerElectronicsBoardLog,3,pMatrix22);
	aTra->AddNode(pinnerElectronicsBoardLog,4,pMatrix24);
	aTra->AddNode(pinnerElectronicsBoardLog,5,pMatrix26);
	aTra->AddNode(pinnerElectronicsBoardLog,6,pMatrix32);

	//Copper holders for the SSDs

	aTra->AddNode(pHolderBoxLog1,1,pMatrixHold1);
	aTra->AddNode(pHolderBoxLog1,2,pMatrixHold2);
	aTra->AddNode(pHolderBoxLog3,3,pMatrixHold3);
	aTra->AddNode(pHolderBoxLog4,4,pMatrixHold4);
	aTra->AddNode(pHolderBoxLog5,5,pMatrixHold5);
	aTra->AddNode(pHolderBoxLog6,6,pMatrixHold6);
	aTra->AddNode(pHolderBoxLog7,7,pMatrixHold7);
	aTra->AddNode(pHolderBoxLog7,8,pMatrixHold8);
	aTra->AddNode(pHolderBoxLog9,9,pMatrixHold9);
	aTra->AddNode(pHolderBoxLog10,10,pMatrixHold10);
	aTra->AddNode(pHolderBoxLog11,11,pMatrixHold11_1);
	aTra->AddNode(pHolderBoxLog12,12,pMatrixHold12);
	aTra->AddNode(pHolderSSDLog1,1,pMatrixSSD1);
	aTra->AddNode(pHolderSSDLog1,2,pMatrixSSD2);
	aTra->AddNode(pHolderSSDLog2,1,pMatrixSSD3);
	aTra->AddNode(pHolderSSDLog2,2,pMatrixSSD4);

	AddSensitiveVolume(TraLog);
	fNbOfSensitiveVol+=1;

	//Si sensors
	aTra->AddNode(TraLog,1, pMatrix4);
	aTra->AddNode(TraLog,2, pMatrix6);
	aTra->AddNode(TraLog,3, pMatrix8);
	aTra->AddNode(TraLog,4, pMatrix10);
	aTra->AddNode(TraLog,5, pMatrix12);
	aTra->AddNode(TraLog,6, pMatrix2);
*/



	TGeoRotation *rotg = new TGeoRotation();
	rotg->RotateX(0.);
	rotg->RotateY(0.);
	rotg->RotateZ(0.);
	dx=tx=0.0;
	dy=ty=0.0;
	dz=tz=0.0;

	TGeoCombiTrans *t_zero = new TGeoCombiTrans("t_zero");
	aTraMedium_ReactionChamberLog->AddNode(aTra,1, t_zero);
	
	TGeoCombiTrans *t0 = new TGeoCombiTrans(tx,ty,tz,rotg);
	//pWorld->AddNode(aTra,1, GetGlobalPosition(t0));
	pWorld->AddNode(aTraMedium_ReactionChamberLog,1, GetGlobalPosition(t0));

}

ClassImp(R3BTra)
