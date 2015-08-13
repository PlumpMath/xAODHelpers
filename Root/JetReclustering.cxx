#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

#include <xAODHelpers/JetReclustering.h>

#include "xAODJet/JetContainer.h"
#include "xAODJet/JetAuxContainer.h"

#include "xAODEventInfo/EventInfo.h"

#include "xAODAnaHelpers/tools/ReturnCheck.h"
#include "xAODAnaHelpers/HelperFunctions.h"

#include <xAODJetReclustering/JetReclusteringTool.h>

// this is needed to distribute the algorithm to the workers
ClassImp(JetReclustering)

JetReclustering :: JetReclustering () {}

EL::StatusCode JetReclustering :: setupJob (EL::Job& job)
{
  job.useXAOD();
  xAOD::Init("JetReclustering").ignore(); // call before opening first file

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode JetReclustering :: histInitialize () { return EL::StatusCode::SUCCESS; }
EL::StatusCode JetReclustering :: fileExecute () { return EL::StatusCode::SUCCESS; }
EL::StatusCode JetReclustering :: changeInput (bool /*firstFile*/) { return EL::StatusCode::SUCCESS; }

EL::StatusCode JetReclustering :: initialize ()
{
  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  m_jetReclusteringTool = new JetReclusteringTool("Fakey");
  RETURN_CHECK("JetReclustering::initialize()", m_jetReclusteringTool->setProperty("InputJetContainer", "AntiKt4LCTopoJets"), "");
  //RETURN_CHECK("JetReclustering::initialize()", m_jetReclusteringTool->setProperty("InputJetContainer",  "InputJetsForReclustering"), "");
  RETURN_CHECK("JetReclustering::initialize()", m_jetReclusteringTool->setProperty("OutputJetContainer", "OutputJetsAfter"), "");
  RETURN_CHECK("JetReclustering::initialize()", m_jetReclusteringTool->initialize(), "");

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode JetReclustering :: execute ()
{
  const xAOD::EventInfo* eventInfo(nullptr);
  RETURN_CHECK("JetReclustering::execute()", HelperFunctions::retrieve( eventInfo, "EventInfo", m_event, 0, false), "Could not retrieve EventInfo object.");

  // grab the small radius jets
  const xAOD::JetContainer* smallRjets(nullptr);
  RETURN_CHECK("JetReclustering::execute()", HelperFunctions::retrieve( smallRjets, "AntiKt4LCTopoJets", m_event, m_store, false), "Could not retrieve smallRjet object.");

  const xAOD::JetContainer* largeRjets(nullptr);
  RETURN_CHECK("JetReclustering::execute()", HelperFunctions::retrieve( largeRjets, "AntiKt10LCTopoTrimmedPtFrac5SmallR20Jets", m_event, m_store, false), "Could not retrieve largeRjet object.");

  /*
  xAOD::JetContainer* jets(new xAOD::JetContainer);
  xAOD::JetAuxContainer* jets_aux(new xAOD::JetAuxContainer);
  jets->setStore(jets_aux);

  for(const auto jet: *largeRjets){
    for(const auto constit: jet->getConstituents()){
      xAOD::Jet* jet_new(new xAOD::Jet(constit));
      jets->push_back(jet_new);
    }
  }

  for(const auto jet: *smallRjets){
    xAOD::Jet* jet_new(new xAOD::Jet(*jet));
    jets->push_back(jet_new);
  }

  static std::string containerName = "InputJetsForReclustering";
  RETURN_CHECK("JetReclustering::execute()", m_store->record(jets, containerName), "Can't record container");
  RETURN_CHECK("JetReclustering::execute()", m_store->record(jets_aux, containerName+"Aux."), "Can't record aux container");
  */

  m_jetReclusteringTool->execute();

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode JetReclustering :: postExecute () { return EL::StatusCode::SUCCESS; }
EL::StatusCode JetReclustering :: finalize () {
  delete m_jetReclusteringTool;
  return EL::StatusCode::SUCCESS;
}
EL::StatusCode JetReclustering :: histFinalize () { return EL::StatusCode::SUCCESS; }
