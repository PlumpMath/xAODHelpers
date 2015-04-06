# xAODHelpers - A RootCore Package

## Installing
The last stable analysis base used is **2.1.30**. To install,
```bash
mkdir myRootCore && cd myRootCore
rcSetup Base,2.1.30
git clone -b WTagPlots --single-branch https://github.com/kratsg/xAODHelpers.git
rc checkout_pkg atlasoff/Reconstruction/Jet/JetSubStructureUtils/tags/JetSubStructureUtils-00-02-08
rc find_packages
rc compile_pkg xAODHelpers
```

## Dependencies
 - dependencies are in [cmt/Makefile.RootCore](cmt/Makefile.RootCore)
 - uses Miles [JetSubstructureTools](https://github.com/mileswu/JetSubstructureTools) which is included in AnalysisBase 2.0.18+

### Functionality Included

![Jet Transverse Momentum for AntiKt10 reclustered jets, AntiKt10LCTopo jets, and AntiKt10 trimmed jets](/data/jet_pts.png?raw=true "Jet Pts")

![Jet Masses for AntiKt10 reclustered jets, AntiKt10LCTopo jets, and AntiKt10 trimmed jets](/data/jet_masses.png?raw=true "Jet Masses")

### Tested Against AnalysisBase versions:
 - 2.1.30, 29
 - 2.0.23
 - 2.0.22
 - 2.0.20
 - 2.0.18

#### Authors
- [Giordon Stark](https://github.com/kratsg)

##### Acknowledgements
- [Miles Wu](https://github.com/mileswu)
