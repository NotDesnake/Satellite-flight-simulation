#pragma execution_character_set("utf-8")
#include <iostream>
#include <boost/nowide/iostream.hpp>
#include <boost/nowide/args.hpp>
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
#include <conio.h>


void Visualization(Satellite& sat);
HWND consoleWindow = GetConsoleWindow();
std::vector<std::string> typeOfSpacecraft;

void MannedSpacecraftSettings(Satellite& sat, int argc, char* argv[]) {

}


void SatellteSettings(Satellite& sat, int argc, char* argv[]) {
	std::string temp;
	system("cls");
	int user_answer1;
	bool isTrue1 = false;
	boost::nowide::args _(argc, argv);

	int ans = 0;
	bool bl;

	auto Title = [&]() -> int {
		boost::nowide::cout << "\033[32m"
			<< "  ██████╗███████╗████████╗████████╗██╗███╗   ██╗ ██████╗ ██████╗ \n"
			<< " ██╔════╝██╔════╝╚══██╔══╝╚══██╔══╝██║████╗  ██║██╔════╝██╔════╝ \n"
			<< " ╚█████╗ █████╗     ██║      ██║   ██║██╔██╗ ██║██║  ███╗╚█████╗ \n"
			<< "  ╚═══██╗██╔══╝     ██║      ██║   ██║██║╚██╗██║██║   ██║ ╚═══██╗\n"
			<< " ██████╔╝███████╗   ██║      ██║   ██║██║ ╚████║╚██████╔╝██████╔╝\n"
			<< " ╚═════╝ ╚══════╝   ╚═╝      ╚═╝   ╚═╝╚═╝  ╚═══╝ ╚═════╝ ╚═════╝ \n"
			<< "\033[0m";

		boost::nowide::cout << "\033[90m ┌──────────────────────────────────────────────────────────────────────────────┐\033[0m\n";
		boost::nowide::cout << "\033[96m   ► ORBITAL PARAMETERS CONFIGURATION MATRIX                                    \033[0m\n";
		boost::nowide::cout << "\033[90m └──────────────────────────────────────────────────────────────────────────────┘\033[0m\n\n";

		return 67;
		};
	Title();

	boost::nowide::cout << "\033[33mEnter '1' if you want to run the simulation with default values, and select '2' if you want to set custom values.\033[0m\n";
	while (!isTrue1) {
		boost::nowide::cout << ">>" << std::flush;

		std::string line;
		std::getline(boost::nowide::cin, line);

		bool isNumber1 = !line.empty();
		for (char c : line) {
			if (!std::isdigit(c)) {
				isNumber1 = false;
				break;
			}
		}

		if (!isNumber1) {
			boost::nowide::cout << "\033[031mIncorrect input! Enter a number.\033[0m\n";
			continue;
		}

		user_answer1 = std::stoi(line);

		if (user_answer1 < 1 || user_answer1 > 2) {
			boost::nowide::cout << "\033[031mEnter '1' or '2'\033[0m\n";
			continue;
		}
		isTrue1 = true;
	}
	switch (user_answer1) {
	case 1:
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

		system("cls");
		Title();
		boost::nowide::cout << "\033[33mPreview of the set parameters (the parameters that will be calculated during the simulation are still 0).\n\033[31mTo start the simulation, press 'ENTER'\033[0m\n" << std::flush;
		sat.ShowInfo();
		SendMessage(consoleWindow, WM_VSCROLL, SB_TOP, 0);
		while (_getch() != 13) {}
		system("cls");
		Visualization(sat);
		break;

	case 2:
		system("cls");
		Title();
		boost::nowide::cout << "\033[33mCustom configuration of satellite parameters. Parameter No.: 1/17\033[0m\n";

		//=== QUETION 1 ===
		boost::nowide::cout << "(1) Enter the initial X coordinate for the satellite >> " << std::flush;
		std::cin >> ans;
		sat.position[0] = static_cast<float>(ans);
		boost::nowide::cout << "(1) Enter the initial Y coordinate for the satellite >> " << std::flush;
		std::cin >> ans;
		sat.position[1] = static_cast<float>(ans);
		boost::nowide::cout << "(1) Enter the initial Z coordinate for the satellite >> " << std::flush;
		std::cin >> ans;
		sat.position[2] = static_cast<float>(ans);


		system("cls");
		Title();
		boost::nowide::cout << "\033[33mCustom configuration of satellite parameters. Parameter No.: 2/17\033[0m\n";


		//=== QUETION 2 ===
		boost::nowide::cout << "(2) Enter the X-coordinate velocity >> " << std::flush;
		std::cin >> ans;
		sat.velocity[0] = static_cast<float>(ans);
		boost::nowide::cout << "(2) Enter the Y-coordinate velocity >> " << std::flush;
		std::cin >> ans;
		sat.velocity[1] = static_cast<float>(ans);
		boost::nowide::cout << "(2) Enter the Z-coordinate velocity >> " << std::flush;
		std::cin >> ans;
		sat.velocity[2] = static_cast<float>(ans);


		system("cls");
		Title();
		boost::nowide::cout << "\033[33mCustom configuration of satellite parameters. Parameter No.: 3/17\033[0m\n";

		std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

		//=== QUETION 3 ===
		std::string str;
		boost::nowide::cout << "(3) Enter satellite name >> " << std::flush;
		std::getline(boost::nowide::cin, str);
		sat.name = str;


		system("cls");
		Title();
		boost::nowide::cout << "\033[33mCustom configuration of satellite parameters. Parameter No.: 4/17\033[0m\n";


		//=== QUETION 4 ===
		boost::nowide::cout << "(4) Enter the satellite's dry mass (kg) >> " << std::flush;
		std::cin >> ans;
		sat.mass = static_cast<float>(ans);


		system("cls");
		Title();
		boost::nowide::cout << "\033[33mCustom configuration of satellite parameters. Parameter No.: 5/17\033[0m\n";


		//=== QUETION 5 ===
		boost::nowide::cout << "(5) Enter the satellite's fuel mass (kg) >> " << std::flush;
		std::cin >> ans;
		sat.fuelMass = static_cast<double>(ans);


		system("cls");
		Title();
		boost::nowide::cout << "\033[33mCustom configuration of satellite parameters. Parameter No.: 6/17\033[0m\n";


		//=== QUETION 6 ===
		boost::nowide::cout << "(6) Enter the satellite's cross section area (m^2) >> " << std::flush;
		std::cin >> ans;
		sat.crossSectionArea = static_cast<double>(ans);


		system("cls");
		Title();
		boost::nowide::cout << "\033[33mCustom configuration of satellite parameters. Parameter No.: 7/17\033[0m\n";

		//=== QUETION 7 ===
		boost::nowide::cout << "(7) Enter the satellite's drag coefficient >> " << std::flush;
		std::cin >> ans;
		sat.dragCoefficient = static_cast<double>(ans);


		system("cls");
		Title();
		boost::nowide::cout << "\033[33mCustom configuration of satellite parameters. Parameter No.: 8/17\033[0m\n";


		//=== QUETION 8 ===
		boost::nowide::cout << "(8) Enter the satellite's thrust (N) >> " << std::flush;
		std::cin >> ans;
		sat.thrust = static_cast<double>(ans);


		system("cls");
		Title();
		boost::nowide::cout << "\033[33mCustom configuration of satellite parameters. Parameter No.: 9/17\033[0m\n";


		//=== QUETION 9 ===
		boost::nowide::cout << "(9) Enter the satellite's specific impuls (c) >> " << std::flush;
		std::cin >> ans;
		sat.specificImpuls = static_cast<double>(ans);


		system("cls");
		Title();
		boost::nowide::cout << "\033[33mCustom configuration of satellite parameters. Parameter No.: 10/17\033[0m\n";


		//=== QUETION 10 ===
		boost::nowide::cout << "(10) Enter the satellite's battery capacity >> " << std::flush;
		std::cin >> ans;
		sat.batteryCapacity = static_cast<double>(ans);


		system("cls");
		Title();
		boost::nowide::cout << "\033[33mCustom configuration of satellite parameters. Parameter No.: 11/17\033[0m\n";


		//=== QUETION 11 ===
		boost::nowide::cout << "(11) Enter the satellite's power consumption >> " << std::flush;
		std::cin >> ans;
		sat.powerConsumption = static_cast<double>(ans);


		system("cls");
		Title();
		boost::nowide::cout << "\033[33mCustom configuration of satellite parameters. Parameter No.: 12/17\033[0m\n";


		//=== QUETION 12 ===
		boost::nowide::cout << "(12) Enter the satellite's power generation rate >> " << std::flush;
		std::cin >> ans;
		sat.powerGenerationRate = static_cast<double>(ans);


		system("cls");
		Title();
		boost::nowide::cout << "\033[33mCustom configuration of satellite parameters. Parameter No.: 13/17\033[0m\n";


		//=== QUETION 13 ===
		boost::nowide::cout << "(13) Enter the satellite's target altitude (km) >> " << std::flush;
		std::cin >> ans;
		sat.targetAltitudeKm = static_cast<double>(ans);


		system("cls");
		Title();
		boost::nowide::cout << "\033[33mCustom configuration of satellite parameters. Parameter No.: 14/17\033[0m\n";


		//=== QUETION 14 ===
		boost::nowide::cout << "(14) Enter the satellite's altitude tolerance (km) >> " << std::flush;
		std::cin >> ans;
		sat.altitudeToleranceKm = static_cast<double>(ans);


		system("cls");
		Title();
		boost::nowide::cout << "\033[33mCustom configuration of satellite parameters. Parameter No.: 15/17\033[0m\n";


		//=== QUETION 15 ===
		boost::nowide::cout << "(15) Enter the satellite's max eccentricity >> " << std::flush;
		std::cin >> ans;
		sat.maxEccentricity = static_cast<double>(ans);


		system("cls");
		Title();
		boost::nowide::cout << "\033[33mCustom configuration of satellite parameters. Parameter No.: 16/17\033[0m\n";


		//=== QUETION 16 ===
		boost::nowide::cout << "(16) Enable automatic orbit correction? (1 - Yes / 0 - No) >> " << std::flush;
		std::cin >> bl;
		sat.orbitCorrectionEnabled = bl;


		system("cls");
		Title();
		boost::nowide::cout << "\033[33mCustom configuration of satellite parameters. Parameter No.: 17/17\033[0m\n";

		//=== QUETION 17 ===
		boost::nowide::cout << "(17) Enter the X coordinate of the ground station >> " << std::flush;
		std::cin >> ans;
		sat.groundStation[0] = static_cast<double>(ans);
		boost::nowide::cout << "(17) Enter the Y coordinate of the ground station >> " << std::flush;
		std::cin >> ans;
		sat.groundStation[1] = static_cast<double>(ans);
		boost::nowide::cout << "(17) Enter the Z coordinate of the ground station >> " << std::flush;
		std::cin >> ans;
		sat.groundStation[2] = static_cast<double>(ans);

		break;
	}

	system("cls");
	Title();
	boost::nowide::cout << "\033[33mPreview of the set parameters (the parameters that will be calculated during the simulation are still 0).\n\033[31mTo start the simulation, press 'ENTER'\033[0m\n" << std::flush;
	sat.ShowInfo();
	SendMessage(consoleWindow, WM_VSCROLL, SB_TOP, 0);
	while (_getch() != 13) {}
	system("cls");
	Visualization(sat);
}


