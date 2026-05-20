#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#ifndef SATELLITE_H
#define SATELLITE_H

inline const double Earth_Radius = 6371000;
inline const double G = 6.67430e-11;
inline const double M = 5.972e24;
inline const double PI = 3.141592653589;
inline const double GM = 3.986004418e14;
inline double eA;



class Satellite {
public:
    std::string name = "UNKNOW";

    //Vector parametrs
    std::vector<double> position{ 0.0, 0.0, 0.0 };
    std::vector<double> velocity{ 0.0, 0.0, 0.0 };
    std::vector<double> orientation{ 0.0, 0.0, 0.0 };
    std::vector<double> groundStation = { 2845000.0, 2208000.0, 5249000.0 }; //Moscow station coordinats

    //Orbital parametrs
    double mass = 0.0f;
    double inclination = 0.0;
    double specificImpuls = 0.0;
    double thrust = 0.01;
    double batteryCapacity = 100.0;
    double powerGemerationRate = 0.0;
    double dragCoefficient = 2.2;
    double fuelMass = 0.0;
    double fuelRemaining = fuelMass;
    double powerConsumption = 200.0;
    double crossSectionArea = 2.0;
    double powerGenerationRate = 500.0;
    double targetAltitudeKm = 0.0;
    double altitudeToleranceKm = 5.0;
    double maxEccentricity = 0.01;
    bool orbitCorrectionEnabled = false;

    //General features
    double estimatedLifetime = 0;

    //Others paraments
    double signalDelay = 0.0;
    bool isContactWithGround = false;
    bool isInShadow = false;
    double temperature = 0.0;
    double batteryLevel = 0.0;

    void ShowInfo() {
        std::cout << "===Satellite '" << name << "' detail info===\n\n";
        std::cout << "--Orbital and Main parametrs--" << std::endl;
        std::cout << "Satellite name: " << name << std::endl;
        std::cout << "Mass: " << mass << std::endl;
        std::cout << "Fuel mass: " << fuelMass << std::endl;
        std::cout << "Altitude: " << getAltitude() << std::endl;
        std::cout << "inclination: " << getInclination();
        if (inclination == 0 || inclination > -0.01 && inclination < 0.01) {
            std::cout << " (equatorial orbit)" << std::endl;
        }
        else if (inclination > 89.9 && inclination < 90.1) {
            std::cout << " (polar orbit)" << std::endl;
        }
        else if (inclination > 0 && inclination < 90) {
            std::cout << " (inclined orbit)" << std::endl;
        }
        else if (inclination > 90 && inclination <= 180) {
            std::cout << " (retrograde orbite)" << std::endl;
        }
        else {
            std::cout << std::endl;
        }
        std::cout << "specificImpuls: " << specificImpuls << std::endl;
        std::cout << "thrust: " << thrust << std::endl;
        std::cout << "Battery capacity: " << batteryCapacity << std::endl;
        std::cout << "Drag coefficient: " << dragCoefficient << std::endl;
        std::cout << "Power Generation Rate: " << powerGemerationRate << "\n";
        std::cout << "Elevation Angle: " << eA << "\n\n";

        std::cout << "--Others paraments--" << std::endl;
        std::cout << "Position: x" << position[0] << " y" << position[1] << " z" << position[2] << std::endl;
        std::cout << "Velocity: x" << velocity[0] << " y" << velocity[1] << " z" << velocity[2] << std::endl;
        std::cout << "Orientation: x" << orientation[0] << " y" << orientation[1] << " z" << orientation[2] << std::endl;

        std::cout << "Temperature (C): " << temperature << std::endl;
        std::cout << "Signal Delay: " << signalDelay << std::endl;
        std::cout << "Fuel Remaining: " << fuelRemaining << std::endl;
        std::cout << "Contact With Ground: ";
        if (isContactWithGround == true) {
            std::cout << "TRUE  \n";
        }
        else if (isContactWithGround == false) {
            std::cout << "FALSE \n";
        }
        else {
            std::cout << "UNKNOW\n";
        }
        std::cout << "In Shadow: ";
        if (isInShadow == true) {
            std::cout << "TRUE  \n";
        }
        else if (isInShadow == false) {
            std::cout << "FALSE \n";
        }
        else {
            std::cout << "UNKNOW\n";
        }
        std::cout << "Eccentricity: "
            << std::fixed
            << std::setprecision(12)
            << getEccentricity()
            << std::endl;

        std::cout.unsetf(std::ios_base::floatfield);
        std::cout << std::setprecision(6);

        if (getEccentricity() >= 0.025 or getEccentricity() <= 0.0) {
            std::cout << " (CRITICAL!)\n";
        }
        else {
            std::cout << "\n";
        }

        std::cout << "Orbital Period: " << getOrbitalPeriod() << std::endl;
        std::cout << "Specific Energy: " << getSpecificEnergy() << "\n\n";

        std::cout << "--General features--\n";
        std::cout << "Total Mass: " << getTotalMass() << std::endl;
        std::cout << "Orbital Speed: " << GetOrbitalSpeed() << std::endl;
        std::cout << "Estimated Lifetime: " << estimatedLifetime << std::endl;
        std::cout << "Distance: " << getDistance() << std::endl;
    }

