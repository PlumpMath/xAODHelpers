#include "UCHelpers/Helpers.h"

UCHelpers :: Helpers :: Helpers ()
{
}

int UCHelpers::Helpers::count_container_btags(const xAOD::JetContainer* jets, double bTagCut)
{
  int num_bTags = 0;
  xAOD::JetContainer::const_iterator jet_itr = jets->begin();
  xAOD::JetContainer::const_iterator jet_end = jets->end();
  for( ; jet_itr != jet_end; ++jet_itr ){
    const xAOD::BTagging* btag = (*jet_itr)->btagging();
    if(bool(btag)){
      double mv1 = btag->MV1_discriminant();
      if(mv1 >= bTagCut) num_bTags++;
    }
  }

  return num_bTags;
}

std::pair< xAOD::JetContainer*, xAOD::JetAuxContainer* > UCHelpers::Helpers::select_container_btags( const xAOD::JetContainer* jets, double bTagCut)
{
  xAOD::JetContainer* copyJets = new xAOD::JetContainer();
  xAOD::JetAuxContainer* copyJetsAux = new xAOD::JetAuxContainer();
  copyJets->setStore(copyJetsAux); // connect the two
  xAOD::JetContainer::const_iterator jet_itr = jets->begin();
  xAOD::JetContainer::const_iterator jet_end = jets->end();
  for( ; jet_itr != jet_end; ++jet_itr ){
    const xAOD::BTagging* btag = (*jet_itr)->btagging();
    if(bool(btag)){
      double mv1 = btag->MV1_discriminant();
      if(mv1 >= bTagCut){
        xAOD::Jet* copied_jet = new xAOD::Jet();
        copied_jet->makePrivateStore( **jet_itr );
        copyJets->push_back( copied_jet );
      }
    }
  }
  std::pair< xAOD::JetContainer*, xAOD::JetAuxContainer* > copyJets_pair;
  copyJets_pair = std::make_pair(copyJets, copyJetsAux);
  return copyJets_pair;
}

int UCHelpers::Helpers::count_truthLabel_byId(const xAOD::JetContainer* jets, int user_truthLabelID)
{
  int num_truthLabel = 0;
  xAOD::JetContainer::const_iterator jet_itr = jets->begin();
  xAOD::JetContainer::const_iterator jet_end = jets->end();
  for( ; jet_itr != jet_end; ++jet_itr ){
    if( (*jet_itr)->isAvailable<int>("TruthLabelID") ){
      const int data_truthLabelID = (*jet_itr)->getAttribute<int>("TruthLabelID");
      if(data_truthLabelID == user_truthLabelID){
        num_truthLabel++;
      }
    }
  }
  return num_truthLabel;
}

std::pair< xAOD::JetContainer*, xAOD::JetAuxContainer* > UCHelpers::Helpers::match_largeR_jet_to_smallR_jets(const xAOD::Jet* largeR_jet, const xAOD::JetContainer* smallR_jets)
{
  xAOD::JetContainer* matchedJets = new xAOD::JetContainer();
  xAOD::JetAuxContainer* matchedJetsAux = new xAOD::JetAuxContainer();
  matchedJets->setStore(matchedJetsAux); // connect the two

  TLorentzVector jet4Vector = largeR_jet->p4();
  double jetSizeParameter = largeR_jet->getSizeParameter();

  xAOD::JetContainer::const_iterator jet_itr = smallR_jets->begin();
  xAOD::JetContainer::const_iterator jet_end = smallR_jets->end();


  for( ; jet_itr != jet_end; ++jet_itr){
    if(jet4Vector.DeltaR( (*jet_itr)->p4() ) <= jetSizeParameter){
      // copy this jet to the output container
      xAOD::Jet* smallR_jet = new xAOD::Jet();
      smallR_jet->makePrivateStore( **jet_itr );
      matchedJets->push_back( smallR_jet );
    }
  }

  std::pair< xAOD::JetContainer*, xAOD::JetAuxContainer* > matchedJets_pair;
  matchedJets_pair = std::make_pair(matchedJets, matchedJetsAux);
  return matchedJets_pair;
}
