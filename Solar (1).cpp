#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include<cmath>
using namespace std;

//Whatever you want to change in files, do change it in signup and update profile as well.
float inv_pow;
float total_price;
float  area, location, inv_q, phours, energy, w_len;
float st_a = 0.56, energy_out, inv_price, price_of_plates, e_p;
double maintenance_cost, labour_cost, net_metering_cost, wiring_cost;
int usage = 0, maintain1 = 0, netmeter = 0, labour = 0;
int noPlates;
//Battries:
char b;//battries(y/n)
double energyConsumption = 0, backupDays = 0, DoD = 0;
int systemVoltage = 0;
double batteryCapacity = 0, batterySizeAh = 0;
float batteryCost = 0;
int batteryType = 0; //Type of battery
int hinv_choice = 0; // Hybrid inverter choice
double hinv_cost = 0; //Stores the hybrid inverter cost
int baseinverter_cap = 5000; // The base size of an inverter
bool end1 = false;


string inv_comp;
string plates_typ;
string battery_typ;
string metertype = "No Meter Included";
string houseSize;



void viewInstructions()
{
	cout << R"(
@ Solar Management and Pricing System

## Overview
The Solar Management and Pricing System is a C++ program designed to help users plan and estimate the cost of installing a solar energy system. 
The program calculates the total cost, suggests suitable solar components, and provides a Return on Investment (ROI) based on electricity savings.

---

## Features
- **User Profiles**:
  - Signup, login, view, update, and delete user profiles.
  - Store user data in a CSV file for persistence.

- **Cost Estimation**:
  - Calculate the cost of solar panels, inverters, batteries, and other components.
  - Supports both **personal** and **industrial** use cases.
  - Includes optional costs for maintenance, labor, and net metering.

- **Solar Panel Selection**:
  - Recommends suitable solar panel types based on energy needs and available area.
  - Supports multiple types of solar panels (e.g., Polycrystalline, Monocrystalline).

- **Return on Investment (ROI)**:
  - Calculates daily and annual electricity savings.
  - Estimates the time to break even and total savings over a user-defined period.


)" << endl;
}


//Reset all global variables after each iteration:
void resetGlobals() 
{
	end1 = false;
	noPlates = 0;
	total_price = 0;
	st_a = 0.56;
	energy_out = 0;
	inv_price = 0;
	price_of_plates = 0;
	usage = 0;
	maintain1 = 0;
	netmeter = 0;
	labour = 0;
	area = 0;
	location = 0;
	inv_q = 0;
	phours = 0;
	energy = 0;
	w_len = 0;
	plates_typ = "";
	inv_comp = "";
	metertype = "No Meter Included";
	houseSize = "";
	inv_pow = 0;
	e_p = 0;
	energyConsumption = 0; 
	backupDays = 0, DoD = 0;
	systemVoltage = 0;
	batteryCapacity = 0;
	batterySizeAh = 0;
	batteryCost = 0;
	batteryType = 0;
	hinv_choice = 0;
	hinv_cost = 0; 
	battery_typ = "";
}





// Struct to store user profiles
struct User 
{
	string username;
	string cnic;
	string plates;
	int plates_cost;
	string battery;
	int battery_cost;
	float battery_pow;
	string inverter;
	int inverter_cost;
	string meter = "Net Meter";
	string house;
	int No_Plates;
	float Energy;//Energy in kW per day for output display
	int totalPrice;
	float ef;
};


// Maximum number of users
const int MAX_USERS = 500;
User users[MAX_USERS];
int userCount = 0;


//Conversion of string to float:
float stringToFloat(const string& str) {
	float number = 0.0f;
	bool isNegative = false;
	bool decimalFound = false;
	float decimalPlace = 0.1f;

	// Check if the string starts with a '-' sign
	int start = 0;
	if (!str.empty() && str[0] == '-') {
		isNegative = true;
		start = 1; // Skip the '-' sign for processing
	}

	for (int i = start; i < str.length(); i++) {
		char c = str[i];
		if (c >= '0' && c <= '9') {
			if (!decimalFound) {
				number = number * 10 + (c - '0');
			}
			else {
				number += (c - '0') * decimalPlace;
				decimalPlace *= 0.1f;
			}
		}
		else if (c == '.' && !decimalFound) {
			// Handle the first occurrence of the decimal point
			decimalFound = true;
		}
		else {
			// Invalid character
			//cout << "Invalid input: non-numeric character found" << endl;
		}
	}

	return isNegative ? -number : number;
}


