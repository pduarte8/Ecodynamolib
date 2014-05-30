#include "config.h"
#include "SymbiosesFramework.h"
#include "TelePrompter.h"
#include "EcoDynamoAquaticModel.h"
#include <cassert>

using namespace std;

EcoDynamoAquaticModel::EcoDynamoAquaticModel() {
    setPluginName("EcoDynamo 1.0");
    nOrganisms = 1;
    t = 0;
    dt = 720;
    dynamo = new EcoDynamo();
    TEcoDynClass *dynclass = dynamo->init();
    //s = new TTriDimensionalSymbioses(dynclass, "foobar");
}

EcoDynamoAquaticModel::~EcoDynamoAquaticModel() {
    dynamo->finalize();
    delete s;
    delete dynamo;
}
int EcoDynamoAquaticModel::next() {
    dynamo->next();
    return 1;
}

int EcoDynamoAquaticModel::getModelTime() {
    t += dt;
    return t;
}

int EcoDynamoAquaticModel::getModelTimeStep() {
    return dt;
}

void EcoDynamoAquaticModel::getOrganism(Organism *o, int n,
        float lat, float lon, float depth) {
    assert(n >= 0 and n <= nOrganisms);

    o->setId(n);
    o->setWeight(0.0);
    o->setAbundance(0.0);
    o->setSize(0.0);
    o->setAge(t);
    o->setGrowthRate(0.0);
    o->setReproductionRate(0.0);
    o->setMortalityRate(0.0);
    o->setFeedingRate(0.0);
    for (int i=0; i < o->getNumBodyBurdens(); i++) {
        o->setBodyBurden(i, 0.0);
    }
}

