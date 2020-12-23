#ifndef INISIALIZESETTING_H
#define INISIALIZESETTING_H
#include <iostream>
#include <fstream>
#include "CarStructure.h"
#include "MapInformation.h"
#include "Information.h"
#include "random.h"

class InisializeSetting {
private:
	struct ConstantValues {
		double per_cell;
		std::vector<int> Vmax;
		int G;
		int S;
		double r;
		double q;
		double p1;
		double p2;
		double p3;
		double p4;
	};

public:
	int N;
	int LaneNum;
	int L;
	int CNum;
	Random *random;
	ConstantValues constants;
	Car car;
	MapInformation map;
	Information information;
	std::vector<int> Listofcar_lanechangeable;
	std::vector<int> Listofcar_notallowedtolanechange;
	void initialize();
private:
	void _setConstantValues();
	void _setStrategy();
	void _setInitializePosition();
	void _assignVmax();
};

#endif // !INISIALIZESETTING_H

