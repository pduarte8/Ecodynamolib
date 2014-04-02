#include "config.h"
#include "SymbiosesFramework.h"
#include "TelePrompter.h"
#include "EcoDynamoAquaticModel.h"
#include <cassert>

using namespace std;

EcoDynamoAquaticModel::EcoDynamoAquaticModel() {
    setPluginName("EcoDynamo 1.0");
    nOrganisms = 0;
    t = 0;
    dt = 3600;
    s = new SymbiosesExample();
    //ecodynamo_initialize();
}

EcoDynamoAquaticModel::~EcoDynamoAquaticModel() {
    delete s;
    //ecodynamo_finalize();
}

int EcoDynamoAquaticModel::getModelTime() {
	//return ecodynamo_get_time();
    t += dt;
    return t;
}

int EcoDynamoAquaticModel::getModelTimeStep() {
    return dt;
	//return ecodynamo_get_time_step();
}

void EcoDynamoAquaticModel::getControlVolume(int i, int j, int d, float *vol) {
    vol[0] = 1.0;
    vol[1] = 1.0;
    vol[2] = 1.0;
    //ecodynamo_control_volume(vol);
}

void EcoDynamoAquaticModel::getOrganism(Organism *o, int n,
        float lat, float lon, float depth) {
    if (n == 0) {
        s->printGrid();
    } else if (n == 1) {
        s->printVelocity(10, 10, 10);
        s->printVelocity(20, 20, 20);
        s->printVelocity(30, 30, 30);
    } else {
        s->printTS(10, 10, 10);
        s->printTS(20, 20, 20);
        s->printTS(30, 30, 30);
    }
    //assert(n < nOrganisms);
    //ecodynamo_get_organism(o, n+1, lat, lon, depth);
}

