#ifndef ECODYNAQUATICMODEL_H
#define ECODYNAQUATICMODEL_H

#include <vector>
#include "AquaticModel.h"
#include "Organism.h"
#include "hydrobjt.h"

class EcoDynamoAquaticModel: public AquaticModel {
    public:
        EcoDynamoAquaticModel();
        ~EcoDynamoAquaticModel();

        int getModelTime();
        int getModelTimeStep();

        int getNumOrganismTypes() const { return nOrganisms; }
        void getControlVolume(int i, int j, int d, float *vol);
        void getOrganism(Organism *o, int n, float lat, float lon, float depth);
    private:
        int nOrganisms;
        int t;
        int dt;
        SymbiosesExample *s;

};

#endif
// vim:et:sw=4:ts=4:
