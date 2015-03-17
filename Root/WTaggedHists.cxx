#include <xAODHelpers/WTaggedHists.h>

WTaggedHists :: WTaggedHists (std::string name, std::string detailStr) :
  HistogramManager(name, detailStr)
{
}

WTaggedHists :: ~WTaggedHists () {}

EL::StatusCode WTaggedHists::initialize() {

  m_jetTags           = book(m_name, "multiplicity",  "Jet Tags", 10, 0, 9);

  Info("WTaggedHists::initialize()", m_name.c_str());

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode WTaggedHists::execute( const xAOD::JetContainer* jets, float eventWeight ) {
  m_jetTags->Fill( jets->size(), eventWeight );
  return EL::StatusCode::SUCCESS;
}
