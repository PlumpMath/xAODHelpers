#ifndef UCHelpers_ElectronHists_H
#define UCHelpers_ElectronHists_H

#include <UCHelpers/HistogramManager.h>
#include "xAODEgamma/ElectronContainer.h"

class ElectronHists : public HistogramManager {

  public:
    // initializer and destructor
    ElectronHists();
    ~ElectronHists();

    EL::StatusCode initialize(bool sumw2 = true);
    EL::StatusCode execute();
    EL::StatusCode finalize();
    using HistogramManager::book; // make other overloaded versions of book() to show up in subclass

    // basics
    TH1 *h_electronPt; //!
    TH1 *h_electronM; //!
    TH1 *h_electronEta; //!
    TH1 *h_electronPhi; //!

    // topological (eg: per event)
    TH1 *h_numElectrons; //!

    // scattering angle of two highest pt electrons in CoM frame

  private:
};

#endif