// Function to convert a string to an integer
int stringToInt(const string& str) {
	int number = 0;
	bool isNegative = false;
	int start = 0;

	// Check for negative sign
	if (!str.empty() && str[0] == '-') {
		isNegative = true;
		start = 1; // Skip the '-' sign
	}

	// Loop through each character of the string
	for (int i = start; i < str.length(); i++) {
		if (str[i] >= '0' && str[i] <= '9') {
			number = number * 10 + (str[i] - '0'); // Convert char to digit and build the number
		}
		else {
			//cout << "Invalid input: non-numeric character found" << endl;
		}
	}

	return isNegative ? -number : number;
}


// Function to calculate maintenance cost
double calculateMaintenanceCost(int maintain1, int months = 12) {
	if (maintain1 == 1) {
		return 0;
	}
	else if (maintain1 == 2) {
		return (150 * months )/ 12;
	}
	else if (maintain1 == 3) {
		return (1200 * (months / 12)) / 12;
	}
	return 0;
}


// Function to calculate labor cost
double calculateLabourCost(int labour) {
	if (labour == 1) {
		return 0;
	}
	else if (labour == 2) {
		return 50;
	}
	else if (labour == 3) {
		return 30;
	}
	else if (labour == 4)
	{
		return 45;
	}
	return 0;
}


// Function to calculate net metering cost
double calculateNetMeteringCost(int netmeter) {
	if (netmeter == 1) {
		metertype = "Net Meter";
		return 200;
	}
	else if (netmeter == 2) {
		metertype = "No Meter Included";
		return 0;
	}
	return 0;
}


// Function to calculate wiring cost
double calculateWiringCost(double w_len) {
	double cost_per_meter = 2.5;
	return w_len * cost_per_meter;
}


float personal_plates() 
{
	float e_psp = 0.17, e_tsp = 0.19;
	float p_psp = 250, p_tsp = 350;

	float no_psp, no_tsp;

	float energy_psp = st_a * phours * 5000 * e_psp;
	float energy_tsp = st_a * phours * 5000 * e_tsp;

	// Calculate the number of plates needed for both types
	no_psp = (energy / 365) / energy_psp;
	no_tsp = (energy / 365) / energy_tsp;
	no_psp = ceil(no_psp);
	no_tsp = ceil(no_tsp);

	// Energy and area calculations
	energy_psp *= no_psp;
	energy_tsp *= no_tsp;

	float a_psp = no_psp * st_a;
	float a_tsp = no_tsp * st_a;

	float pf_psp = no_psp * p_psp;
	float pf_tsp = no_tsp * p_tsp;

	bool pspFeasible = (energy_psp * 365 >= energy) && (a_psp <= area);
	bool tspFeasible = (energy_tsp * 365 >= energy) && (a_tsp <= area);

	if (pspFeasible && (!tspFeasible || pf_psp <= pf_tsp)) {
		plates_typ = "Polycrystalline Solar Panels";
		energy_out = energy_psp; // Per year
		noPlates = no_psp;
		e_p = 17;
		return pf_psp;
	}
	else if (tspFeasible) {
		plates_typ = "Thin-Film Solar Panels";
		energy_out = energy_tsp; // Per year
		noPlates = no_tsp;
		e_p = 19;
		return pf_tsp;
	}
	else {
		cout << "Your energy requirements exceed the energy production that can be achieved within your available area.";
		end1 = true;
		return 0; // Return 0 to indicate failure
	}
}


float industrial_plates() 
{
	float e_psp = 0.2, e_tsp = 0.22;
	float p_psp = 500, p_tsp = 700;

	float no_psp, no_tsp;

	float energy_psp = st_a * phours * 5000 * e_psp;
	float energy_tsp = st_a * phours * 5000 * e_tsp;

	// Calculate the number of plates needed for both types
	no_psp = (energy / 365) / energy_psp;
	no_tsp = (energy / 365) / energy_tsp;

	// Energy and area calculations
	energy_psp *= no_psp;
	energy_tsp *= no_tsp;

	float a_psp = no_psp * st_a;
	float a_tsp = no_tsp * st_a;

	float pf_psp = no_psp * p_psp;
	float pf_tsp = no_tsp * p_tsp;

	bool pspFeasible = (energy_psp * 365 >= energy) && (a_psp <= area);
	bool tspFeasible = (energy_tsp * 365 >= energy) && (a_tsp <= area);

	if (pspFeasible && (!tspFeasible || pf_psp <= pf_tsp)) {
		plates_typ = "Monocrystalline Solar Panels";
		energy_out = energy_psp; // Per year
		noPlates = no_psp;
		e_p = 20;
		return pf_psp;
	}
	else if (tspFeasible) {
		plates_typ = "Bifacial Solar Panels";
		energy_out = energy_tsp; // Per year
		noPlates = no_tsp;
		e_p = 22;
		return pf_tsp;
	}
	else {
		cout << "Your energy requirements exceed the energy production that can be achieved within your available area.";
		end1 = true;
		return 0; // Return 0 to indicate failure
	}
}


