# xAODHelpers - A RootCore Package

## Installing
The last stable analysis base used is **2.0.23**. To install,
```bash
mkdir myRootCore && cd myRootCore
rcSetup Base,2.0.23
git clone https://github.com/kratsg/xAODHelpers.git
rc checkout_pkg atlasoff/Reconstruction/Jet/JetSubStructureUtils
rc find_packages
rc compile_pkg xAODHelpers
```

## Dependencies
 - dependencies are in [cmt/Makefile.RootCore](cmt/Makefile.RootCore)
 - uses Miles [JetSubstructureTools](https://github.com/mileswu/JetSubstructureTools) which is included in AnalysisBase 2.0.18+ _except for SubjetFinder_.

### Functionality Included
 - `JetHists` is a plotting class for Jets to predefine a lot of plots to make for an input JetContainer (see `execute()`). This should only be changed to add new plots, but not to add in plots that require cuts on a jet -- that should be done in an EL algorithm such as `JetKinematics`.
 - `JetKinematics` is an `EventLoop` algorithm used to automate the creation of histograms for a given jet container. See [makeJetKinematics.cxx](util/makeJetKinematics.cxx) for an example.
 - `Helpers::jet_reclustering` and `Helpers::jet_trimming` are functions to allow the user to recluster and trim jets appropriately.

![Jet Transverse Momentum for AntiKt10 reclustered jets, AntiKt10LCTopo jets, and AntiKt10 trimmed jets](/data/jet_pts.png?raw=true "Jet Pts")

![Jet Masses for AntiKt10 reclustered jets, AntiKt10LCTopo jets, and AntiKt10 trimmed jets](/data/jet_masses.png?raw=true "Jet Masses")

### Tested Against AnalysisBase versions:
 - 2.0.23
 - 2.0.22
 - 2.0.20
 - 2.0.18

#### Authors
- [Giordon Stark](https://github.com/kratsg)

##### Acknowledgements
- [Miles Wu](https://github.com/mileswu)
