#ifndef ECODYNAQUATICMODEL_H
#define ECODYNAQUATICMODEL_H

#include <vector>
#include "AquaticModel.h"
#include "Organism.h"
#include "hydrobjt.h"
#include "EcoDynamo.h"

class EcoDynamoAquaticModel: public AquaticModel {
    public:
        EcoDynamoAquaticModel();
        ~EcoDynamoAquaticModel();

        int next();
        int getModelTime();
        int getModelTimeStep();

        int getNumOrganismTypes() const { return nOrganisms; }
        void getOrganism(Organism *o, int n, float lat, float lon, float depth);
    private:
        int nOrganisms;
        int t;
        int dt;
        EcoDynamo *dynamo;
        TTriDimensionalSymbioses *s;

};

#endif
// vim:et:sw=4:ts=4:
