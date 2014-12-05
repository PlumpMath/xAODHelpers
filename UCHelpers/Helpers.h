#ifndef UCHelpers_Helpers_H
#define UCHelpers_Helpers_H

// include AntiKt4Truth Jets for btagging
#include "xAODJet/JetContainer.h"
// aux container for deep copies
#include "xAODJet/JetAuxContainer.h"
// deal with btagging
#include "xAODBTagging/BTagging.h"

namespace UCHelpers {

  class Helpers {

  public:
    Helpers();

    // 70% loose efficiency, cut by allowing only those with x >= btag_cut
    int count_event_btags (const xAOD::JetContainer* jets, double bTagCut=0.8);
    // given a largeR jet and a container of smallR jets, return a std::pair of dR matching jets
    std::pair< xAOD::JetContainer*, xAOD::JetAuxContainer* > match_largeR_jet_to_smallR_jets(const xAOD::Jet* largeR_jet, const xAOD::JetContainer* smallR_jets);
  private:

  };
}
#endif