void MainMenu(Satellite& sat, int argc, char* argv[]) {
	int user_answer;
	bool isTrue = false;

	boost::nowide::args _(argc, argv);

	system("cls");

	boost::nowide::cout << "\033[32m" 
		<< " ██████╗ ██████╗  ██████╗       ██╗███████╗ ██████╗████████╗    ███████╗ ██████╗██╗     ██╗██████╗  ███████╗███████╗\n"
		<< " ██╔══██╗██╔══██╗██╔═══██╗      ██║██╔════╝██╔════╝╚══██╔══╝    ██╔════╝██╔════╝██║     ██║██╔══██╗██╔════╝██╔════╝\n"
		<< " ██████╔╝██████╔╝██║   ██║      ██║█████╗  ██║        ██║       █████╗  ██║     ██║     ██║██████╔╝███████╗█████╗  \n"
		<< " ██╔═══╝ ██╔══██╗██║   ██║██╗   ██║██╔══╝  ██║        ██║       ██╔══╝  ██║     ██║     ██║██╔═══╝ ╚════██║██╔══╝  \n"
		<< " ██║     ██║  ██║╚██████╔╝╚██████╔╝███████╗╚██████╗   ██║       ███████╗╚██████╗███████╗██║██║     ███████║███████╗\n"
		<< " ╚═╝     ╚═╝  ╚═╝ ╚═════╝  ╚═════╝ ╚══════╝ ╚═════╝   ╚═╝       ╚══════╝ ╚═════╝╚══════╝╚═╝╚═╝     ╚══════╝╚══════╝\n"
		<< "\033[0m";

	boost::nowide::cout << "\033[90m ┌──────────────────────────────────────────────────────────────────────────────┐\033[0m\n";
	boost::nowide::cout << "\033[96m   ► SIMULATION OF SPACECRAFT FLIGHTS BY @DESNAKE ◄                      \033[0m\n";
	boost::nowide::cout << "\033[90m └──────────────────────────────────────────────────────────────────────────────┘\033[0m\n\n";

	boost::nowide::cout << "\033[33m ┌── SELECT SPACECRAFT TYPE ──────────────────────────────────────────────────┐\033[0m\n";

	for (size_t i = 0; i < typeOfSpacecraft.size(); i++) {
		std::string icon = " 🛸 ";
		if (i == 1) icon = " 🚀 ";
		if (i == 2) icon = " 🛰️ ";


		boost::nowide::cout << "\033[33m │ \033[0m"
			<< "[" << i + 1 << "]" << icon << typeOfSpacecraft[i];

		int padding = 66 - static_cast<int>(typeOfSpacecraft[i].length());
		for (int p = 0; p < padding; p++) boost::nowide::cout << " ";

		boost::nowide::cout << "\033[33m  │\033[0m\n";
	}

	boost::nowide::cout << "\033[33m └────────────────────────────────────────────────────────────────────────────┘\033[0m\n\n";

	while (!isTrue) {
		boost::nowide::cout << "\033[5m @DESNAKESS // ENTER TARGET INDEX >> \033[0m" << std::flush;

		std::string line;
		std::getline(boost::nowide::cin, line);

		bool isNumber = !line.empty();
		for (char c : line) {
			if (!std::isdigit(c)) {
				isNumber = false;
				break;
			}
		}

		if (!isNumber) {
			boost::nowide::cout << "\033[91m [ERROR] Invalid system command! Input must be an integer.\033[0m\n\n";
			continue;
		}

		user_answer = std::stoi(line);

		if (user_answer < 1 || user_answer >(int)typeOfSpacecraft.size()) {
			boost::nowide::cout << "\033[91m [ERROR] Access denied. Selected spacecraft index out of bounds.\033[0m\n\n";
			continue;
		}
		isTrue = true;
	}

	if (user_answer == 1) {
		SatellteSettings(sat, argc, argv);
	}
	if (user_answer == 2) {
		SatellteSettings(sat, argc, argv);
	}
}



int main(int argc, char* argv[]) {

	typeOfSpacecraft.push_back("Satellite");
	typeOfSpacecraft.push_back("Manned Spacecraft");
	typeOfSpacecraft.push_back("Space Station");

    Satellite sat;
	MainMenu(sat, argc, argv);
    

    return 0;
}