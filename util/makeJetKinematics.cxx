#include "xAODRootAccess/Init.h"
#include "SampleHandler/SampleHandler.h"
#include "SampleHandler/ToolsDiscovery.h"
#include "EventLoop/Job.h"
#include "EventLoop/DirectDriver.h"
#include "SampleHandler/DiskListLocal.h"
#include <TSystem.h>

#include "UCHelpers/JetKinematics.h"

int main( int argc, char* argv[] ) {

  // Take the submit directory from the input if provided:
  std::string submitDir = "submitDir";
  if( argc > 1 ) submitDir = argv[ 1 ];

  // Set up the job for xAOD access:
  xAOD::Init().ignore();

  // Construct the samples to run on:
  SH::SampleHandler sh;

  const char* inputFilePath = "/share/t3data/kratsg/xAODs";
  SH::DiskListLocal list (inputFilePath);
  SH::scanDir (sh, list, "AOD.01604209._000001.pool.root.1"); // specifying one particular file for testing

  // Set the name of the input TTree. It's always "CollectionTree"
  // for xAOD files.
  sh.setMetaString( "nc_tree", "CollectionTree" );

  // Print what we found:
  sh.print();

  // Create an EventLoop job:
  EL::Job job;
  job.sampleHandler( sh );

  // Add our analysis to the job:
  JetKinematics* jk_AntiKt10LC = new JetKinematics();
  /* This is the default container used */
  jk_AntiKt10LC->m_jetContainerName = "AntiKt10LCTopoJets";
  jk_AntiKt10LC->m_jetDisplayName = "LC";

  JetKinematics* jk_AntiKt10Truth = new JetKinematics();
  jk_AntiKt10Truth->m_jetContainerName = "AntiKt10TruthJets";
  jk_AntiKt10Truth->m_jetDisplayName = "Truth";

  JetKinematics* jk_AntiKt10TruthWZ = new JetKinematics();
  jk_AntiKt10TruthWZ->m_jetContainerName = "AntiKt10TruthWZJets";
  jk_AntiKt10TruthWZ->m_jetDisplayName = "TruthWZ";

  /* Doesn't work since this is an xAOD::JetTrigContainer
  JetKinematics* jk_HLT = new JetKinematics();
  jk_HLT->m_jetContainerName = "HLT_xAOD__JetContainer_TrigHLTJetRec";
  jk_HLT->m_jetDisplayName = "HLT";
  */

  // Attach algorithms
  job.algsAdd( jk_AntiKt10LC );
  job.algsAdd( jk_AntiKt10Truth );
  job.algsAdd( jk_AntiKt10TruthWZ );

  // Run the job using the local/direct driver:
  EL::DirectDriver driver;
  driver.submit( job, submitDir );

  return 0;
}