void personal_inv()
{
	int unit_price = 500;
	float no_u = (inv_pow) / 5000;
	no_u = ceil(no_u);
	inv_price = unit_price * no_u;
}


void industrial_inv()
{
	int unit_price = 800;
	float no_u = (inv_pow) / 5000;
	no_u = ceil(no_u);
	inv_price = unit_price * no_u;
}

void batteryInverters()
{
	energyConsumption = inv_pow;
	do
	{
		cout << "\nEnter the type of hybrid inverter you want with your batteries:\n";
		cout << "1. SMA Solar Technology (more expensive, more reliable)\n";
		cout << "2. Fronius (less expensive, reliable)\n";
		cout << "Your choice: ";
		cin >> hinv_choice;

		if (hinv_choice != 1 && hinv_choice != 2)
		{
			cout << "Invalid inverter type. Please select 1 or 2.\n";
		}
	} while (hinv_choice != 1 && hinv_choice != 2);

	if (hinv_choice == 1)
	{
		inv_comp = "SMA Hybrid";
		hinv_cost = 1350 * (energyConsumption / 5000);
	}
	else if (hinv_choice == 2)
	{
		inv_comp = "Fronius Hybrid";
		hinv_cost = 1200 * (energyConsumption / 5000);
	}
	//cout << "Inverter Cost: $" << hinv_cost << "\n";
	inv_price = hinv_cost;
}



void batteryInput()
{

	do
	{
		cout << "Enter number of backup days (1-3): ";
		cin >> backupDays;

		if (backupDays <= 0 || backupDays > 3)
		{
			cout << "Invalid number of backup days. Please enter a positive number.\n";
		}
	} while (backupDays <= 0 || backupDays > 3);

}


void batteryCalculation()
{
	energyConsumption = (energy_out/365);
	// Calculate battery capacity in kWh
	DoD = DoD / 100.0;
	if (DoD < 0)
	{
		cout << "DoD < 0" << endl;
	}
	batteryCapacity = (energyConsumption * backupDays) / DoD;

	// Calculate battery size in Ah
	batterySizeAh = (batteryCapacity * 1000) / systemVoltage;
}

void personalBatteries()
{
	energyConsumption = (energy_out/365);
	do
	{
		cout << "\nBattery Options:\n";
		cout << "1. Lead-Acid (cheapest, personal use)\n";
		cout << "2. Nickel-Iron (expensive, personal use)\n";
		cin >> batteryType;

		if (batteryType != 1 && batteryType != 2)
		{
			cout << "Invalid battery type. Please select 1 or 2.\n";
		}
	} while (batteryType != 1 && batteryType != 2);

	systemVoltage = 12;

	if (batteryType == 1) // Lead-Acid
	{
		battery_typ = "Lead Acid Battery";
		DoD = 50;
		batteryCost = 10 * energyConsumption;
		cout << "BatteryCost: " << batteryCost << endl;
	}
	else if (batteryType == 2) // Nickel-Iron
	{
		battery_typ = "Nickel Iron Battery";
		DoD = 80;
		batteryCost = 13 * energyConsumption;
		cout << "BatteryCost: " << batteryCost << endl;
	}
	batteryCalculation();
}


void industrialBatteries()
{
	energyConsumption = (energy_out/365);
	do
	{
		cout << "\nBattery Options:\n";
		cout << "1. Li-ion (more expensive, industrial use)\n";
		cout << "2. Flow batteries (most expensive, industrial use)\n";
		cin >> batteryType;

		if (batteryType != 1 && batteryType != 2)
		{
			cout << "Invalid battery type. Please select 1 or 2.\n";
		}
	} while (batteryType != 1 && batteryType != 2);

	systemVoltage = 24;

	if (batteryType == 1) // Li-ion
	{
		battery_typ = "Li-ion Battery";
		DoD = 85;
		batteryCost = 15 * energyConsumption;
		cout << "BatteryCost: " << batteryCost << endl;
	}
	else if (batteryType == 2) // Flow batteries
	{
		battery_typ = "Flow Battery";
		DoD = 100;
		batteryCost = 25 * energyConsumption;
		cout << "BatteryCost: " << batteryCost << endl;
	}
	batteryCalculation();
}



