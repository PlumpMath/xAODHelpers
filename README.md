# xAODHelpers - A RootCore Package

## Installing
The last stable analysis base used is **2.0.18**. To install,
```bash
mkdir myRootCore && cd myRootCore
rcSetup Base,2.0.18
git clone https://github.com/kratsg/xAODHelpers.git
# git clone https://github.com/mileswu/JetSubstructureTools
git clone https://github.com/kratsg/JetSubStructureUtils
rc find_packages
rc compile_pkg xAODHelpers
```

## Dependencies
 - dependencies are in [cmt/Makefile.RootCore](cmt/Makefile.RootCore)
 - uses Miles [JetSubstructureTools](https://github.com/mileswu/JetSubstructureTools) which is included in AnalysisBase 2.0.18+ _except for SubjetFinder_.

### Functionality Included
 - `HistogramManager` helps manage histograms in `EventLoop` algorithms by initializing histograms and adding it to the worker.
 - `JetKinematics` is an `EventLoop` algorithm used to automate the creation of histograms for a given jet container. See [makeJetKinematics.cxx](util/makeJetKinematics.cxx) for an example.

#### Authors
- [Giordon Stark](https://github.com/kratsg)

##### Acknowledgements
This is inspired by Gabriel Facini's [package](https://svnweb.cern.ch/trac/atlasperf/browser/CombPerf/Tracking/TrackingInDenseEnvironments/SimpleAnaxAOD/trunk/HistManager/).
