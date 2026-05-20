#include <iostream>
#include <map>
#include <string>
#include  <functional>
#include <vector>
#include <sstream>
#define NOMINMAX
#include <windows.h>
#include <cstdlib>
#include <cmath>
#include <thread>
#include "Satellite.h"

void Visualization(Satellite& sat);

int main() {

    Satellite sat;
    sat.position[0] = 7000000;
    sat.position[1] = 0;
    sat.position[2] = 0;

    sat.velocity[0] = 0;
    sat.velocity[1] = 7546;
    sat.velocity[2] = 0;

    sat.mass = 500;
    sat.fuelRemaining = 100;
    sat.fuelMass = 50;
    sat.name = "Desnake";
    sat.crossSectionArea = 2.0;

    sat.thrust = 10;
    sat.specificImpuls = 300;

    sat.targetAltitudeKm = 629.0;
    sat.altitudeToleranceKm = 5.0;
    sat.maxEccentricity = 0.005;
    sat.orbitCorrectionEnabled = true;

    Visualization(sat);
    

    return 0;
}