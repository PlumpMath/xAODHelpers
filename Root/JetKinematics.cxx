#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

#include <UCHelpers/JetKinematics.h>

// count events
#include "xAODEventInfo/EventInfo.h"
// Jets
#include "xAODJet/JetContainer.h"

// subjet finding
#include "JetSubStructureUtils/SubjetFinder.h"

// manage histograms for us
#include "UCHelpers/HistogramManager.h"

#include "xAODBTagging/BTagging.h"

// this is needed to distribute the algorithm to the workers
ClassImp(JetKinematics)

JetKinematics :: JetKinematics ()
{
  m_jetContainerName = "AntiKt10LCTopoJets";
  m_jetDisplayName = "AntiKt10LCTopoJets";
}



EL::StatusCode JetKinematics :: setupJob (EL::Job& job)
{
  job.useXAOD();
  xAOD::Init(("JetKinematics_"+m_jetDisplayName).c_str()).ignore(); // call before opening first file
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetKinematics :: histInitialize ()
{
  HistogramManager* hists = new HistogramManager(wk());
  hists->namePostfix = m_jetDisplayName;

  Info("histInitialize()", "%s", m_jetContainerName.c_str());

  h_jetPt           = hists->book("jetPt", "Transverse Momentum", "{p}_{t} [GeV]", 100, 0, 500);
  h_jetM            = hists->book("jetM" , "Mass", "{m} [GeV]", 100, 0, 500);
  h_jetEta          = hists->book("jetEta", "Eta", "\\eta", 100, -4.9, 4.9);
  h_jetPhi          = hists->book("jetPhi", "Phi", "\\phi", 100, -3.2, 3.2);

  h_numJets         = hists->book("numJets", "Num. Jets per Event", "# jets", 100, 0, 50);
  h_numSubjets      = hists->book("numSubjets", "Num. Subjets per Event", "# subjets", 100, 0, 100);
  h_numbtags        = hists->book("num_btags", "JetFitterCOMBNN_p > 0.5", "JetFitterCOMBNN_p > 0.5", 6, 0.5, 5.5);


  h_jetTau1         = hists->book("jetTau1", "Tau1", "{\\tau}_{1}", 100, 0, 1);
  h_jetTau2         = hists->book("jetTau2", "Tau2", "{\\tau}_{2}", 100, 0, 1);
  h_jetTau3         = hists->book("jetTau3", "Tau3", "{\\tau}_{3}", 100, 0, 1);
  h_jetDip12        = hists->book("jetDip12", "Dip12", "{dip}_{12}", 100, 0, 1);
  h_jetDip13        = hists->book("jetDip13", "Dip13", "{dip}_{13}", 100, 0, 1);
  h_jetDip23        = hists->book("jetDip23", "Dip23", "{dip}_{23}", 100, 0, 1);
  h_jet_numSubjets  = hists->book("jet_numSubjets", "Num Subjets per Jet", "# subjets", 100, 0, 20);
  h_jet_btags_p     = hists->book("btags_p", "JetFitterCOMBNN (antiKt Topo LC jets)", "JetFitterCOMBNN (antiKt Topo LC jets)", 100, -3.550, 3.250);

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetKinematics :: fileExecute ()
{
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetKinematics :: changeInput (bool firstFile)
{
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetKinematics :: initialize ()
{
  m_event = wk()->xaodEvent();
  m_eventCounter = 0;
  m_jetCounter = 0;
  m_numEvents = m_event->getEntries();

  Info("initialize()", "Number of events = %lli", m_numEvents );

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
 
  // get jet container of interest
  const xAOD::JetContainer* jetContainer = 0;
  if ( !m_event->retrieve( jetContainer, m_jetContainerName.c_str() ).isSuccess() ){ // retrieve arguments: container type, contain    er key
    Error("execute()", "Failed to retrieve AntiKt10LCTopoJets container. Exiting." );
    return EL::StatusCode::FAILURE;
  }
 
  // Info("execute()", "  number of %s = %lu", m_jetContainerName.c_str(), jetContainer->size());
 
  xAOD::JetContainer::const_iterator jet_itr = jetContainer->begin();
  xAOD::JetContainer::const_iterator jet_end = jetContainer->end();
 
  JetSubStructureUtils::SubjetFinder subjetFinder;
  std::vector<fastjet::PseudoJet> subjets;

  long int numSubjets = 0;
  long int numBTags = 0;
  // loop over the jets
  for( ; jet_itr != jet_end; ++jet_itr ) {
    // basic kinematics
    h_jetPt->Fill( (*jet_itr)->pt()*0.001 );
    h_jetM->Fill( (*jet_itr)->m()*0.001 );
    h_jetEta->Fill( (*jet_itr)->eta() );
    h_jetPhi->Fill( (*jet_itr)->phi() );

    // substructure
    if( (*jet_itr)->isAvailable<float>("Tau1") ){
      h_jetTau1->Fill( (*jet_itr)->getAttribute<float>("Tau1") );
      h_jetTau2->Fill( (*jet_itr)->getAttribute<float>("Tau2") );
      h_jetTau3->Fill( (*jet_itr)->getAttribute<float>("Tau3") );
    }

    if( (*jet_itr)->isAvailable<float>("Dip12") ){
      h_jetDip12->Fill( (*jet_itr)->getAttribute<float>("Dip12")*0.000001 );
      h_jetDip13->Fill( (*jet_itr)->getAttribute<float>("Dip13")*0.000001 );
      h_jetDip23->Fill( (*jet_itr)->getAttribute<float>("Dip23")*0.000001 );
    }

    /* Miles: R=0.3 kT >5 GeV subjets */
    subjets = subjetFinder.result(**jet_itr);
    numSubjets+= subjets.size();

    h_jet_numSubjets->Fill( subjets.size() );
    m_jetCounter++;


    /*
    // btagging
    //typedef std::vector< ElementLink< xAOD::BTagging> > btaggingLink;
    //typedef ElementLink< xAOD::BTagging> btaggingLink;
    typedef ElementLink< DataVector< xAOD::BTagging >> btagLink_t;
    if( (*jet_itr)->isAvailable<btagLink_t>("btaggingLink")){
      Info("btaggingAvailable", "found it?");
      const btagLink_t & btaggingLink = (*jet_itr)->getAttribute<btagLink_t>("btaggingLink");
      if( btaggingLink.isValid() ){
        Info("tagging", "%lu", (*btaggingLink)->nSV1_TrackParticles());
        h_jet_numbtags->Fill( (*btaggingLink)->nSV0_TrackParticles() );
        numBTags += (*btaggingLink)->nSV1_TrackParticles();
      } else {
        Info("tagging", "not valid");
      }
    }
    */

    // http://acode-browser.usatlas.bnl.gov/lxr/source/atlas/Event/xAOD/xAODBTagging/xAODBTagging/versions/BTagging_v1.h
    const xAOD::BTagging* btag = (*jet_itr)->btagging();
    if(btag){
      const double prob = btag->JetFitterCombNN_pb();
      h_jet_btags_p->Fill( prob );
      if(prob > 0.5){
        numBTags += 1;
      }
    } else {
      Info("execute()", "Could not get the btagging informationa");
    }

  } 

  h_numJets->Fill( jetContainer->size() );
  h_numSubjets->Fill( numSubjets );
  h_numbtags->Fill( numBTags+0.5 ); // offset it

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetKinematics :: postExecute ()
{
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetKinematics :: finalize ()
{
  Info("finalize()", "%d/%lli events, %d jets", m_eventCounter, m_numEvents, m_jetCounter);
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode JetKinematics :: histFinalize ()
{
  return EL::StatusCode::SUCCESS;
}
