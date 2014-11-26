#include <UCHelpers/JetHists.h>

// subjet finding
#include "JetSubStructureUtils/SubjetFinder.h"
// deal with the btagging
#include "xAODBTagging/BTagging.h"

JetHists::JetHists() {}
JetHists::~JetHists() {}

void JetHists::book(bool sumw2) {
  h_jetPt           = book("jetPt", "Transverse Momentum", "{p}_{t} [GeV]", 100, 0, 500, sumw2);
  h_jetM            = book("jetM" , "Mass", "{m} [GeV]", 100, 0, 500, sumw2);
  h_jetEta          = book("jetEta", "Eta", "\\eta", 100, -4.9, 4.9, sumw2);
  h_jetPhi          = book("jetPhi", "Phi", "\\phi", 100, -3.2, 3.2, sumw2);

  h_numJets         = book("numJets", "Num. Jets per Event", "# jets", 100, 0, 20, sumw2);
  h_numSubjets      = book("numSubjets", "Num. Subjets per Event", "# subjets", 100, 0, 200, sumw2);
  h_numbtags        = book("num_btags", "JetFitterCOMBNN_p > 0.5", "JetFitterCOMBNN_p > 0.5", 6, 0.5, 5.5, sumw2);

  h_jetTau1         = book("jetTau1", "Tau1", "{\\tau}_{1}", 100, 0, 1, sumw2);
  h_jetTau2         = book("jetTau2", "Tau2", "{\\tau}_{2}", 100, 0, 1, sumw2);
  h_jetTau3         = book("jetTau3", "Tau3", "{\\tau}_{3}", 100, 0, 1, sumw2);
  h_jetDip12        = book("jetDip12", "Dip12", "{dip}_{12}", 100, 0, 1, sumw2);
  h_jetDip13        = book("jetDip13", "Dip13", "{dip}_{13}", 100, 0, 1, sumw2);
  h_jetDip23        = book("jetDip23", "Dip23", "{dip}_{23}", 100, 0, 1, sumw2);
  h_jet_numSubjets  = book("jet_numSubjets", "Num Subjets per Jet", "# subjets", 100, 0, 10, sumw2);
  h_jet_btags_p     = book("btags_p", "JetFitterCOMBNN (antiKt Topo LC jets)", "JetFitterCOMBNN (antiKt Topo LC jets)", 100, 0, 1, sumw2);
}

EL::StatusCode JetHists::fill() {
  // get jet container of interest
  const xAOD::JetContainer* jets = 0;
  if ( !m_wk->xaodEvent()->retrieve( jets, m_containerName.c_str() ).isSuccess() ){ // retrieve arguments: container type, contain        er key
    Error("execute()", "Failed to retrieve %s. Exiting.", m_containerName.c_str() );
    return EL::StatusCode::FAILURE;
  }

  // Info("execute()", "  number of %s = %lu", m_jetContainerName.c_str(), jets->size());

  xAOD::JetContainer::const_iterator jet_itr = jets->begin();
  xAOD::JetContainer::const_iterator jet_end = jets->end();

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

  h_numJets->Fill( jets->size() );
  h_numSubjets->Fill( numSubjets );
  h_numbtags->Fill( numBTags+0.5 ); // offset it

  return EL::StatusCode::SUCCESS;

}
