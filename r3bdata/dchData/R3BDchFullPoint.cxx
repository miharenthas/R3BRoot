// -------------------------------------------------------------------------
// -----                      R3BDchFullPoint source file                  -----
// -------------------------------------------------------------------------

#include "R3BDchFullPoint.h"

#include <iostream>

using std::cout;
using std::endl;
using std::flush;


// -----   Default constructor   -------------------------------------------
R3BDchFullPoint::R3BDchFullPoint() : FairMCPoint() {
  fX      = fY  = fZ  = 0.;
  fPx     = fPy = fPz = 0.;
  fLocalX      = fLocalY  = fLocalZ  = 0.;
  fLocalPx     = fLocalPy = fLocalPz = 0.;
  fModule      = -1;
  fLayer       = -1;
  fCell        = -1;
}
// -------------------------------------------------------------------------



// -----   Standard constructor   ------------------------------------------
R3BDchFullPoint::R3BDchFullPoint(Int_t trackId, Int_t mod, Int_t layer, Int_t cell, TVector3 pos,
			         TVector3 lpos, TVector3 mom, TVector3 lmom,
			         Double_t tof, Double_t length, Double_t eLoss)
  : FairMCPoint(trackId, mod, pos, mom, tof, length, eLoss) {
  fLocalX  = lpos.X();
  fLocalY  = lpos.Y();
  fLocalZ  = lpos.Z();
  fLocalPx = lmom.Px();
  fLocalPy = lmom.Py();
  fLocalPz = lmom.Pz();
  fModule  = mod;
  fLayer   = layer;
  fCell    = cell;
}
// -------------------------------------------------------------------------



// -----   Destructor   ----------------------------------------------------
R3BDchFullPoint::~R3BDchFullPoint() { }
// -------------------------------------------------------------------------




// -----   Public method Print   -------------------------------------------
void R3BDchFullPoint::Print(const Option_t* opt) const {
  cout << "-I- R3BDchFullPoint: STS Point for track " << fTrackID 
       << " in detector " << fDetectorID << endl;
  cout << "    Position (" << fX << ", " << fY << ", " << fZ
       << ") cm" << endl;
  cout << "    Momentum (" << fPx << ", " << fPy << ", " << fPz
       << ") GeV" << endl;
  cout << "    Time " << fTime << " ns,  Length " << fLength 
       << " cm,  Energy loss " << fELoss*1.0e06 << " keV" << endl;
}
// -------------------------------------------------------------------------





// -----   Public method IsUsable   ----------------------------------------
Bool_t R3BDchFullPoint::IsUsable() const {
return kTRUE;
}
// -------------------------------------------------------------------------


ClassImp(R3BDchFullPoint)
