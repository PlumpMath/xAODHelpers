#include <UCHelpers/JetHists.h>

// subjet finding
#include "JetSubStructureUtils/SubjetFinder.h"
// deal with the btagging
#include "xAODBTagging/BTagging.h"

JetHists::JetHists() {}
JetHists::~JetHists() {}

EL::StatusCode JetHists::initialize(bool sumw2) {
  h_jetPt           = book("jetPt", m_containerName, "{p}_{t} [GeV]", 100, 0, 500, sumw2);
  h_jetM            = book("jetM" , m_containerName, "{m} [GeV]", 100, 0, 500, sumw2);
  h_jetEta          = book("jetEta", m_containerName, "\\eta", 100, -4.9, 4.9, sumw2);
  h_jetPhi          = book("jetPhi", m_containerName, "\\phi", 100, -3.2, 3.2, sumw2);

  h_numJets         = book("numJets", m_containerName, "# jets", 100, 0, 20, sumw2);
  h_numSubjets      = book("numSubjets", m_containerName, "# subjets", 100, 0, 200, sumw2);

  h_jetTau1         = book("jetTau1", m_containerName, "{\\tau}_{1}", 100, 0, 1, sumw2);
  h_jetTau2         = book("jetTau2", m_containerName, "{\\tau}_{2}", 100, 0, 1, sumw2);
  h_jetTau3         = book("jetTau3", m_containerName, "{\\tau}_{3}", 100, 0, 1, sumw2);
  h_jetDip12        = book("jetDip12", m_containerName, "{dip}_{12}", 100, 0, 1, sumw2);
  h_jetDip13        = book("jetDip13", m_containerName, "{dip}_{13}", 100, 0, 1, sumw2);
  h_jetDip23        = book("jetDip23", m_containerName, "{dip}_{23}", 100, 0, 1, sumw2);
  h_jet_numSubjets  = book("jet_numSubjets", m_containerName, "# subjets", 100, 0, 10, sumw2);

  h_mv1_discriminant = book("btag", m_containerName, "MV1 Discriminant", 100, 0, 1, sumw2);

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode JetHists::execute() {
  // get jet container of interest
  typedef const xAOD::JetContainer* jet_t;

  jet_t jets = 0;
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

    if(!isTrigger()){
      if( (*jet_itr)->isAvailable<int>("TruthLabelID") ){
        const int truthLabelID = (*jet_itr)->getAttribute<int>("TruthLabelID");
        // if it is a b-quark
        if(truthLabelID == 5){
          // http://acode-browser.usatlas.bnl.gov/lxr/source/atlas/Event/xAOD/xAODBTagging/xAODBTagging/versions/BTagging_v1.h
          const xAOD::BTagging* btag = (*jet_itr)->btagging();
          if(bool(btag)){
            double mv1 = btag->MV1_discriminant();
            /*
              http://acode-browser.usatlas.bnl.gov/lxr/source/atlas/PhysicsAnalysis/JetMissingEtID/JetMissingEtTagTools/src/JetMissingEtTagTool.cxx#0229
              if (mv1 >  0.9827)  pid |= 1<< 12;      // MV1 @ 60% 
              if (mv1 >  0.7892)  pid |= 1<< 13;      // MV1 @ 70% 
              if (mv1 >  0.6073)  pid |= 1<< 14;      // MV1 @ 75% 
              if (mv1 >  0.1340)  pid |= 1<< 15;      // MV1 @ 85% 
            */
            h_mv1_discriminant->Fill( mv1 );
          } else {
            Info("execute()", "Could not get the btagging informationa");
          }
        }
      }
    }

  }

  h_numJets->Fill( jets->size() );
  h_numSubjets->Fill( numSubjets );

  return EL::StatusCode::SUCCESS;

}

EL::StatusCode JetHists::finalize() {
  // called per worker node
  if(!isTrigger()){
    // integrate btagging
    // http://www.hep.shef.ac.uk/teaching/phy6040/ROOT/ROOTseminars/Seminar_3.html
    TH1* h_int_mv1_discriminant = book("btag_integrated", m_containerName, "MV1 Discriminant", 100, 0, 1, true);
    h_mv1_discriminant->ComputeIntegral();
    double* integratedBins = h_mv1_discriminant->GetIntegral();
    h_int_mv1_discriminant->SetContent(integratedBins);
    h_int_mv1_discriminant->GetYaxis()->SetTitle("Normalized counts");
  }
  return EL::StatusCode::SUCCESS;
}

bool JetHists::isTrigger(){
  // http://stackoverflow.com/a/2340309
  return (m_containerName.find(std::string("Trig")) != std::string::npos);
}
