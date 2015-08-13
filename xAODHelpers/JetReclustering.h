#ifndef xAODHelpers_JetReclustering_H
#define xAODHelpers_JetReclustering_H

#include <EventLoop/Algorithm.h>

#include <xAODRootAccess/Init.h>
#include <xAODRootAccess/TEvent.h>
#include <xAODRootAccess/TStore.h>

class JetReclusteringTool;

class JetReclustering : public EL::Algorithm
{
private:
  /* For counting and statistics */
  xAOD::TEvent *m_event; //!
  xAOD::TStore *m_store; //!

 JetReclusteringTool*  m_jetReclusteringTool; //!

public:
  // this is a standard constructor
  JetReclustering ();

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
  ClassDef(JetReclustering, 1);
};

#endif