void Appliances()
{
	int i = 0;
	float t, er = 0, no, ans;
	while (er != -1)
	{
		i++;
		cout << "\nEnter energy consumption (in watts) for Appliances " << i << " : \n[Or press -1 if you have no more appliances] :" << endl;
		cin >> er;
		if (er != -1)
		{
			cout << "\nEnter average time (in hours) for which this Appliances runs: " << endl;
			cin >> t;
			cout << "\nEnter the number of these Appliances (if you have more Appliances of the same type) :" << endl;
			cin >> no;
			inv_pow += (no * er);
			ans = no * er * t * 365;
			energy += ans;
			//cout << energy;
		}
	}
}


int input()
{
	Appliances();
	do
	{
		cout << "\nEnter your usage: \n1. Industrial \n2. Personal:\n";
		cin >> usage;

		if (usage != 1 && usage != 2)
		{
			cout << "Invalid entry!!" << endl;
		}

	} while (usage != 1 && usage != 2);

	if (usage == 1)
	{
		int z;
		cout << "\nEnter the size of your Industry: \n1. 5 acers. \n2. 7 acers. \n3. 10 acers. \n4. 15 acers. \n5. 20 acers.";
		cin >> z;
		if (z == 1)
		{
			houseSize = "5 Acers.";
		}
		else if (z == 2)
		{
	
			houseSize = "7 Acers.";
		}
		else if (z == 3)
		{
			houseSize = "10 Acers.";
		}
		else if (z == 4)
		{
			houseSize = "15 Acers.";
		}
		else if (z == 5)
		{
			houseSize = "20 Acers.";
		}
		
		cout << "\nIn which of the following parts of the country is your industral unit laying? (for weather)";
		cout << "\n1. Northern Area\n2. Southern Area\n3. Western Area\n4. Eastern Area\n5. Fedral Area";
		cin >> z;

	}
	if (usage == 2)
	{
		int z;
		cout << "\nEnter the size of your house: \n1. 5 marlas. \n2. 7 marlas \n3. 10 marlas \n4. 15 marlas \n5. 20 marlas.";
		cin >> z;

		if (z == 1)
		{
			houseSize = "5 Marlas.";
		}
		else if (z == 2)
		{
			houseSize = "7 Marlas.";
		}
		else if (z == 3)
		{
			houseSize = "10 Marlas.";
		}
		else if (z == 4)
		{
			houseSize = "15 Marlas.";
		}
		else if (z == 5)
		{
			houseSize = "20 Marlas.";
		}
		cout << "\nWhich part of the country are you a residential of? (for weather)";
		cout << "\n1. Northern Area\n2. Southern Area\n3. Western Area\n4. Eastern Area\n5. Fedral Area";
		cin >> z;
	}


	do
	{
		cout << "\nEnter your maximum available area for the installation of plates on your roof (in squared meters unites (1.7 minimun)) :";
		cin >> area;

		if (area <= 1.7)
		{
			cout << "Invalid entry!!" << endl;
		}

	} while (area <= 1.7);

	



	do
	{
		cout << "\nEnter number peak sunlight hours in your area (1 - 12): ";
		cin >> phours;

		if (phours <= 0 || phours > 12)
		{
			cout << "Invalid entry!!" << endl;
		}

	} while (phours <= 0 || phours > 12);

	//Battries Option:

	do
	{
		cout << "\nDo you need battry system as well? (y/n): " << endl;
		cin >> b;
		if (b != 'y' && b != 'Y' && b != 'n' && b != 'N')
		{
			cout << "Invalid Entry" << endl;
		}
	} while (b != 'y' && b != 'Y' && b != 'n' && b != 'N');

	if (b == 'y' || b == 'Y')
	{
		batteryInput();
	}

	if (b == 'n' || b == 'N')
	{
		do
		{
			cout << "\nDesired quality of Inverter (for chosing company) : \n1. high (sma solar)\n2. moderate (fronius) \n3. low (growatt):";
			cin >> inv_q;

			if (inv_q != 1 && inv_q != 2 && inv_q != 3)
			{
				cout << "Invalid entry!!" << endl;
			}

		} while (inv_q != 1 && inv_q != 2 && inv_q != 3);
	}
	
	do
	{
		cout << "\nEnter the approximate wiring length required (in meters) :";
		cin >> w_len;

		if (w_len <= 0)
		{
			cout << "Invalid entry!!" << endl;
		}

	} while (w_len <= 0);


	do
	{
		cout << "\nMaintainace requriment : \n1. Not Required\n2. Monthly ($150 per month) \n3. Yearly ($1200 per year, 100$ this month):";
		cin >> maintain1;

		if (maintain1 != 1 && maintain1 != 2 && maintain1 != 3)
		{
			cout << "Invalid entry!!" << endl;
		}

	} while (maintain1 != 1 && maintain1 != 2 && maintain1 != 3);

	do
	{
		cout << "\nLabour requriment : \n1. not required\n2. within 24 hours installation ($50) \n3. within 5 days ($30) \n4. If yourinstallation exceeds 5 days (45$):";
		cin >> labour;

		if (labour != 1 && labour != 2 && labour != 3 && labour != 4)
		{
			cout << "Invalid entry!!" << endl;
		}

	} while (labour != 1 && labour != 2 && labour != 3 && labour != 4);


	do
	{
		cout << "\nNet metering: \n1. yes \n2. no:\n";
		cin >> netmeter;

		if (netmeter != 1 && netmeter != 2)
		{
			cout << "Invalid entry!!" << endl;
		}

	} while (netmeter != 1 && netmeter != 2);


	return 0;


}


