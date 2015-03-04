#ifndef xAODHelpers_WTaggedHistsAlgo_H
#define xAODHelpers_WTaggedHistsAlgo_H

#include <EventLoop/Algorithm.h>
#include <EventLoop/StatusCode.h>

// Infrastructure include(s):
#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"

class JetHists;

class WTaggedHistsAlgo : public EL::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:

  std::string m_name;
  std::string m_configName;
  std::string m_decorationName;

  xAOD::TEvent *m_event;  //!
  xAOD::TStore *m_store;  //!


private:
  JetHists* m_plots0W; //!
  JetHists* m_plots1W; //!
  JetHists* m_plots2W; //!
  JetHists* m_plots3W; //!
  JetHists* m_plots4W; //!

  // configuration variables
  std::string m_inContainerName;  //!
  std::string m_detailStr;        //!

  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)
public:
  // Tree *myTree; //!
  // TH1 *myHist; //!



  // this is a standard constructor
  WTaggedHistsAlgo ();
  WTaggedHistsAlgo (std::string name, std::string configName);

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

  // these are the functions not inherited from Algorithm
  virtual EL::StatusCode configure ();

  // this is needed to distribute the algorithm to the workers
  ClassDef(WTaggedHistsAlgo, 1);
};

#endif
