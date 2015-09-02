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
  RETURN_CHECK("JetReclustering::initialize()", m_jetReclusteringTool->setProperty("OutputJetContainer", "RCJets"), "");
  RETURN_CHECK("JetReclustering::initialize()", m_jetReclusteringTool->initialize(), "");

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode JetReclustering :: execute ()
{
  const xAOD::EventInfo* eventInfo(nullptr);
  RETURN_CHECK("JetReclustering::execute()", HelperFunctions::retrieve( eventInfo, "EventInfo", m_event, 0, false), "Could not retrieve EventInfo object.");

  m_jetReclusteringTool->execute();

  // grab the rc jets
  const xAOD::JetContainer* rc_jets(nullptr);
  RETURN_CHECK("JetReclustering::execute()", HelperFunctions::retrieve( rc_jets, "RCJets", m_event, m_store, false), "Could not retrieve RCJets object.");

  Info("execute()", "\r\n\r\n\r\n");
  for(auto jet: *rc_jets){
    const xAOD::Jet* subjet(nullptr);
    const xAOD::BTagging* btag(nullptr);
    for(auto constit: jet->getConstituents()){
      subjet = static_cast<const xAOD::Jet*>(constit->rawConstituent());
      btag = subjet->btagging();
      if(btag)
        Info("execute()", "btagging: %0.6f", btag->MV1_discriminant());

      Info("execute()", "\tsubjet pt: %0.6f", subjet->pt());
    }
  }


  return EL::StatusCode::SUCCESS;
}

EL::StatusCode JetReclustering :: postExecute () { return EL::StatusCode::SUCCESS; }
EL::StatusCode JetReclustering :: finalize () {
  delete m_jetReclusteringTool;
  return EL::StatusCode::SUCCESS;
}
EL::StatusCode JetReclustering :: histFinalize () { return EL::StatusCode::SUCCESS; }
