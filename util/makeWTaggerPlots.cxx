#include "xAODRootAccess/Init.h"
#include "SampleHandler/SampleHandler.h"
#include "SampleHandler/ToolsDiscovery.h"
#include "EventLoop/Job.h"
#include "EventLoop/DirectDriver.h"
#include "SampleHandler/DiskListLocal.h"
#include <TSystem.h>

#include <xAODHelpers/WTaggedHistsAlgo.h>

int main( int argc, char* argv[] ) {

  // Take the submit directory from the input if provided:
  std::string submitDir = "submitDir";
  if( argc > 1 ) submitDir = argv[ 1 ];

  // Set up the job for xAOD access:
  xAOD::Init().ignore();

  // Construct the samples to run on:
  SH::SampleHandler sh;

  /*
  // get the data path for xAODAnaHelpers/data
  std::string dataPath = gSystem->ExpandPathName("/share/t3data/kratsg/xAODs");
  SH::DiskListLocal list (dataPath);
  SH::scanDir (sh, list, "*.root.1", "mc14_13TeV.110401.PowhegPythia_P2012_ttbar_nonallhad.merge.DAOD_JETM8.e2928_s1982_s2008_r5787_r5853_p1845_tid04894526_00");
  */

  SH::DiskListLocal list("/share/t3data3/kratsg/xAODs");
  SH::scanDir(sh, list, "*", "mc14_13TeV.110401.PowhegPythia_P2012_ttbar_nonallhad.merge.DAOD_SUSY4.e2928_s1982_s2008_r5787_r5853_p1862_tid05130501_00");

  /*
  SH::DiskListLocal list("/share/home/kratsg");
  SH::scanDir(sh, list, "DAOD*.root", "xAODAnalysis");
  */

  // Set the name of the input TTree. It's always "CollectionTree"
  // for xAOD files.
  sh.setMetaString( "nc_tree", "CollectionTree" );

  // Print what we found:
  sh.print();

  // Create an EventLoop job:
  EL::Job job;
  job.sampleHandler( sh );

  // Add our analysis to the job:
  WTaggedHistsAlgo* plotWTags = new WTaggedHistsAlgo("wtags_", "$ROOTCOREBIN/data/xAODHelpers/test_plotWTagExample.config");

  // Attach algorithms
  job.algsAdd( plotWTags );

  // Run the job using the local/direct driver:
  EL::DirectDriver driver;
  driver.submit( job, submitDir );

  return 0;
}

