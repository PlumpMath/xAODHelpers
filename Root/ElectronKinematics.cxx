#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <EventLoop/Job.h>

#include <xAODHelpers/ElectronKinematics.h>
#include <xAODHelpers/ElectronHists.h>

// count events
#include "xAODEventInfo/EventInfo.h"

// this is needed to distribute the algorithm to the workers
ClassImp(ElectronKinematics)

ElectronKinematics :: ElectronKinematics ()
{
  m_electronContainerName = "ElectronCollection";
  m_electronDisplayName = "ElectronCollection";
}



EL::StatusCode ElectronKinematics :: setupJob (EL::Job& job)
{
  job.useXAOD();
  xAOD::Init(("ElectronKinematics_"+m_electronDisplayName).c_str()).ignore(); // call before opening first file
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode ElectronKinematics :: histInitialize ()
{
  m_histManager = new ElectronHists();
  m_histManager->assign(wk());
  m_histManager->m_containerName = m_electronContainerName;
  m_histManager->m_namePostfix = m_electronDisplayName;

  Info("histInitialize()", "%s", m_electronContainerName.c_str());

  // initialize with sumw2=true
  m_histManager->initialize();

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode ElectronKinematics :: fileExecute ()
{
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode ElectronKinematics :: changeInput (bool firstFile)
{
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode ElectronKinematics :: initialize ()
{
  m_event = wk()->xaodEvent();
  m_eventCounter = 0;
  m_numEvents = m_event->getEntries();

  Info("initialize()", "Number of events = %lli", m_numEvents );

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode ElectronKinematics :: execute ()
{

  // print every 500 events, so we know where we are:
  if( (m_eventCounter % 500) ==0 ) Info("execute()", "Event number = %i", m_eventCounter );
  m_eventCounter++;

  //----------------------------
  // Event information
  //--------------------------- 
  const xAOD::EventInfo* eventInfo = 0;
  if( ! m_event->retrieve( eventInfo, "EventInfo").isSuccess() ){
    Error("execute()", "Failed to retrieve event info collection. Exiting." );
    return EL::StatusCode::FAILURE;
  }

  m_histManager->execute();

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode ElectronKinematics :: postExecute ()
{
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode ElectronKinematics :: finalize ()
{
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode ElectronKinematics :: histFinalize ()
{
  m_histManager->finalize();
  Info("finalize()", "%d/%lli events", m_eventCounter, m_numEvents);
  if(m_histManager){
    delete m_histManager;
    m_histManager = 0;
  }
  return EL::StatusCode::SUCCESS;
}
