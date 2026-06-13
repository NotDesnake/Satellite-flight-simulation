#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#ifndef MANNEDSPACECRAFT_H
#define MANNEDSPACECRAFT_H

struct MannedSpacecraft {

	std::string name = "NAME";


	// === Aerodynamic / Kinematic / Physics ===
	glm::dvec3 m_position{ 0.0f, 6371000.0, 0.0 };
	glm::dvec3 m_velocity{ 0.0, 0.0, 0.0 };
	glm::dvec3 m_acceleration{ 0.0, 0.0, 0.0 };

	glm::dquat m_orientation = glm::dquat(1.0f, 0.0f, 0.0f, 0.0f);
	glm::vec3 m_angularVelocity{ 0.0f, 0.0f, 0.0f };
	glm::vec3 m_angularAcceleration{ 0.0f, 0.0f, 0.0f };
	double m_dryMass = 6350.0;
	glm::vec3 m_dragCoefficient{ 0.0f, 0.0f, 0.0f };
	glm::vec3 m_crossSectionArea{ 0.0f, 0.0f, 0.0f };
	glm::vec3 aerodynamicCenter{ 0.0f, 0.0f, 0.0f };

	// === Engines / Fuel / SAS ===
	float m_mainEngineThrust = 2950.0f;
	float m_rcsThrust = 130.0f;
	bool m_rcsValveStates[28]{ false };
	float m_engineGimbalLimit = 5.0f;
	glm::vec2 m_gimbalAngle{ 0.0f, 0.0f };
	float m_specificImpulse = 305.0f;
	double m_fuelMass = 800.0;
	double m_rocketFuelMass = 274000.0;
	float m_rcsPropellantMass = 100.0f;
	float m_PropellantPressure = 300000.0f;
	float m_sasSolidFuelMass = 1500.0f;
	float m_sasThrust = 780000.0f;

	// === Energy (SEP) / Temperature Control ===
	float m_mainBusVoltage = 28.0f;
	float m_batteryCharge = 100.0f;
	bool m_isInEarthShadow = false;
	float m_solarPanelSunAngle = 0.0f;
	bool m_heaterPowerState[4]{ true, false, true, false };
	float m_pumpFlowRate = 1.5f;
	float m_hullTemperature = 20.0f;


	// === Durabillity / Comparaments / Construction ===
	float m_sealElasticity = 100.0f;
	float m_structuralFatigue = 0.0f;
	float m_hullVibrationLevel = 0.0f;
	float m_leakRate = 0.0f;
	float m_pressureDropRate = 0.0f;
	uint8_t m_leakSectionID = 0;
	uint8_t m_parachuteState = 0;
	bool m_boosterAttached[4]{ true, true, true, true };
	bool m_stage2Attached = true;
	bool m_stage3Attached = true;
	bool m_fairingAttached = true;
	bool m_isBO_Active = true;
	bool m_isSA_Active = true;
	bool m_isPAO_Active = true;
	bool m_isDocked = false;
	uint8_t m_visualMeshID = 1;

	// === Life Support === 
	float m_oxygenMass = 3.5f;
	float m_nitrogenMass = 11.5f;
	float m_co2Mass = 0.05f;
	float m_co2ScrubberLife = 100.0f;
	float m_cabinPressure = 101325.0f;
	float m_cabinTemperature = 22.0f;
	float m_humidityLevel = 50.0f;
	float m_airMakeUpGasMass = 20.0f;
	bool m_isCompensationValveOpen = false;

	// === Navigation ===
	double m_onboardTime = 0.0f;
	bool m_bcvkPowerState = true;
	bool m_ramCorrupted = false;
	glm::vec3 m_dusReading{ 0.0f, 0.0f, 0.0f };
	uint8_t m_starTrackerState = 0;
	bool m_plasmaBlackoutActive = false;
	bool m_groundStationLOS = true;
	bool m_isManualControl = false;
	uint8_t m_displayFormatID = 1;
	uint64_t m_matrixIndicatorsState = 0;
	bool m_audioAlarmActive = false;