void calculateROI(float energyPerDay, float totalCost)
{
	float tariffPerKWh; // Tariff per unit of electricity
	int years; // User-defined period for savings calculation

	// Get user inputs
	cout << "\nEnter the current electricity tariff (PKR per kWh): ";
	cin >> tariffPerKWh;

	cout << "Enter the number of years you want to calculate savings for: ";
	cin >> years;

	if (tariffPerKWh <= 0 || years <= 0)
	{
		cout << "Error: Tariff and years must be positive values.\n";
		return;
	}

	// Calculate daily and annual savings
	float dailySavings = energyPerDay * tariffPerKWh;
	float annualSavings = dailySavings * 365;

	// Calculate time to break even
	float breakEvenTime = totalCost / annualSavings;

	// Calculate total savings over the given period
	float totalSavings = annualSavings * years;

	// Display results
	cout << "\n========== ROI Calculation ==========\n";
	cout << "Daily Savings: PKR " << dailySavings << "/-\n";
	cout << "Annual Savings: PKR " << annualSavings << "/-\n";
	cout << "Time to Break Even: " << breakEvenTime << " years\n";
	cout << "Total Savings in " << years << " years: PKR " << totalSavings << "/-\n";
	cout << "=====================================\n";
}


// Function to load user data from a CSV file
void loadUsers()
{
	ifstream file("users.csv");
	if (file.is_open()) {
		userCount = 0;
		string line;
		while (getline(file, line)) {
			stringstream ss(line);
			string totalPriceStr;
			string Energystr;
			string No_PlatesStr;
			string platesCoststr;
			string inverterCoststr;
			string batterypowstr;
			string batterycoststr;
			string efstr;

			// Getting CSV line into user fields of structs:
			getline(ss, users[userCount].username, ',');
			getline(ss, users[userCount].cnic, ',');
			getline(ss, users[userCount].plates, ',');
			getline(ss, users[userCount].inverter, ',');
			getline(ss, users[userCount].meter, ',');
			getline(ss, users[userCount].house, ',');
			getline(ss, users[userCount].battery, ',');
			getline(ss, No_PlatesStr, ',');
			getline(ss, platesCoststr, ',');
			getline(ss, inverterCoststr, ',');
			getline(ss, batterycoststr, ',');
			getline(ss, batterypowstr, ',');
			getline(ss, Energystr, ',');
			getline(ss, totalPriceStr, ',');
			getline(ss, efstr, ',');

			// Convervion of string to int and float.
			users[userCount].No_Plates = stringToInt(No_PlatesStr);
			users[userCount].plates_cost = stringToInt(platesCoststr);
			users[userCount].inverter_cost = stringToInt(inverterCoststr);
			users[userCount].battery_cost = stringToInt(batterycoststr);
			users[userCount].battery_pow = stringToFloat(batterypowstr);
			users[userCount].Energy = stringToFloat(Energystr); 
			users[userCount].totalPrice = stringToInt(totalPriceStr);
			users[userCount].ef = stringToInt(efstr);
			userCount++;
		}
		file.close();
	}
	else
	{
		//cout << "There is some error with the file loading." << endl;
	}
}


// Function to save user data to CSV file
void saveUsers() {
	ofstream file("users.csv");
	if (!file.is_open()) {
		cout << "Error: Unable to open file for writing." << endl;
		return;
	}
	for (int i = 0; i < userCount; i++) {
		file << users[i].username << ","
			<< users[i].cnic << ","
			<< users[i].plates << ","
			<< users[i].inverter << ","
			<< users[i].meter << ","
			<< users[i].house << ","
			<< users[i].battery << ","
			<< users[i].No_Plates << ","
			<< users[i].plates_cost << ","
			<< users[i].inverter_cost << ","
			<< users[i].battery_cost << ","
			<< users[i].battery_pow << ","
			<< users[i].Energy << ","
			<< users[i].totalPrice << ","
			<< users[i].ef << endl;
	}
	file.close();
	//cout << "All users saved successfully!" << endl;
}


