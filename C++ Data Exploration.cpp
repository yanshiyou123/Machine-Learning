#include <iostream>
#include <String>
#include <fstream>
#include <ostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>
using namespace std;
void print_stats_RM(vector<double>);
void print_stats_Medv(vector<double>);
void covar(vector<double>, vector<double>);
void cor(vector<double>, vector<double>);

int main(int argc, char** argv) {
	ifstream inFS;
	string line;
	string rm_in, medv_in;
	const int MAX_LEN = 1000;
	vector<double> rm(MAX_LEN);
	vector<double> medv(MAX_LEN);
	cout << "Opening file Boston.csv." << endl;
	inFS.open("Boston.csv");
	if (!inFS.is_open()) {
		cout << "Could not open file Boston.csv" << endl;
		return 1;
	}
	cout << "Reading line 1:" << endl;
	getline(inFS, line);
	cout << "Heading: " << line << endl;

	int numObs = 0;
	while (inFS.good()) {
		getline(inFS, rm_in, ',');
		getline(inFS, medv_in, '\n');

		rm.at(numObs) = stof(rm_in);
		medv.at(numObs) = stof(medv_in);

		numObs++;
	}
	rm.resize(numObs);
	medv.resize(numObs);

	cout << "rm New Length: " << rm.size() << endl;
	cout << "medv New Length: " << medv.size() << endl;

	inFS.close();

	cout << "Number of records: " << numObs << endl;
	cout << "Stats for rm: " << endl;
	print_stats_RM(rm);
	cout << "Stats for medv: " << endl;
	print_stats_Medv(medv);

	covar(rm, medv);
	cor(rm, medv);
	return 0;
}

void print_stats_RM(vector<double> data) {
	double sum = accumulate(data.begin(), data.end(), 0.0);
	double average = accumulate(data.begin(), data.end(), 0.0) / data.size();
	double min = *min_element(data.begin(), data.end());
	double max = *max_element(data.begin(), data.end());
	double median = (data[data.size() / 2] + data[data.size() / 2 - 1]) / 2;
	cout << "Sum of a numeric vector: " << endl;
	cout << sum << endl;
	cout << "Mean of a numeric vector: " << endl;
	cout << average << endl;
	cout << "Median of a numeric vector: " << endl;
	sort(data.begin(), data.end());
	cout << median << endl;
	cout << "Range of a numeric vector: " << endl;
	cout << "Range is: " << min << " to " << max << endl;
}
void print_stats_Medv(vector<double> data) {
	double sum = accumulate(data.begin(), data.end(), 0.0);
	double average = accumulate(data.begin(), data.end(), 0.0) / data.size();
	double min = *min_element(data.begin(), data.end());
	double max = *max_element(data.begin(), data.end());
	double median = (data[data.size() / 2] + data[data.size() / 2 - 1]) / 2;
	cout << "Sum of a numeric vector: " << endl;
	cout << sum << endl;
	cout << "Mean of a numeric vector: " << endl;
	cout << average << endl;
	cout << "Median of a numeric vector: " << endl;
	sort(data.begin(), data.end());
	cout << median << endl;
	cout << "Range of a numeric vector: " << endl;
	cout << "Range is: " << min << " to " << max << endl;
}
void covar(vector<double>data1, vector<double>data2) {
	double average1 = accumulate(data1.begin(), data1.end(), 0.0) / data1.size();
	double average2 = accumulate(data2.begin(), data2.end(), 0.0) / data2.size();
	double sum = 0;
	for (int i = 0; i < data1.size() && i < data2.size(); i++) {
		sum += (data1[i] - average1) * (data2[i] - average2);
	}
	double covariance = sum / (data1.size() - 1);
	cout << "The covariance between rm and medv: " << endl;
	cout << covariance << endl;

}
void cor(vector<double>data1, vector<double>data2) {
	double average1 = accumulate(data1.begin(), data1.end(), 0.0) / data1.size();
	double average2 = accumulate(data2.begin(), data2.end(), 0.0) / data2.size();
	double sum = 0;
	double x = 0;
	double y = 0;
	for (int i = 0; i < data1.size() && i < data2.size(); i++) {
		sum += (data1[i] - average1) * (data2[i] - average2);
		x += pow((data1[i] - average1), 2);
		y += pow((data2[i] - average2), 2);
	}
	double correlation = sum / sqrt(x * y);
	cout << "The correlation between rm and medv: " << endl;
	cout << correlation;
}
