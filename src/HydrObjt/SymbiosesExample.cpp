#include <iostream>
#include <Eigen/Core>

#include "ecodyn.rh"
#include "hydrobjt.h"
#include "SymbiosesFramework.h"
#include "HydrodynamicModel.h"
#include "AtmosphericModel.h"

using namespace std;
using namespace Eigen;

static HydrodynamicModel *ocean;
static AtmosphericModel *atmo;

SymbiosesExample::SymbiosesExample() {
    SymbiosesFramework &symb = SymbiosesFramework::getInstance();
    ocean = symb.getHydrodynamicModel("ocean");
    atmo = symb.getAtmosphericModel("meteo");

    griddims = symb.getReferenceGridDimensions();
    gridlats = symb.getReferenceGridLatitudes();
    gridlons = symb.getReferenceGridLongitudes();
}

SymbiosesExample::~SymbiosesExample() {
}

void SymbiosesExample::printGrid() const {
    MatrixXf dep(griddims[0], griddims[1]);
    for (int j=0; j < griddims[1]; j++) {
        for (int i=0; i < griddims[0]; i++) {
            float lat = gridlats[i + j*griddims[0]];
            float lon = gridlons[i + j*griddims[0]];
            dep(i, j) = ocean->getDepthSounding(lat, lon);
            cout << lat << " " << lon << " " << dep(i,j) << endl;
        }
    }
}

void SymbiosesExample::printVelocity(int i, int j, int k) const {
    float v[3];
    float layers[griddims[2]];
    float lat = gridlats[i + j*griddims[0]];
    float lon = gridlons[i + j*griddims[0]];
    ocean->getLayerDepths(lat, lon, layers);
    float d = layers[k];
    ocean->getVelocity(lat, lon, d, v);
    cout << "velocity = "
        << lat << " " << lon << " "
        << v[0] << " " << v[1] << " " << v[2] << endl;
}

void SymbiosesExample::printTS(int i, int j, int k) const {
    float t, s;
    float layers[griddims[2]];
    float lat = gridlats[i + j*griddims[0]];
    float lon = gridlons[i + j*griddims[0]];
    ocean->getLayerDepths(lat, lon, layers);
    float d = layers[k];
    cout << "t,s = "
        << lat << " " << lon << " "
        << ocean->getTemperature(lat, lon, d) << " "
        << ocean->getSalinity(lat, lon, d) << endl;
}
