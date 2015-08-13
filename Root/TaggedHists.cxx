#include <xAODHelpers/TaggedHists.h>

TaggedHists :: TaggedHists (std::string name, std::string detailStr) :
  HistogramManager(name, detailStr)
{
}

TaggedHists :: ~TaggedHists () {}

StatusCode TaggedHists::initialize() {

  m_jetTags           = book(m_name, "multiplicity",  "Jet Tags", 10, 0, 9);

  Info("TaggedHists::initialize()", m_name.c_str());

  return StatusCode::SUCCESS;
}

StatusCode TaggedHists::execute( const xAOD::JetContainer* jets, float eventWeight ) {
  m_jetTags->Fill( jets->size(), eventWeight );
  return StatusCode::SUCCESS;
}
