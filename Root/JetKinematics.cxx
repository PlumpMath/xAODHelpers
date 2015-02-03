#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

#include <xAODHelpers/JetKinematics.h>
#include <xAODHelpers/JetHists.h>

// count events
#include "xAODEventInfo/EventInfo.h"

// this is needed to distribute the algorithm to the workers
ClassImp(JetKinematics)

JetKinematics :: JetKinematics () {}

EL::StatusCode JetKinematics :: setupJob (EL::Job& job)
{
  job.useXAOD();
  xAOD::Init(("JetKinematics_"+m_jetDisplayName).c_str()).ignore(); // call before opening first file
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetKinematics :: histInitialize ()
{
  Info("histInitialize()", "%s", m_jetContainerName.c_str());
  m_plots = new JetHists(m_jetDisplayName, m_jetDetailStr);
  m_plots->initialize();
  m_plots->record( wk() );
  return EL::StatusCode::SUCCESS;
}


EL::StatusCode JetKinematics :: fileExecute () { return EL::StatusCode::SUCCESS; }
EL::StatusCode JetKinematics :: changeInput (bool firstFile) { return EL::StatusCode::SUCCESS; }

EL::StatusCode JetKinematics :: initialize ()
{
  Info("initialize()", "JetKinematics_%s", m_jetContainerName.c_str() );
  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();
  m_eventCounter = 0;
  return EL::StatusCode::SUCCESS;
}


EL::StatusCode JetKinematics :: execute ()
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

  //----------------------------
  // Event weight
  //--------------------------- 
  float eventWeight(1);
  if( eventInfo->isAvailable< float >( "eventWeight" ) ){
    eventWeight = eventInfo->auxdecor< float >( "eventWeight" );
  }

  const xAOD::JetContainer* jets = 0;
  if ( !m_event->retrieve( jets, m_jetContainerName ).isSuccess() ){
    if ( !m_store->retrieve( jets, m_jetContainerName ).isSuccess() ){
      Error("execute()  ", "Failed to retrieve %s container. Exiting.", m_jetContainerName.c_str() );
      return EL::StatusCode::FAILURE;
    }
  }

  m_plots->execute(jets, eventWeight);

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode JetKinematics :: postExecute () { return EL::StatusCode::SUCCESS; }
EL::StatusCode JetKinematics :: finalize () { return EL::StatusCode::SUCCESS; }

EL::StatusCode JetKinematics :: histFinalize ()
{
  if(m_plots){
    delete m_plots;
    m_plots = 0;
  }
  return EL::StatusCode::SUCCESS;
}
