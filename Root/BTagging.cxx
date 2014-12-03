#include "UCHelpers/BTagging.h"

UCHelpers :: BTagging :: BTagging ()
{
}

int UCHelpers::BTagging::result(const xAOD::JetContainer* jets, double bTagCut)
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
