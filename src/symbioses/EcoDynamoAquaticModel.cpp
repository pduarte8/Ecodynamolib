#include "config.h"
#include "SymbiosesFramework.h"
#include "TelePrompter.h"
#include "EcoDynamoAquaticModel.h"
#include "ecodynamo_aquatic_model.h"
#include <cassert>

using namespace std;

EcoDynamoAquaticModel::EcoDynamoAquaticModel() {
    setPluginName("EcoDynamo/Stuff 1.0");
    nOrganisms = 0;
    ecodynamo_initialize();
}

EcoDynamoAquaticModel::~EcoDynamoAquaticModel() {
    ecodynamo_finalize();
}

int EcoDynamoAquaticModel::getModelTime() {
	return ecodynamo_get_time();
}

int EcoDynamoAquaticModel::getModelTimeStep() {
	return ecodynamo_get_time_step();
}

void EcoDynamoAquaticModel::getControlVolume(int i, int j, int d, float *vol) {
    ecodynamo_control_volume(vol);
}

void EcoDynamoAquaticModel::getOrganism(Organism *o, int n,
        float lat, float lon, float depth) {
    assert(n < nOrganisms);
    ecodynamo_get_organism(o, n+1, lat, lon, depth);
}

