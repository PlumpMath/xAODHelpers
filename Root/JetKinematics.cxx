#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

#include <xAODHelpers/JetKinematics.h>
#include <xAODHelpers/JetHists.h>
#include <xAODHelpers/Helpers.h>

// count events
#include "xAODEventInfo/EventInfo.h"

#include "xAODAnaHelpers/tools/ReturnCheck.h"
#include "xAODAnaHelpers/HelperFunctions.h"

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
EL::StatusCode JetKinematics :: changeInput (bool /*firstFile*/) { return EL::StatusCode::SUCCESS; }

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
  const xAOD::EventInfo* eventInfo(nullptr);
  RETURN_CHECK("JetKinematics::execute()", HelperFunctions::retrieve( eventInfo, "EventInfo", m_event, 0, false), "");

  //----------------------------
  // Event weight
  //---------------------------
  float eventWeight(1);
  if( eventInfo->isAvailable< float >( "eventWeight" ) ){
    eventWeight = eventInfo->auxdecor< float >( "eventWeight" );
  }

  const xAOD::JetContainer* jets(nullptr);
  RETURN_CHECK("JetKinematics::execute()", HelperFunctions::retrieve( jets, m_jetContainerName, m_event, m_store, false), "");

  m_plots->execute(jets, eventWeight);


  // check the reclustering
  const xAOD::JetContainer* smallRjets(nullptr);
  RETURN_CHECK("JetKinematics::execute()", HelperFunctions::retrieve( smallRjets, "AntiKt4LCTopoJets", m_event, m_store, false), "");

  const xAOD::JetContainer* largeRjets(nullptr);
  RETURN_CHECK("JetKinematics::execute()", HelperFunctions::retrieve( largeRjets, "AntiKt10LCTopoJets", m_event, m_store, false), "");

  xAODHelpers::Helpers helpers;
  xAOD::JetContainer* reclusteredJets = new xAOD::JetContainer();
  xAOD::JetAuxContainer* reclusteredJetsAux = new xAOD::JetAuxContainer;
  reclusteredJets->setStore(reclusteredJetsAux);

  // recluster 0.4 jets into 1.0 jets
  helpers.jet_reclustering(*reclusteredJets, smallRjets);

  std::string printStr = "\tPt: %0.2f\tMass: %0.2f\tEta: %0.2f\tPhi: %0.2f\n";

  std::cout << smallRjets->size() << " small-R jets" << std::endl;
  for(const auto jet: *smallRjets)
    printf(printStr.c_str(), jet->pt()/1000., jet->m()/1000., jet->eta(), jet->phi());

  std::cout << largeRjets->size() << " large-R jets" << std::endl;
  for(const auto jet: *largeRjets)
    printf(printStr.c_str(), jet->pt()/1000., jet->m()/1000., jet->eta(), jet->phi());

  std::cout << reclusteredJets->size() << " reclustered jets" << std::endl;
  for(const auto jet: *reclusteredJets)
    printf(printStr.c_str(), jet->pt()/1000., jet->m()/1000., jet->eta(), jet->phi());

  delete reclusteredJets;
  delete reclusteredJetsAux;
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
