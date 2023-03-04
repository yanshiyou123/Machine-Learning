#include <iostream>
#include <String>
#include <fstream>
#include <ostream>
#include <vector>
#include <sstream>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <ctime>
using namespace std;
void logistic(double Sex[],double w[],double logis[]);
int main(int argc, char** argv) {
	ifstream inFS;
	string line;
	string blank_in, pclass_in, survived_in, sex_in, age_in;
	const int MAX_LEN = 2000;
	vector<double> survived(MAX_LEN);
	vector<double> sex(MAX_LEN);
	vector<double> error;
	cout << "Opening file titanic_project.csv." << endl;
	inFS.open("titanic_project.csv");
	if (!inFS.is_open()) {
		cout << "Could not open file titanic_project.csv" << endl;
		return 1;
	}

	cout << "Reading line 1:" << endl;
	getline(inFS, line);
	cout << "Heading: " << line << endl;
	
	int numObs = 0;
	while (inFS.good()) {
		getline(inFS, blank_in, ',');
		getline(inFS, pclass_in, ',');
		getline(inFS, survived_in, ',');
		getline(inFS, sex_in, ',');
		getline(inFS, age_in, '\n');
		
		survived.at(numObs) = stof(survived_in);
		sex.at(numObs) = stof(sex_in);
		

		numObs++;
	}
	survived.resize(numObs);
	sex.resize(numObs);
	//Train and test
	double surv_train[800];
	double sex_train[800];
	
	for (int i = 0; i < 800; i++) {
		surv_train[i] = survived.at(i);
		sex_train[i] = sex.at(i);
	}
	double err[800];
	double logis[800];
	double predict[800];
	double e = 2.718281828;
	double rate = 0.01;
	double w[2] = { 0,0 };
	double g[2] = { 0,0 };
	//Time
	chrono::time_point<chrono::system_clock> start, end;
	start = chrono::system_clock::now();
	//10 epochs
	for (int i = 0; i < 10; i++) {
		logistic(sex_train, w, logis);
		for (int j = 0; j < 800; j++) {
			err[j] = surv_train[j] - logis[j];
		}
		
		for (int k = 0; k < 800; k++) {
			g[0] += err[k];
			g[1] += (sex_train[k] * err[k]);
		}
		for (int p = 0; p < 2; p++) {
			w[p] += (rate * g[p]);
			//cofficients
			cout << "cofficients: " << w[p] << endl;
		}
		
	}
	
	
	double surv_test[246];
	double sex_test[246];
	//predict
	for (int i = 800; i < 1046; i++) {
		surv_test[i - 800] = survived.at(i);
		sex_test[i - 800] = sex.at(i);
	}
	
	double prob = 0;
	double TP = 0, TN = 0, FP = 0, FN = 0;
	for (int i = 0; i < 246; i++) {
		prob = (1 / (1 + pow(e, ((-1) * (w[0] + (w[1] * sex_test[i]))))));
		if (prob < 0.5) {
			predict[i] = 0;
		}	
		else {
			predict[i] = 1;
		}
	}
	for (int i = 0; i < 246; i++) {
		if (predict[i] == 0 && surv_test[i] == 0) {
			TP++;
		}
		if (predict[i] == 1 && surv_test[i] == 1) {
			TN++;
		}
		if (predict[i] == 0 && surv_test[i] == 1) {
			FP++;
		}
		if (predict[i] == 1 && surv_test[i] == 0) {
			FN++;
		}
	}
	//accuracy, sensitivity, specificity
	double accuracy, sensitivity, specificity;
	accuracy = (TP + TN) / (TP + TN + FP + FN);
	cout << "accuracy: " << accuracy << endl;
	sensitivity = TP / (TP + FN);
	cout << "sensitivity: " << sensitivity << endl;
	specificity = TN / (TN + FP);
	cout << "specificity: " << specificity << endl;

	//Time
	end = chrono::system_clock::now();
	chrono::duration<double> totalsec = end - start;
	cout << "RunTime: " << totalsec.count() << endl;
	inFS.close();
	return 0;
}

void logistic(double sex[], double w[], double logis[]) {
	double a0, a1;
	double e = 2.718281828;
	for (int i = 0; i < 800; i++) {
		a0 = w[0];
		a1 = sex[i] * w[1];
		//sigmoid function
		logis[i] = 1.0 / (1.0 + pow(e, (-1 * (a0 + a1))));
	}
}