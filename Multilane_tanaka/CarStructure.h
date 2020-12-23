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
	struct SignalKind {
		static const int Non = 0;
		static const int Right = 1;
		static const int Left = 2;
	};
	struct StrategyKind {
		static const int C = 0;
		static const int D = 1;
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
	std::vector<int> signal;
	std::vector<int> strategy;
	void initialize(int N);
	void copyCurrentToPrevious();
};

#endif // !CARSTRUCTURE_H

