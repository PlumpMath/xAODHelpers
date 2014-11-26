#ifndef UCHelpers_JetKinematics_H
#define UCHelpers_JetKinematics_H

#include <EventLoop/Algorithm.h>

#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"

// ElementLink
#include "AthLinks/ElementLink.h"


// for histograms
#include <TH1.h>

class JetKinematics : public EL::Algorithm
{
public:

  /* For counting and statistics */
  xAOD::TEvent *m_event; //!
  int m_eventCounter; //!
  int m_jetCounter; //!
  long long int m_numEvents; //!

  /* For retrieving the correct jet object */
  std::string m_jetContainerName;
  std::string m_jetDisplayName;

  // basics
  TH1 *h_jetPt; //!
  TH1 *h_jetM; //!
  TH1 *h_jetEta; //!
  TH1 *h_jetPhi; //!

  // topological (eg: per event)
  TH1 *h_numJets; //!
  TH1 *h_numSubjets; //!
  TH1 *h_numbtags; //!
  // scattering angle of two highest pt jets in CoM frame
  // number of top tags
  // number of W tags
  // angular separate between two highest pt top-tagged jets
  // relative mass difference between two highest pt top-tagged jets

  // substructure
  TH1 *h_jetTau1; //!
  TH1 *h_jetTau2; //!
  TH1 *h_jetTau3; //!
  TH1 *h_jetDip12; //!
  TH1 *h_jetDip13; //!
  TH1 *h_jetDip23; //!
  TH1 *h_jet_numSubjets; //!

  TH1 *h_jet_btags_p; //!

  // this is a standard constructor
  JetKinematics ();

  // these are the functions inherited from Algorithm
  virtual EL::StatusCode setupJob (EL::Job& job);
  virtual EL::StatusCode fileExecute ();
  virtual EL::StatusCode histInitialize ();
  virtual EL::StatusCode changeInput (bool firstFile);
  virtual EL::StatusCode initialize ();
  virtual EL::StatusCode execute ();
  virtual EL::StatusCode postExecute ();
  virtual EL::StatusCode finalize ();
  virtual EL::StatusCode histFinalize ();

  // this is needed to distribute the algorithm to the workers
  ClassDef(JetKinematics, 1);
};

#endif
