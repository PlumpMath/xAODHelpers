#ifndef xAODHelpers_JetKinematics_H
#define xAODHelpers_JetKinematics_H

#include <EventLoop/Algorithm.h>

#include <xAODRootAccess/Init.h>
#include <xAODRootAccess/TEvent.h>
#include <xAODRootAccess/TStore.h>

// for histograms
#include <TH1.h>

class JetHists;

class JetKinematics : public EL::Algorithm
{

private:
    // for holding the set of histogram codes
    JetHists* m_plots; //!

public:
  /* For counting and statistics */
  xAOD::TEvent *m_event; //!
  xAOD::TStore *m_store; //!
  int m_eventCounter; //!

  /* For retrieving the correct jet object */
  std::string m_jetContainerName;
  std::string m_jetDisplayName;
  std::string m_jetDetailStr;

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
