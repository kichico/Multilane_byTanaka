#ifndef CARSTRUCTURE_H
#define CARSTRUCTURE_H
#include <vector>

class Car {
private:
	struct CurrentPrevious {
		std::vector<int> current;
		std::vector<int> previous;
	};
public:
	enum SignalKind {
		Non, Right, Left
	};
	enum StrategyKind {
		C, D
	};
	struct FrontRearCar {
		int front;
		int rear;
	};
	struct Mesurement {
		bool merured;
		double elapsed;
	};
	CurrentPrevious velocity;
	CurrentPrevious position;
	CurrentPrevious distance;
	CurrentPrevious laneNumber;
	std::vector<int> S;
	std::vector<int> canditateVelocity;
	std::vector<FrontRearCar> arround;
	std::vector<Mesurement> mesure;
	std::vector<SignalKind> signal;
	std::vector<StrategyKind> strategy;
	void initialize(int N);
	void copyCurrentToPrevious();
};

#endif // !CARSTRUCTURE_H

