#ifndef UCHelpers_ElectronKinematics_H
#define UCHelpers_ElectronKinematics_H

#include <EventLoop/Algorithm.h>

#include <xAODRootAccess/Init.h>
#include <xAODRootAccess/TEvent.h>

// for histograms
#include <TH1.h>

class ElectronHists;

class ElectronKinematics : public EL::Algorithm
{
public:

  /* For counting and statistics */
  xAOD::TEvent *m_event; //!
  int m_eventCounter; //!
  long long int m_numEvents; //!

  #ifndef __CINT__
    // for holding the set of histogram codes
    ElectronHists* m_histManager; //!
  #endif // not __CINT__

  /* For retrieving the correct electron object */
  std::string m_electronContainerName;
  std::string m_electronDisplayName;

  // this is a standard constructor
  ElectronKinematics ();

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
  ClassDef(ElectronKinematics, 1);
};

#endif
