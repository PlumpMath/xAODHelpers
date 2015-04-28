#include <xAODHelpers/JetHists.h>

// btagging
#include "xAODHelpers/Helpers.h"
// subjet finding
#include "JetSubStructureUtils/SubjetFinder.h"

JetHists::JetHists(std::string name, std::string detailStr):
  HistogramManager(name, detailStr)
{
}

JetHists::~JetHists() {}

StatusCode JetHists::initialize() {
  h_jetPt           = book(m_name, "jetPt", "{p}_{t} [GeV]", 100, 0, 500);
  h_jetM            = book(m_name, "jetM", "{m} [GeV]", 100, 0, 500);
  h_jetEta          = book(m_name, "jetEta", "\\eta", 100, -4.9, 4.9);
  h_jetPhi          = book(m_name, "jetPhi", "\\phi", 100, -3.2, 3.2);

  h_trimmed_jetPt   = book(m_name, "trimmed_jetPt", "{p}_{t} [GeV]", 100, 0, 500);
  h_trimmed_jetM    = book(m_name, "trimmed_jetM", "{m} [GeV]", 100, 0, 500);

  h_rc_jetPt        = book(m_name, "rc_jetPt", "p_t [GeV]", 100, 0, 500);
  h_rc_jetM         = book(m_name, "rc_jetM", "m [GeV]", 100, 0, 500);
  h_numrcJets       = book(m_name, "event_numrcJets", "# rc jets", 100, 0, 20);

  h_numJets         = book(m_name, "event_numJets", "# jets", 100, 0, 20);
  h_numSubjets      = book(m_name, "event_numSubjets", "# subjets", 100, 0, 200);

  h_jetTau1         = book(m_name, "jetTau1", "{\\tau}_{1}", 100, 0, 1);
  h_jetTau2         = book(m_name, "jetTau2", "{\\tau}_{2}", 100, 0, 1);
  h_jetTau3         = book(m_name, "jetTau3", "{\\tau}_{3}", 100, 0, 1);
  h_jetDip12        = book(m_name, "jetDip12", "{dip}_{12}", 100, 0, 1);
  h_jetDip13        = book(m_name, "jetDip13", "{dip}_{13}", 100, 0, 1);
  h_jetDip23        = book(m_name, "jetDip23", "{dip}_{23}", 100, 0, 1);
  h_jet_numSubjets  = book(m_name, "jet_numSubjets", "# subjets", 100, 0, 10);

  h_mv1_discriminant = book(m_name, "mv1_discriminant", "MV1 Discriminant", 200, 0, 1);
  /*
  h_num_bTags        = book("num_bTags", m_containerName, "Number of B-Tags per Jet", 10, -0.5, 9.5);
  h_num_bTags_withTruth = book("num_bTags_withTruth", m_containerName, "Number of Truth Bs per Jet", 10, -0.5, 9.5);
  */


  return StatusCode::SUCCESS;
}

StatusCode JetHists::execute(const xAOD::JetContainer* jets, float eventWeight) {
  xAOD::JetContainer::const_iterator jet_itr = jets->begin();
  xAOD::JetContainer::const_iterator jet_end = jets->end();

  //JetSubStructureUtils::SubjetFinder subjetFinder;
  JetSubStructureUtils::SubjetFinder subjetFinder(fastjet::kt_algorithm, 0.3, 0.0);
  std::vector<fastjet::PseudoJet> subjets;

  // for btagging
  xAODHelpers::Helpers helpers;

  long int numSubjets = 0;
  // loop over the jets
  for( ; jet_itr != jet_end; ++jet_itr ) {
    // basic kinematics
    h_jetPt->Fill( (*jet_itr)->pt()*0.001, eventWeight );
    h_jetM->Fill( (*jet_itr)->m()*0.001, eventWeight );
    h_jetEta->Fill( (*jet_itr)->eta(), eventWeight );
    h_jetPhi->Fill( (*jet_itr)->phi(), eventWeight );

    // substructure
    if( (*jet_itr)->isAvailable<float>("Tau1") ){
      h_jetTau1->Fill( (*jet_itr)->getAttribute<float>("Tau1"), eventWeight );
      h_jetTau2->Fill( (*jet_itr)->getAttribute<float>("Tau2"), eventWeight );
      h_jetTau3->Fill( (*jet_itr)->getAttribute<float>("Tau3"), eventWeight );
    }

    if( (*jet_itr)->isAvailable<float>("Dip12") ){
      h_jetDip12->Fill( (*jet_itr)->getAttribute<float>("Dip12"), eventWeight );
      h_jetDip13->Fill( (*jet_itr)->getAttribute<float>("Dip13"), eventWeight );
      h_jetDip23->Fill( (*jet_itr)->getAttribute<float>("Dip23"), eventWeight );
    }

    /* Miles: R=0.3 kT >0 GeV subjets */
    subjets = subjetFinder.result(**jet_itr);
    numSubjets+= subjets.size();

    TLorentzVector trimmedJet = helpers.jet_trimming(*jet_itr);

    h_trimmed_jetPt->Fill( trimmedJet.Pt()*0.001, eventWeight );
    h_trimmed_jetM->Fill( trimmedJet.M()*0.001, eventWeight );

    h_jet_numSubjets->Fill( subjets.size(), eventWeight );

    if(m_detailStr.find("btag") != std::string::npos){
      if( (*jet_itr)->isAvailable<int>("TruthLabelID") ){
        const int truthLabelID = (*jet_itr)->getAttribute<int>("TruthLabelID");
        // if it is a b-quark
        if(truthLabelID == 5){
          // http://acode-browser.usatlas.bnl.gov/lxr/source/atlas/Event/xAOD/xAODBTagging/xAODBTagging/versions/BTagging_v1.h
          const xAOD::BTagging* btag = (*jet_itr)->btagging();
          if(bool(btag)){
            double mv1 = btag->MV1_discriminant();
            h_mv1_discriminant->Fill( mv1, eventWeight );
          } else {
            Info("execute()", "Could not get the btagging informationa");
          }
        }
      }
    }

    /*
    const xAOD::JetContainer matched_jets = helpers.match_largeR_jet_to_smallR_jets( (*jet_itr), antikt4_jets);
    const xAOD::JetContainer btagged_jets = helpers.select_container_btags(&matched_jets, 0.8);
    // int num_bTags = helpers.count_container_btags(matched_jets.first, 0.8);
    int num_bTags = btagged_jets.size(); // 0.8 ~ 70% efficiency
    int num_bTags_withTruth = helpers.count_truthLabel_byID(&btagged_jets, 5); // #count btags
    h_num_bTags->Fill( num_bTags );
    h_num_bTags_withTruth->Fill ( num_bTags_withTruth );
    */



  }


  int num_rc_jets = 0;
  std::vector<TLorentzVector> rc_jets = helpers.jet_reclustering(jets);
  for(auto rc_jet: rc_jets){
    if(rc_jet.Pt() > 50.0){
      h_rc_jetPt->Fill( rc_jet.Pt(), eventWeight );
      h_rc_jetM->Fill( rc_jet.M(), eventWeight );
      num_rc_jets++;
    }
  }

  h_numJets->Fill( jets->size(), eventWeight );
  h_numrcJets->Fill( num_rc_jets, eventWeight );
  h_numSubjets->Fill( numSubjets, eventWeight );

  return StatusCode::SUCCESS;
}
