#include "DecideVelocityCanditate.h"

void DecideVelocityCanditate::decideVelocity() {
	car.canditateVelocity = car.velocity.current;
	maximumSumV = std::vector<int>(LaneNum, 0);
	for (int i = 0; i < N; i++) {
		car.S[i] = constants.S;
		_rule1(i);
		_rule2(i);
		_rule3(i);
		_rule4(i);
		maximumSumV[car.laneNumber.current[i]] += car.canditateVelocity[i];
	}
}

void DecideVelocityCanditate::_rule1(int ID) {
	int front = car.arround[ID].front;
	if (car.distance.current[ID] >= constants.G || car.canditateVelocity[ID] <= car.velocity.current[front]) {
		if (car.canditateVelocity[ID] < constants.Vmax[ID]) {
			car.canditateVelocity[ID]++;
		}
	}
}

void DecideVelocityCanditate::_rule2(int ID) {
	if (random->random(1.0) <= constants.q) {
		if (random->random(1.0) >= constants.r) car.S[ID] = 1;
		int s = 0;
		int id = car.arround[ID].front;
		int position = car.position.previous[ID];
		int si_position;
		int gap = 0;
		while (s < car.S[ID]) {
			si_position = car.position.previous[id];
			if (si_position > position) {
				gap += si_position - position;
			}
			else {
				gap += si_position - position + L;
			}
			position = si_position;
			id = car.arround[id].front;
			s++;
		}
		int canditate_v2 = gap - car.S[ID];
		if (canditate_v2 < car.canditateVelocity[ID]) {
			car.canditateVelocity[ID] = canditate_v2;
		}
	}
}

void DecideVelocityCanditate::_rule3(int ID) {
	int s = 0;
	int id = car.arround[ID].front;
	int position = car.position.current[ID];
	int si_position;
	int gap = 0;
	while (s < car.S[ID]) {
		si_position = car.position.current[id];
		if (si_position > position) {
			gap += si_position - position;
		}
		else {
			gap += si_position - position + L;
		}
		position = si_position;
		id = car.arround[id].front;
		s++;
	}
	int canditate_v3 = gap - car.S[ID];
	if (canditate_v3 < car.canditateVelocity[ID]) {
		car.canditateVelocity[ID] = canditate_v3;
	}
}

void DecideVelocityCanditate::_rule4(int ID) {
	double p;
	int front = car.arround[ID].front;
	if (constants.G <= car.distance.current[ID]) {
		p = constants.p1;
	}
	else {
		if (car.velocity.current[ID] < car.velocity.current[front]) {
			p = constants.p2;
		}
		else if (car.velocity.current[ID] == car.velocity.current[front]) {
			p = constants.p3;
		}
		else {
			p = constants.p4;
		}
	}
	if (random->random(1.0) < 1 - p) {
		int canditate_v4 = car.canditateVelocity[ID] - 1;
		if (1 < canditate_v4) {
			car.canditateVelocity[ID] = canditate_v4;
		}
		else {
			car.canditateVelocity[ID] = 1;
		}
	}
}