    Satellite() {
        position.resize(3, 0.0);
        velocity.resize(3, 0.0);
        orientation.resize(3, 0.0);
    }

    double getDistance() const {
        return sqrt(position[0] * position[0] + position[1] * position[1] + position[2] * position[2]);
    }

    double GetOrbitalSpeed() const {
        return sqrt(velocity[0] * velocity[0] + velocity[1] * velocity[1] + velocity[2] * velocity[2]);
    }

    double getEccentricity() const {

        //Angular momentum per unit mass
        double hx = position[1] * velocity[2] - position[2] * velocity[1];
        double hy = position[2] * velocity[0] - position[0] * velocity[2];
        double hz = position[0] * velocity[1] - position[1] * velocity[0];

        double r = getDistance();

        //Vector of eccentricity
        double ex = (velocity[1] * hz - velocity[2] * hy) / GM - position[0] / r;
        double ey = (velocity[2] * hx - velocity[0] * hz) / GM - position[1] / r;
        double ez = (velocity[0] * hy - velocity[1] * hx) / GM - position[2] / r;

        double ecc = sqrt(ex * ex + ey * ey + ez * ez);

        if (ecc <= 0) {
            return 0.0;
        }
        return ecc;
    }


    double getAltitude() {
        return (getDistance() - Earth_Radius) / 1000;
    }

    double getOrbitalPeriod() {
        return 2 * PI * sqrt(pow(getDistance(), 3) / (GM));
    }

    double getSpecificEnergy() {
        return (GetOrbitalSpeed() * GetOrbitalSpeed()) / 2.0 - (GM) / getDistance();
    }

    double getTotalMass() {
        return mass + fuelRemaining;
    }

    double getInclination() {
        double hx = position[1] * velocity[2] - position[2] * velocity[1];
        double hy = position[2] * velocity[0] - position[0] * velocity[2];
        double hz = position[0] * velocity[1] - position[1] * velocity[0];
        double h = sqrt(hx * hx + hy * hy + hz * hz);

        inclination = acos(hz / h) * (180.0 / PI);
        return inclination;
    }

    void updateNadirOrientation() {
        double r = getDistance();
        double nadirX = -position[0] / r;
        double nadirY = -position[1] / r;
        double nadirZ = -position[2] / r;

        //Pitch
        orientation[1] = asin(nadirZ) * (180.0 / PI);
        //Yaw
        orientation[2] = atan2(nadirY, nadirX) * (180.0 / PI);
        //Roll
        orientation[0] = 0.0;
    }

    void updateSignalDelay() {
        const double speedOfLight = 299792458.0;
        signalDelay = (2.0 * getDistance()) / speedOfLight * 1000.0;
    }

    void applyDrag(double deltaTime) {
        double altitudeM = getAltitude() * 1000;
        if (altitudeM > 1000000) return;

        double rho = 1.225 * exp(-altitudeM / 8500.0);

        double speed = GetOrbitalSpeed();
        double totalMass = getTotalMass();

        double dragForce = 0.5 * rho * speed * speed * dragCoefficient * crossSectionArea;

        double decel = (dragForce / totalMass) * deltaTime;
        velocity[0] -= decel * (velocity[0] / speed);
        velocity[1] -= decel * (velocity[1] / speed);
        velocity[2] -= decel * (velocity[2] / speed);

    }

    void updateGroundContact() {

        double dx = position[0] - groundStation[0];
        double dy = position[1] - groundStation[1];
        double dz = position[2] - groundStation[2];

        double gsR = sqrt(groundStation[0] * groundStation[0] + groundStation[1] * groundStation[1] + groundStation[2] * groundStation[2]);

        double nx = groundStation[0] / gsR;
        double ny = groundStation[1] / gsR;
        double nz = groundStation[2] / gsR;

        double dist = sqrt(dx * dx + dy * dy + dz * dz);
        double dot = dx * nx + dy * ny + dz * nz;
        double elevationAngle = asin(dot / dist) * (180.0 / PI);

        eA = elevationAngle;

        isContactWithGround = (elevationAngle > 0.0);
    }

    void updateShadow() {
        std::vector<double> sunDirection = { 1.0, 0.0, 0.0 };

        double proj = position[0] * sunDirection[0] + position[1] * sunDirection[1] + position[2] * sunDirection[2];

        if (proj >= 0) {
            isInShadow = false;
            return;
        }

        double r = getDistance();
        double distToAxis = sqrt(r * r - proj * proj);

        isInShadow = (distToAxis < Earth_Radius);
    }


