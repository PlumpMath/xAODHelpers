#include "xAODHelpers/Helpers.h"

xAODHelpers :: Helpers :: Helpers ()
{
}

bool xAODHelpers::Helpers::check_bTag_cut(const xAOD::Jet* jet, double bTagCut)
{
  const xAOD::BTagging* btag = jet->btagging();
  if(bool(btag)){
    double mv1 = btag->MV1_discriminant();
    return mv1 >= bTagCut;
  }
  return false;
}

bool xAODHelpers::Helpers::check_truthLabel_ID(const xAOD::Jet* jet, int truthLabel_ID)
{
  if( jet->isAvailable<int>("TruthLabelID") ){
    return jet->getAttribute<int>("TruthLabelID") == truthLabel_ID;
  }
  return false;
}

int xAODHelpers::Helpers::count_container_btags(const xAOD::JetContainer* jets, double bTagCut)
{
  int num_bTags = 0;

  for(auto jet : *jets){
    if(xAODHelpers::Helpers::check_bTag_cut(jet, bTagCut)) num_bTags++;
  }

  return num_bTags;
}

xAOD::JetContainer* xAODHelpers::Helpers::select_container_btags(xAOD::JetContainer* jets, double bTagCut)
{
  xAOD::JetContainer* selectedJets = new xAOD::JetContainer(SG::VIEW_ELEMENTS);

  for(auto jet : *jets){
    if(xAODHelpers::Helpers::check_bTag_cut(jet, bTagCut)) selectedJets->push_back( jet );
  }
  return selectedJets;
}

void xAODHelpers::Helpers::select_container_btags(xAOD::JetContainer* jets, double bTagCut, std::string decoratorName)
{
  for(auto jet: *jets){
    if(xAODHelpers::Helpers::check_bTag_cut(jet, bTagCut) ) jet->auxdecor<int>(decoratorName) = 1;
  }

}

int xAODHelpers::Helpers::count_truthLabel_byID(const xAOD::JetContainer* jets, int truthLabel_ID)
{
  int num_truthLabel = 0;
  for(auto jet: *jets){
    if(xAODHelpers::Helpers::check_truthLabel_ID(jet, truthLabel_ID)) num_truthLabel++;
  }
  return num_truthLabel;
}

xAOD::JetContainer* xAODHelpers::Helpers::match_largeR_jet_to_smallR_jets(const xAOD::Jet* largeR_jet, const xAOD::JetContainer* smallR_jets)
{
  xAOD::JetContainer* matchedJets = new xAOD::JetContainer(SG::VIEW_ELEMENTS);

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

  return matchedJets;
}
