#include "OneLaneRevised_S_NFS.h"

OneLaneRevised_S_NFS::OneLaneRevised_S_NFS(int filenumber,int LaneNum, int L) {
	this->filenumber = filenumber;
	this->LaneNum = LaneNum;
	this->L = L;
}

void OneLaneRevised_S_NFS::calculate() {
	int C_iter = 100;
	double Crate = C_iter*0.01;
	std::string f1 = "Result/Local" + to_string(filenumber) + "_Crate_" + to_string(C_iter) + ".dat";
	std::string f2 = "Result/Global" + to_string(filenumber) + "_Crate_" + to_string(C_iter) + ".dat";
	const char* F1 = f1.c_str();
	const char* F2 = f2.c_str();
	std::ifstream ifs(F2);
	std::vector<bool> NFinished((L * LaneNum) / 75, false);
	if (ifs) {
		std::string SLine, S;
		while (std::getline(ifs, SLine)) {
			std::stringstream SS;
			SS << SLine;
			std::getline(SS, S, ',');
			int Val = (stoi(S)) / 10 - 1;
			NFinished[Val] = true;
		}
	}
	ifs.close();
	std::ofstream ofs1(F1, std::ios::app);
	std::ofstream ofs2(F2, std::ios::app);
	std::vector<int> NLists;
	//for (int i = 0; i < NFinished.size(); i++) if (!NFinished[i]) NLists.push_back((i + 1) * 75);
//#ifdef _OPENMP
//#pragma omp parallel for
//#endif
	for (int i = 0; i < 10; i++) {
		int N = 900;
		double rho = 1.0 * N / (L * LaneNum);
		UpdatePosition* DoCal = new UpdatePosition(LaneNum, L, N, (int)round(Crate * N), filenumber);
		UpdatePosition::NextInfomrations nextInfos;
		DoCal->initialize();
		for (int i = 0; i < 1800; i++) {
			DoCal->decideVelocity();
			DoCal->insentiveCheck();
			DoCal->car.copyCurrentToPrevious();
			nextInfos = DoCal->updatePosition(false);
			if (DoCal->laneChange()) nextInfos = DoCal->updatePosition(false);
		}
		for (int i = 0; i < 300; i++) {
			DoCal->decideVelocity();
			DoCal->insentiveCheck();
			DoCal->car.copyCurrentToPrevious();
			nextInfos = DoCal->updatePosition(true);
			if (DoCal->laneChange()) nextInfos = DoCal->updatePosition(true);
			DoCal->car.mesure = nextInfos.mesure;
			DoCal->information.eachLaneResults = nextInfos.eachLaneResults;
			DoCal->information.allResults = nextInfos.allResults;
		}
		DoCal->information.ave_C /= (300 * ((double)N - (double)DoCal->Listofcar_lanechangeable.size()));
		DoCal->information.ave_D /= (300 * (double)DoCal->Listofcar_lanechangeable.size());
		std::cout << "Number of lanechangers=>" << DoCal->information.DCars.size() << std::endl;
		std::string forCout, forOfs2;
		std::vector<std::string> forOfs1(LaneNum);
		forCout = to_string(rho);
		forOfs2 = to_string(N) + "," + to_string(rho);
		double flux = 0;
		for (int i = 0; i < LaneNum; i++) {
			double av, k;
			int Q = DoCal->information.eachLaneResults[i].CountNum - 1;
			if (Q > 1) {
				av = DoCal->information.eachLaneResults[i].AV / DoCal->information.eachLaneResults[i].CountNum;
				if (av > 0)	k = 1000 / 300 * Q / av;
				else k = 0;
			}
			else {
				Q = 0;
				av = k = 0;
			}
			flux += Q;
			forCout += ", " + to_string(Q);
			forOfs1[i] = to_string(k) + "," + to_string(Q);
			//forOfs2 += "," + to_string(Q);
		}
		flux /= 3;
		forOfs2 += "," + to_string(flux);
		forOfs2 += "," + to_string(DoCal->information.ave_C) + "," + to_string(DoCal->information.ave_D);
		forCout += ", " + to_string(DoCal->information.ave_C) + ", " + to_string(DoCal->information.ave_D);
#ifdef _OPENMP
#pragma omp critical
#endif
		{
			std::cout << forCout << std::endl;
			for (int i = 0; i < forOfs1.size(); i++) ofs1 << forOfs1[i] << std::endl;
			ofs2 << forOfs2 << std::endl;
		}
		delete DoCal;
	}
	ofs1.close();
	ofs2.close();
}

