# xAODHelpers - A RootCore Package

**Note: v2.0 of BosonTagger**

## Installing
The last stable analysis base used is **2.3.12**. To install,
```bash
mkdir myRootCore && cd $_
rcSetup Base,2.3.12
git clone -b WTagPlots --single-branch https://github.com/kratsg/xAODHelpers.git
git clone https://github.com/kratsg/JetSubstructureTools
ln -s JetSubstructureTools/JetSubStructureUtils JetSubStructureUtils
rc find_packages
rc compile
```

## Dependencies
 - dependencies are in [cmt/Makefile.RootCore](cmt/Makefile.RootCore)
 - uses Miles [JetSubstructureTools](https://github.com/mileswu/JetSubstructureTools)

### Functionality Included

![Jet Transverse Momentum for AntiKt10 reclustered jets, AntiKt10LCTopo jets, and AntiKt10 trimmed jets](/data/jet_pts.png?raw=true "Jet Pts")

![Jet Masses for AntiKt10 reclustered jets, AntiKt10LCTopo jets, and AntiKt10 trimmed jets](/data/jet_masses.png?raw=true "Jet Masses")

#### Authors
- [Giordon Stark](https://github.com/kratsg)

##### Acknowledgements
- [Miles Wu](https://github.com/mileswu)
