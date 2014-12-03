#ifndef UCHelpers_BTagging_H
#define UCHelpers_BTagging_H

// include AntiKt4Truth Jets for btagging
#include "xAODJet/JetContainer.h"
// deal with btagging
#include "xAODBTagging/BTagging.h"


namespace UCHelpers {

  class BTagging {

  public:
    BTagging();
    // 70% loose efficiency, cut by allowing only those with x >= btag_cut
    int result (const xAOD::JetContainer* jets, double bTagCut=0.8);

  private:

  };
}
#endif