    void updateTemperature(double deltaTime) {
        const double solarConstant = 1361.0;
        const double stefanBoltzmann = 5.67e-8;
        const double absorptivity = 0.3;
        const double emessivity = 0.8;
        const double surfaceArea = 4.0;
        const double heatCapacity = 500.0;

        double tempK = temperature + 273.15;

        double solarPower = isInShadow ? 0.0 : solarConstant * absorptivity * surfaceArea;

        double radiatedPower = emessivity * stefanBoltzmann * surfaceArea * pow(tempK, 4);
        double netPower = solarPower - radiatedPower;

        double deltaTemp = (netPower * deltaTime) / heatCapacity;
        temperature += deltaTemp;
    }

    void updatePower(double deltaTime) {
        double hours = deltaTime / 3600.0;

        if (!isInShadow) {
            double netPower = powerGenerationRate - powerConsumption;
            batteryLevel += netPower * hours;
        }
        else {
            batteryLevel -= powerConsumption * hours;
        }

        batteryLevel = std::max(0.0, std::min(batteryLevel, batteryCapacity));
    }

    void applyThrust(double deltaTime, std::vector<double> thrustDirection) {
        if (fuelRemaining <= 0) return;

        double g0 = 9.80665;

        double fuelBurnRate = thrust / (specificImpuls * g0);
        double fuelUsed = fuelBurnRate * deltaTime;

        if (fuelUsed > fuelRemaining) {
            fuelUsed = fuelRemaining;
        }

        fuelRemaining -= fuelUsed;

        double totalMass = getTotalMass();
        double ax = (thrust * thrustDirection[0] / totalMass);
        double ay = (thrust * thrustDirection[1] / totalMass);
        double az = (thrust * thrustDirection[2] / totalMass);

        velocity[0] += ax * deltaTime;
        velocity[1] += ay * deltaTime;
        velocity[2] += az * deltaTime;
    }

    void correctOrbit(double deltaTime) {
        if (!orbitCorrectionEnabled) return;
        if (fuelRemaining <= 0) return;

        double currentAlt = getAltitude();
        double ecc = getEccentricity();
        double speed = GetOrbitalSpeed();
        double r = getDistance();

        std::vector<double> prograde = { velocity[0] / speed, velocity[1] / speed, velocity[2] / speed };
        std::vector<double> retrograde = { -velocity[0] / speed, -velocity[1] / speed, -velocity[2] / speed };

        double rdotv = position[0] * velocity[0] + position[1] * velocity[1] + position[2] * velocity[2];
        bool nearApsides = std::abs(rdotv) / (r * speed) < 0.01;

        if (ecc > maxEccentricity * 1.05) {
            if (nearApsides) {
                if (rdotv > 0) {
                    if (currentAlt > targetAltitudeKm) {}
                }
            }
        }

        if (currentAlt < (targetAltitudeKm - altitudeToleranceKm)) {
            if (rdotv < 0.1 && rdotv > -0.01) {
                applyThrust(deltaTime, prograde);
            }
        }
        else if (currentAlt > (targetAltitudeKm + altitudeToleranceKm)) {
            if (rdotv < 0.1 && rdotv > -0.01) {
                applyThrust(deltaTime, retrograde);
            }
        }
    }

    void Update(double deltaTime) {

        /*double currentR = sqrt(position[0] * position[0] + position[1] * position[1] + position[2] * position[2]);

        if (currentR <= Earth_Radius) {
            velocity = { 0.0, 0.0, 0.0 };
            position[0] = (position[0] / currentR) * Earth_Radius;
            position[1] = (position[1] / currentR) * Earth_Radius;
            position[2] = (position[2] / currentR) * Earth_Radius;
            return;
        }*/

        updateShadow();
        updatePower(deltaTime);
        updateTemperature(deltaTime);
        applyDrag(deltaTime);
        correctOrbit(deltaTime);
        updateSignalDelay();
        updateNadirOrientation();
        updateGroundContact();

        auto calcAccel = [&](std::vector<double>& pos) -> std::vector<double> {
            double x = pos[0], y = pos[1], z = pos[2];
            double r = sqrt(x * x + y * y + z * z);
            double g = GM / (r * r * r);
            return { -g * x, -g * y, -g * z };
            };

        std::vector<double> a0 = calcAccel(position);

        position[0] += velocity[0] * deltaTime + 0.5 * a0[0] * deltaTime * deltaTime;
        position[1] += velocity[1] * deltaTime + 0.5 * a0[1] * deltaTime * deltaTime;
        position[2] += velocity[2] * deltaTime + 0.5 * a0[2] * deltaTime * deltaTime;


        std::vector<double> a1 = calcAccel(position);

        velocity[0] += 0.5 * (a0[0] + a1[0]) * deltaTime;
        velocity[1] += 0.5 * (a0[1] + a1[1]) * deltaTime;
        velocity[2] += 0.5 * (a0[2] + a1[2]) * deltaTime;
    }
};

#endif