	// === Logic / Triggers / Docking ===
	uint8_t m_targetStationID = 0;
	glm::vec2 m_kursAntennaPitchRoll{ 0.0f, 0.0f };
	bool m_isDockingProbeExtended = false;
	float m_dockingAxisMisalignment = 0.0f;
	bool m_dockingHookState[8]{ false };
	bool m_isPadAbortPossible = true;
	bool m_boosterFireDetected = false;
	bool m_isSasFired = false;

	// === External harm === 
	float m_ambientTemperature = 15.0f;
	float m_windSpeed = 3.0f;
	float m_windShear = 0.0f;
	float m_lightningRisk = 0.0f;
	double m_airDensity = 1.225;


	// === KYNEMATIC / AERODYNAMIC ===


	double getAltitude() const {
		return glm::length(m_position) - 6371000.0;
	}

	double getOrbitalSpeed() const {
		return glm::length(m_velocity);
	}

	double getMachNumber() const {
		return getOrbitalSpeed() / 340.29;
	}

	double getDynamicPressure() const {
		double v = getOrbitalSpeed();
		return 0.5 * m_airDensity * v * v;
	}

	float getGForce() const {
		return static_cast<float>(glm::length(m_acceleration) / 9.81);
	}


	// === ENGINES / MASS / ORBITAL MECHANICS ===


	double getTotalMass() const {
		return m_dryMass + m_fuelMass + m_rcsPropellantMass;
	}

	float getFuelPercentage() const {
		return static_cast<float>((m_fuelMass / 800.0) * 100.0);
	}

	double getDeltaV() const {
		if (m_fuelMass <= 0.0) return 0.0;
		return m_specificImpulse * 9.81 * std::log(getTotalMass() / m_dryMass);
	}

	double getCurrentThrustWeight() const {
		if (getTotalMass() == 0.0) return 0.0;
		return m_mainEngineThrust / (getTotalMass() * 9.81);
	}


	// === VECTOR FORCES ===


	glm::dvec3 getGravityVector() const {
		double r = glm::length(m_position);
		if (r == 0.0) return glm::dvec3(0.0);
		double gravityMagnitude = 3.986004418e14 / (r * r);
		return -glm::normalize(m_position) * gravityMagnitude * getTotalMass();
	}

	glm::dvec3 getThrustVector() const {
		glm::dvec3 localThrust(0.0, m_mainEngineThrust, 0.0);
		return m_orientation * localThrust;
	}

	glm::dvec3 getDragForceVector() const {
		if (getOrbitalSpeed() == 0.0) return glm::dvec3(0.0);
		double dragMagnitude = getDynamicPressure() * m_dragCoefficient.y * m_crossSectionArea.y;
		return -glm::normalize(m_velocity) * dragMagnitude;
	}


	// === NAVIGATION ===


	double getPitch() const {
		return glm::degrees(glm::pitch(m_orientation));
	}

	double getYaw() const {
		return glm::degrees(glm::yaw(m_orientation));
	}

	double getRoll() const {
		return glm::degrees(glm::roll(m_orientation));
	}


	// === LIFE SUPPORT SYSTEM ===


	float getCabinPressureAtm() const {
		return m_cabinPressure / 101325.0f;
	}

	float getOxygenPartialPressure() const {
		float totalGasMass = m_oxygenMass + m_nitrogenMass + m_co2Mass;
		if (totalGasMass <= 0.0f) return 0.0f;
		return (m_oxygenMass / totalGasMass) * m_cabinPressure;
	}

	bool isHypoxiaWarning() const {
		return getOxygenPartialPressure() < 16000.0f;
	}

	bool isCrewConscious() const {
		if (getGForce() > 9.0f || isHypoxiaWarning()) {
			return false;
		}
		return true;
	}


	// === ENERGY ===


	float getTotalPowerDraw() const {
		float draw = 0.0f;
		if (m_bcvkPowerState) draw += 400.0f;
		for (bool heater : m_heaterPowerState) {
			if (heater) draw += 150.0f;
		}
		return draw;
	}

	float getPowerBalance() const {
		float solarGeneration = 0.0f;
		if (!m_isInEarthShadow) {
			solarGeneration = 1200.0f;
		}
		return solarGeneration - getTotalPowerDraw();
	}

};

#endif