// Function to signup a new user
void signup() 
{
	resetGlobals();  // Reset before taking new input
	if (userCount >= MAX_USERS) {
		cout << "User limit reached. Cannot add more users.\n";
		return;
	}
	User newUser;
	cout << "Enter username: ";
	cin >> newUser.username;

	// Check if username already exists
	for (int i = 0; i < userCount; i++) {
		if (users[i].username == newUser.username) {
			cout << "Username already exists. Please choose another.\n";
			return;
		}
	}

	cout << "Enter cnic (Used as password!!): ";
	cin >> newUser.cnic;

	input();
	if (usage == 2)
	{
		price_of_plates = personal_plates();
		if (end1)
		{
			return;
		}
		if (b == 'n' || b == 'N')
			personal_inv();
		if (b == 'y' || b == 'Y')
		{
			personalBatteries();
			batteryInverters();
		}
		maintenance_cost = calculateMaintenanceCost(maintain1);
		labour_cost = calculateLabourCost(labour);
		net_metering_cost = calculateNetMeteringCost(netmeter);
		wiring_cost = calculateWiringCost(w_len);

	}
	else
	{
		price_of_plates = industrial_plates();
		if (end1)
		{
			return;
		}
		if (b == 'n' || b == 'N')
			industrial_inv();
		if (b == 'y' || b == 'Y')
		{
			industrialBatteries();
			batteryInverters();
		}
		maintenance_cost = calculateMaintenanceCost(maintain1);
		labour_cost = calculateLabourCost(labour);
		net_metering_cost = calculateNetMeteringCost(netmeter);
		wiring_cost = calculateWiringCost(w_len);

	}

	if (b == 'n' || b == 'N')
	{

		if (inv_q == 1)
		{
			inv_comp = "SMA Solar Technology.";
			inv_price *= 1.5;
		}
		else if (inv_q == 2)
		{
			inv_comp = "Enphase Energy.";
			inv_price *= 1.25;
		}
		else if (inv_q == 3)
		{
			inv_comp = "Growatt.";
			inv_price *= 1;
		}
	}
	
	total_price = inv_price + price_of_plates + maintenance_cost + labour_cost + net_metering_cost + wiring_cost + batteryCost;




	newUser.plates = plates_typ;
	newUser.inverter = inv_comp;
	newUser.meter = metertype;
	newUser.house = houseSize;
	newUser.battery = battery_typ;
	newUser.No_Plates = noPlates;
	newUser.plates_cost = price_of_plates;
	newUser.inverter_cost = inv_price;
	newUser.battery_cost = batteryCost;
	newUser.battery_pow = batterySizeAh;
	newUser.Energy = (energy_out / 365); // per day
	newUser.totalPrice = total_price;
	newUser.ef = e_p;

	// Add user 
	users[userCount++] = newUser;
	saveUsers();
	cout << "Signup successful!\n";
}


// Function to login a user
int login()
{
	string username, cnic;
	cout << "Enter username: ";
	cin >> username;
	cout << "Enter cnic: ";
	cin >> cnic;

	for (int i = 0; i < userCount; i++) {
		if (users[i].username == username && users[i].cnic == cnic) {
			cout << "Login successful!\n";
			return i; // Return the index of the logged-in user
		}
	}
	cout << "Invalid username or cnic.\n";
	return -1;
}


void viewAllProfiles() {
	if (userCount == 0) {
		cout << "No user profiles available.\n";
		return;
	}

	cout << "\n================= All User Profiles =================\n";
	for (int i = 0; i < userCount; i++) {
		cout << "\n========================================\n";
		cout << "         Profile " << i + 1 << "         \n";
		cout << "========================================\n";
		cout << "Username: " << users[i].username << endl;
		cout << "CNIC: " << users[i].cnic << endl;
		cout << "House Size: " << users[i].house << endl;
		cout << "Plates: " << users[i].plates << " (Efficiency of " << users[i].ef << "%)" << endl;
		cout << "Number of Plates: " << (users[i].No_Plates / 3) << " (Of 1.7 sq meter each)" << endl;
		cout << "Cost of Plates: PKR " << (users[i].plates_cost * 275) << "/-" << endl;
		cout << "Inverter: " << users[i].inverter << endl;
		cout << "Cost of Inverter: PKR " << (users[i].inverter_cost * 275) << "/-" << endl;
		cout << "Battery Type: " << users[i].battery << endl;
		cout << "Cost of Battery: PKR " << (users[i].battery_cost * 275) << "/-" << endl;
		cout << "Capacity of Battery (in Ah): " << users[i].battery_pow << endl;
		cout << "Metering Type: " << users[i].meter << endl;
		cout << "Energy (kW) per day: " << users[i].Energy << endl;
		cout << "Total Expenditures: PKR " << (users[i].totalPrice * 275) << "/-\n";
		cout << "========================================\n";
	}
	cout << "\t    ----------------------------------------\n";
}



