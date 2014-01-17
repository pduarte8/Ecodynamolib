#ifndef ECODYN_AQUATIC_MODEL_H
#define ECODYN_AQUATIC_MODEL_H

#include "Organism.h"
#include "AquaticModel.h"

extern "C" {
    int ecodynamo_initialize();
    int ecodynamo_finalize();
    int ecodynamo_next();
    int ecodynamo_get_norganisms();
    void ecodynamo_control_volume(float *vol);
    void ecodynamo_get_organism(Organism *object, int n,
            float lat, float lon, float depth);
    int ecodynamo_get_nbb();
}

#endif
