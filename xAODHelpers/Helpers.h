#ifndef xAODHelpers_Helpers_H
#define xAODHelpers_Helpers_H

// include AntiKt4Truth Jets for btagging
#include "xAODJet/JetContainer.h"
// aux container for deep copies
#include "xAODJet/JetAuxContainer.h"
// deal with btagging
#include "xAODBTagging/BTagging.h"

namespace xAODHelpers {

  class Helpers {

  public:
    Helpers();

    // booleans to help check for certain properties
    bool check_bTag_cut(const xAOD::Jet* jet, double bTagCut);
    bool check_truthLabel_ID(const xAOD::Jet* jet, int truthLabel_ID);

    // helper functions to quickly count
    int count_container_btags (const xAOD::JetContainer* jets, double bTagCut);
    int count_truthLabel_byID (const xAOD::JetContainer* jets, int truthLabel_ID);

    // helper functions to quickly select
    const xAOD::JetContainer select_container_btags(const xAOD::JetContainer* jets, double bTagCut);
    void select_container_btags(const xAOD::JetContainer* jets, double bTagCut, std::string decoratorName);

    // given a largeR jet and a container of smallR jets, return a std::pair of dR matching jets
    const xAOD::JetContainer match_largeR_jet_to_smallR_jets(const xAOD::Jet* largeR_jet, const xAOD::JetContainer* smallR_jets);
  private:

  };
}
#endif