// Function to view profile, The final output!!!
void viewProfile(int userIndex)
{
	if (userIndex == -1) {
		cout << "No user logged in.\n";
		return;
	}
	cout << "\n========================================\n";
	cout << "      " << users[userIndex].username << "'s Profile        \n";
	cout << "========================================\n";
	cout << "Username: " << users[userIndex].username << endl;
	cout << "CNIC: " << users[userIndex].cnic << endl;
	cout << "========================================\n";
	cout << "All data ahead is based on the appliances you entered.\nYou can change or udate them any time later!!\nTry to keep your plates neat and clean, You can also select our monthly or yearly Maintainance Services as per required.\n" << endl;
	cout << "Here are some link you can visit for more info: \n1. Infromation: https://en.wikipedia.org/wiki/Solar_panel \n2. Maintanance Info: https://www.repsol.com/en/energy-and-the-future/technology-and-innovation/solar-panel-maintenance/index.cshtml" << endl;
	cout << "============    User Data   ============\n";
	cout << "House Size: " << users[userIndex].house << endl;
	cout << "Plates: " << users[userIndex].plates << " (Efficiency of  " << users[userIndex].ef << "%)" << endl;
	cout << "Number of Plates: " << (users[userIndex].No_Plates / 3) << " (Of 1.7 sq meter each)" << endl;
	cout << "Cost of Plates: PKR " << (users[userIndex].plates_cost*275) << "/-" << endl;
	cout << "Inverter: " << users[userIndex].inverter << endl;
	cout << "Cost of Inverter: PKR " << (users[userIndex].inverter_cost*275) << "/-" << endl;
	cout << "Battery Type:" << (users[userIndex].battery) << endl;
	cout << "Cost of Battery: PKR " << (users[userIndex].battery_cost*275) << "/-" << endl;
	cout << "Capacity of battery (in Ah): " << (users[userIndex].battery_pow) << endl;
	cout << "Metering type: " << users[userIndex].meter << endl;
	cout << "Energy (kW) per day: " << users[userIndex].Energy << endl;
	cout << "Total Expendetures: $" << users[userIndex].totalPrice << endl;
	cout << "Total Expendetures: PKR " << (users[userIndex].totalPrice * 275) << "/-" << endl;
	cout << "==========================================================================================\n";
	cout << "Note: This is the final price after adding all kinds of expense you would require, enjoy!!\n" << endl;
	cout << "==========================================================================================\n";

}


// Function to update profile
void updateProfile(int userIndex) 
{
	resetGlobals();  // Reset before taking new input
	if (userIndex == -1) {
		cout << "No user logged in.\n";
		return;
	}

	input();
	if (usage == 2)
	{
		price_of_plates = personal_plates();
		if (end1)
		{
			return;
		}
		if (b == 'n' || b == 'N')
			personal_inv();
		if (b == 'y' || b == 'Y')
		{
			personalBatteries();
			batteryInverters();
		}
		maintenance_cost = calculateMaintenanceCost(maintain1);
		labour_cost = calculateLabourCost(labour);
		net_metering_cost = calculateNetMeteringCost(netmeter);
		wiring_cost = calculateWiringCost(w_len);

	}
	else
	{
		price_of_plates = industrial_plates();
		if (end1)
		{
			return;
		}
		if (b == 'n' || b == 'N')
			industrial_inv();
		if (b == 'y' || b == 'Y')
		{
			industrialBatteries();
			batteryInverters();
		}
		maintenance_cost = calculateMaintenanceCost(maintain1);
		labour_cost = calculateLabourCost(labour);
		net_metering_cost = calculateNetMeteringCost(netmeter);
		wiring_cost = calculateWiringCost(w_len);

	}

	if (b == 'n' || b == 'N')
	{

		if (inv_q == 1)
		{
			inv_comp = "SMA Solar Technology.";
			inv_price *= 1.5;
		}
		else if (inv_q == 2)
		{
			inv_comp = "Enphase Energy.";
			inv_price *= 1.25;
		}
		else if (inv_q == 3)
		{
			inv_comp = "Growatt.";
			inv_price *= 1;
		}
	}
	total_price = inv_price + price_of_plates + maintenance_cost + labour_cost + net_metering_cost + wiring_cost + batteryCost;


	// Update user profile in the users array
	users[userIndex].plates = plates_typ;
	users[userIndex].inverter = inv_comp;
	users[userIndex].meter = metertype;
	users[userIndex].house = houseSize;
	users[userIndex].battery = battery_typ;
	users[userIndex].No_Plates = noPlates;
	users[userIndex].plates_cost = price_of_plates;
	users[userIndex].inverter_cost = inv_price;
	users[userIndex].battery_cost = batteryCost;
	users[userIndex].battery_pow = batterySizeAh;
	users[userIndex].Energy = (energy_out / 365); 
	users[userIndex].totalPrice = total_price;
	users[userIndex].ef = (e_p); 

	saveUsers();
	cout << "Profile updated successfully!\n";
}


