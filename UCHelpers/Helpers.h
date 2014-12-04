#ifndef UCHelpers_Helpers_H
#define UCHelpers_Helpers_H

// include AntiKt4Truth Jets for btagging
#include "xAODJet/JetContainer.h"
// deal with btagging
#include "xAODBTagging/BTagging.h"

namespace UCHelpers {

  class Helpers {

  public:
    Helpers();

    // 70% loose efficiency, cut by allowing only those with x >= btag_cut
    int count_event_btags (const xAOD::JetContainer* jets, double bTagCut=0.8);

  private:

  };
}
#endif
