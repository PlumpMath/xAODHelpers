#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

#include "xAODJet/JetContainer.h"
#include "xAODTracking/VertexContainer.h"
#include "xAODEventInfo/EventInfo.h"
#include "AthContainers/ConstDataVector.h"

#include <xAODAnaHelpers/JetHists.h>
#include <xAODHelpers/WTaggedHistsAlgo.h>
#include <xAODAnaHelpers/HelperFunctions.h>
#include <xAODAnaHelpers/HelperClasses.h>

#include <xAODAnaHelpers/tools/ReturnCheck.h>
#include <xAODAnaHelpers/tools/ReturnCheckConfig.h>

#include "TEnv.h"
#include "TSystem.h"

// this is needed to distribute the algorithm to the workers
ClassImp(WTaggedHistsAlgo)

WTaggedHistsAlgo :: WTaggedHistsAlgo () {
}

WTaggedHistsAlgo :: WTaggedHistsAlgo (std::string name, std::string configName) :
  Algorithm(),
  m_name(name),
  m_configName(configName),
  m_plots0W(nullptr),
  m_plots1W(nullptr),
  m_plots2W(nullptr),
  m_plots3W(nullptr),
  m_plots4W(nullptr)
{
}

EL::StatusCode WTaggedHistsAlgo :: setupJob (EL::Job& job)
{
  job.useXAOD();
  xAOD::Init("WTaggedHistsAlgo").ignore();

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode WTaggedHistsAlgo :: histInitialize ()
{

  Info("histInitialize()", "%s", m_name.c_str() );
  // needed here and not in initalize since this is called first
  Info("histInitialize()", "Attempting to configure using: %s", m_configName.c_str());
  if ( this->configure() == EL::StatusCode::FAILURE ) {
    Error("histInitialize()", "%s failed to properly configure. Exiting.", m_name.c_str() );
    return EL::StatusCode::FAILURE;
  } else {
    Info("histInitialize()", "Succesfully configured! \n");
  }

  // declare class and add histograms to output
  m_plots0W = new JetHists(m_name+"0W", m_detailStr);
  m_plots1W = new JetHists(m_name+"1W", m_detailStr);
  m_plots2W = new JetHists(m_name+"2W", m_detailStr);
  m_plots3W = new JetHists(m_name+"3W", m_detailStr);
  m_plots4W = new JetHists(m_name+"4W", m_detailStr);

  m_plots0W -> initialize( );
  m_plots1W -> initialize( );
  m_plots2W -> initialize( );
  m_plots3W -> initialize( );
  m_plots4W -> initialize( );

  m_plots0W -> record( wk() );
  m_plots1W -> record( wk() );
  m_plots2W -> record( wk() );
  m_plots3W -> record( wk() );
  m_plots4W -> record( wk() );

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode WTaggedHistsAlgo :: configure ()
{
  m_configName = gSystem->ExpandPathName( m_configName.c_str() );
  RETURN_CHECK_CONFIG( "WTaggedHistsAlgo::configure()", m_configName);

  // the file exists, use TEnv to read it off
  TEnv* config = new TEnv(m_configName.c_str());
  m_inContainerName         = config->GetValue("InputContainer",  "");
  m_detailStr               = config->GetValue("DetailStr",       "");
  m_decorationName          = config->GetValue("DecorationName", "wtag");

  // in case anything was missing or blank...
  if( m_inContainerName.empty() || m_detailStr.empty() ){
    Error("configure()", "One or more required configuration values are empty");
    return EL::StatusCode::FAILURE;
  }
  Info("configure()", "Loaded in configuration values");

  // everything seems preliminarily ok, let's print config and say we were successful
  config->Print();
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode WTaggedHistsAlgo :: fileExecute () { return EL::StatusCode::SUCCESS; }
EL::StatusCode WTaggedHistsAlgo :: changeInput (bool /*firstFile*/) { return EL::StatusCode::SUCCESS; }

EL::StatusCode WTaggedHistsAlgo :: initialize ()
{
  Info("initialize()", "WTaggedHistsAlgo");
  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode WTaggedHistsAlgo :: execute ()
{

  // pass WTag decoration
  static SG::AuxElement::Accessor<char> passTagDecor(m_decorationName);
  // num WTags decoration
  static SG::AuxElement::Accessor<int> numTagDecor("num_"+m_decorationName);

  const xAOD::EventInfo* eventInfo = HelperFunctions::getContainer<xAOD::EventInfo>("EventInfo", m_event, m_store);

  float eventWeight(1);
  if( eventInfo->isAvailable< float >( "eventWeight" ) ) {
    eventWeight = eventInfo->auxdecor< float >( "eventWeight" );
  }

  // this will be the collection processed - no matter what!!
  const xAOD::JetContainer* inJets = HelperFunctions::getContainer<xAOD::JetContainer>(m_inContainerName, m_event, m_store);

  std::cout << inJets << "|" << eventInfo << std::endl;
  std::cout << m_plots0W << "|" << m_plots1W << "|" << m_plots2W << "|" << m_plots3W << "|" << m_plots4W << std::endl;
  Info("execute()", "here");

  switch( numTagDecor(*eventInfo) ){
      case 0:
        m_plots0W->execute( inJets, eventWeight );
      break;
      case 1:
        m_plots1W->execute( inJets, eventWeight );
      break;
      case 2:
        m_plots2W->execute( inJets, eventWeight );
      break;
      case 3:
        m_plots3W->execute( inJets, eventWeight );
      break;
      case 4:
        m_plots4W->execute( inJets, eventWeight );
      break;
      default:
        Info("execute()", "More than 4 W-tags???");
      break;
  }

  for(const auto jet: *inJets){
    std::cout << "|" << static_cast<bool>(passTagDecor(*jet)) << "|" << std::endl;
  }

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode WTaggedHistsAlgo :: postExecute () { return EL::StatusCode::SUCCESS; }
EL::StatusCode WTaggedHistsAlgo :: finalize () { return EL::StatusCode::SUCCESS; }

EL::StatusCode WTaggedHistsAlgo :: histFinalize () {
  // clean up memory
  if(m_plots0W){
    delete m_plots0W;
    m_plots0W = 0;
  }
  if(m_plots1W){
    delete m_plots1W;
    m_plots1W = 0;
  }
  if(m_plots2W){
    delete m_plots2W;
    m_plots2W = 0;
  }
  if(m_plots3W){
    delete m_plots3W;
    m_plots3W = 0;
  }
  if(m_plots4W){
    delete m_plots4W;
    m_plots4W = 0;
  }

  return EL::StatusCode::SUCCESS;
}