// Function to delete a user profile
void deleteProfile(int userIndex)
{
	if (userIndex == -1) {
		cout << "No user logged in.\n";
		return;
	}

	// Confirm deletion
	char confirmation;
	cout << "Are you sure you want to delete your profile? (y/n): ";
	cin >> confirmation;

	if (confirmation == 'y' || confirmation == 'Y') {
		// Shift all users after the current index to fill the gap
		for (int i = userIndex; i < userCount - 1; i++) {
			users[i] = users[i + 1];
		}
		userCount--; // Decrease the total user count
		saveUsers(); // Save the updated user list to the file

		cout << "Profile deleted successfully!\n";
	}
	else {
		cout << "Profile deletion canceled.\n";
		return;
	}
}


// Main menu
void displayMenu() 
{
	
	cout << "\n\t     ==== Please Chose an option ====\n";
	cout << "1. Signup\n";
	cout << "2. Login\n";
	cout << "3. View Profile\n";
	cout << "4. Update Profile\n";
	cout << "5. Delete Profile\n";
	cout << "6. Calculate ROI (Return On Investment)\n"; 
	cout << "7. View All Profiles\n";
	cout << "8. View Instructios\n";
	cout << "9. Exit\n";
	cout << "\t      ===============================\n";
	//cout << "Enter your choice: ";
	//system("pause");
}


int main()
{
	cout << "\n     ==== \"Solar Management and Pricement Co.Ltd\" ====\n";
	cout << R"(
     //********************************************\\
    //                    HELLO!!                   \\
   //                                                \\
   \\	"WELCOME TO YOUR OWN SOLAR SYSTEM PLANNER"   // 
    \\                                              //
     \\********************************************//
   )" << endl;
	loadUsers();
	int loggedInUserIndex = -1;

	while (true) {
		int choice;
		displayMenu();
		do {
			cout << "Enter your choice: ";
			if (!(cin >> choice)) {
				// Clear the error state
				cin.clear();
				// Discard invalid input
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Invalid Entry!!\nEnter a number between 1 and 9: " << endl;
			}
			else if (choice < 1 || choice > 9) {
				cout << "Invalid Entry!!\nEnter a number between 1 and 9: " << endl;
			}
		} while (choice < 1 || choice > 9);
		
		

		switch (choice)
		{
		case 1:
			signup();
			break;
		case 2:
			loggedInUserIndex = login();
			break;
		case 3:
			viewProfile(loggedInUserIndex);
			break;
		case 4:
			updateProfile(loggedInUserIndex);
			break;
		case 5:
			deleteProfile(loggedInUserIndex);
			loggedInUserIndex = -1;
		case 6:
			if (loggedInUserIndex == -1)
			{
				cout << "Error: You are loged out!!\n";
			}
			else
			{
				calculateROI((users[loggedInUserIndex].Energy /10), (users[loggedInUserIndex].totalPrice*275));
			}
			break;
		case 7:
			viewAllProfiles();
			break;
		case 8:
			viewInstructions();
			break;
		case 9:
			cout << R"(
     //********************************************\\
    //            THANK YOU FOR VISITING!!          \\
   //                                                \\
   \\	                <<BYE>>                      // 
    \\                                              //
     \\********************************************//
   )" << endl;			
			saveUsers();
			return 0;
		default:
			cout << "Invalid choice. Please try again.\n";
			break;
		}
	system("pause");
	}

	//Output:
	/*cout << "Price of plates = " << price_of_plates << endl;
	cout << "Inverter price = " << inv_price << endl;
	cout << "Inverter company " << inv_comp << endl;
	cout << "Total price = " << total_price << endl;
	cout << "Energy in kW = " << energy_out << endl;*/

}