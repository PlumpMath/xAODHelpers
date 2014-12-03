#include <UCHelpers/ElectronHists.h>

ElectronHists::ElectronHists() {}
ElectronHists::~ElectronHists() {}

EL::StatusCode ElectronHists::initialize(bool sumw2) {
  h_electronPt           = book("electronPt", m_containerName, "{p}_{t} [GeV]", 100, 0, 500, sumw2);
  h_electronM            = book("electronM" , m_containerName, "{m} [GeV]", 100, 0, 500, sumw2);
  h_electronEta          = book("electronEta", m_containerName, "\\eta", 100, -4.9, 4.9, sumw2);
  h_electronPhi          = book("electronPhi", m_containerName, "\\phi", 100, -3.2, 3.2, sumw2);

  h_numElectrons         = book("numElectrons", m_containerName, "# electrons", 100, 0, 20, sumw2);

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode ElectronHists::execute() {
  // get electron container of interest
  typedef const xAOD::ElectronContainer* electron_t;

  electron_t electrons = 0;
  if ( !m_wk->xaodEvent()->retrieve( electrons, m_containerName.c_str() ).isSuccess() ){ // retrieve arguments: container type, contain        er key
    Error("execute()", "Failed to retrieve %s. Exiting.", m_containerName.c_str() );
    return EL::StatusCode::FAILURE;
  }

  // Info("execute()", "  number of %s = %lu", m_electronContainerName.c_str(), electrons->size());

  xAOD::ElectronContainer::const_iterator electron_itr = electrons->begin();
  xAOD::ElectronContainer::const_iterator electron_end = electrons->end();

  // loop over the electrons
  for( ; electron_itr != electron_end; ++electron_itr ) {
    // basic kinematics
    h_electronPt->Fill( (*electron_itr)->pt()*0.001 );
    h_electronM->Fill( (*electron_itr)->m()*0.001 );
    h_electronEta->Fill( (*electron_itr)->eta() );
    h_electronPhi->Fill( (*electron_itr)->phi() );
  }

  h_numElectrons->Fill( electrons->size() );

  return EL::StatusCode::SUCCESS;

}

EL::StatusCode ElectronHists::finalize() {
  return EL::StatusCode::SUCCESS;
}
