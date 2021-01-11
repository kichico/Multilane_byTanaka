#include "InisializeSetting.h"

void InisializeSetting::initialize() {
	car.initialize(N);
	map.initialize(LaneNum, L);
	information.initilaize(LaneNum);
	_setConstantValues();
	_setStrategy();
	_setInitializePosition();
	_assignVmax();
}

void InisializeSetting::_setConstantValues() {
	constants.per_cell = 5.6;
	constants.G = 15;
	constants.S = 2;
	constants.r = 0.99;
	constants.q = 0.99;
	constants.p1 = 0.99;
	constants.p2 = 0.99;
	constants.p3 = 0.98;
	constants.p4 = 0.01;
}

void InisializeSetting::_setStrategy() {
	std::vector<int> DCars(N);
	for (int i = 0; i < N; i++) DCars[i] = i;
	for (int i = 0; i < CNum; i++) {
		int rem_size = DCars.size() - 1;
		int xrem = random->random(rem_size);
		int ID = DCars[xrem];
		std::iter_swap(DCars.begin() + xrem, DCars.end() - 1);
		DCars.pop_back();
		car.strategy[ID] = Car::StrategyKind::C;
	}
	for (int i = 0; i < DCars.size(); i++) car.strategy[DCars[i]] = Car::StrategyKind::D;
	information.DCars = DCars;
}

void InisializeSetting::_setInitializePosition() {
	std::vector<int> rem(LaneNum * L);
	for (int i = 0; i < rem.size(); i++) rem[i] = i;
	for (int i = 0; i < N; i++) {
		int rem_size = rem.size() - 1;
		int xrem = random->random(rem_size);
		int p = rem[xrem];
		int laneNum = p / L;
		int position = p % L;
		std::iter_swap(rem.begin() + xrem, rem.end() - 1);
		rem.pop_back();
		map.existance[laneNum][position] = true;
		map.CarID[laneNum][position] = i;
		car.laneNumber.current[i] = car.laneNumber.previous[i] = laneNum;
		car.position.current[i] = car.position.previous[i] = position;
		information.eachLaneCarNum[laneNum]++;
		if (!information.leadCar[laneNum].existence) {
			information.leadCar[laneNum].existence = true;
			information.leadCar[laneNum].ID = i;
		}
	}
	for (int i = 0; i < information.leadCar.size(); i++) {
		if (information.leadCar[i].existence) {
			int ID = information.leadCar[i].ID;
			int startPosition = car.position.current[ID];
			int position = startPosition - 1;
			int maxDistance = 0;
			int frontID = ID;
			int distance = 0;
			while (true) {
				if (position < 0) position += L;
				if (map.existance[i][position]) {
					int rearID = map.CarID[i][position];
					car.arround[ID].front = frontID;
					car.arround[ID].rear = rearID;
					car.distance.current[ID] = car.distance.previous[ID] = distance;
					car.arround[rearID].front = ID;
					if (distance > maxDistance) {
						maxDistance = distance;
						information.leadCar[i].ID = ID;
					}
					frontID = ID;
					ID = rearID;
					distance = 0;
					if (position == startPosition) break;
				}
				else distance++;
				position--;
			}
		}
	}
}

void InisializeSetting::_assignVmax() {
	constants.Vmax = std::vector<int>(N);
	Listofcar_lanechangeable = std::vector<int>(0);
	Listofcar_notallowedtolanechange = std::vector<int>(0);
	int NumofOverspeeder = 0;
	int NumofOrdinaryspeeder = 0;
	int Numofslower = 0;
	int NumofDstrategy = information.DCars.size();
	bool flg_distribute = true;
	bool flg_correlated = true;
	std::vector<int> assign(N, 0);
	if (flg_correlated == false && flg_distribute == false) for (int i = 0; i < N; ++i) constants.Vmax[i] = 5;
	if (flg_distribute == true && flg_correlated == false) {
		for (int i = 0; i < N; ++i) {
			if (i % 3 == 0) {
				assign[i] = 4;
				++Numofslower;
			}
			if (i % 3 == 1) {
				assign[i] = 5;
				++NumofOrdinaryspeeder;
			}
			if (i % 3 == 2) {
				assign[i] = 6;
				++NumofOverspeeder;
			}
			if (car.strategy[i] == Car::StrategyKind::C) Listofcar_notallowedtolanechange.push_back(i);
			if (car.strategy[i] == Car::StrategyKind::D) Listofcar_lanechangeable.push_back(i);
		}
		std::vector<int> rem(N);
		for (int i = 0; i < rem.size(); i++) rem[i] = i;
		for (int i = 0; i < N; i++) {
			int rem_size = rem.size() - 1;
			int xrem = random->random(rem_size);
			int pickedupvelocity = rem[xrem];
			std::iter_swap(rem.begin() + xrem, rem.end() - 1);
			rem.pop_back();
			constants.Vmax[i] = assign[pickedupvelocity];
		}
	}
	if (flg_correlated == true && flg_distribute == true) {
		for (int i = 0; i < N; ++i) {
			if (car.strategy[i] == Car::StrategyKind::C) Listofcar_notallowedtolanechange.push_back(i);
			if (car.strategy[i] == Car::StrategyKind::D) Listofcar_lanechangeable.push_back(i);
		}
		int cnt_assigned = 0;
		for (int i = 0; i < NumofDstrategy; ++i) {
			constants.Vmax[Listofcar_lanechangeable[i]] = 6;
			if (cnt_assigned > NumofOverspeeder) constants.Vmax[Listofcar_lanechangeable[i]] = 5;
			if (cnt_assigned > NumofOrdinaryspeeder + NumofOverspeeder) constants.Vmax[Listofcar_lanechangeable[i]] = 4;
			++cnt_assigned;
		}
		for (int i = 0; i < N - NumofDstrategy; ++i) {
			if (cnt_assigned > NumofOrdinaryspeeder + NumofOverspeeder) {
				constants.Vmax[Listofcar_notallowedtolanechange[i]] = 4;
				++cnt_assigned;
			}
			else if (cnt_assigned > NumofOverspeeder) {
				constants.Vmax[Listofcar_notallowedtolanechange[i]] = 5;
				++cnt_assigned;
			}
			else {
				constants.Vmax[Listofcar_notallowedtolanechange[i]] = 6;
				++cnt_assigned;
			}
		}
	}
}