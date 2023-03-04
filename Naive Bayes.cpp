#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <string>
#include <stdio.h>
#include <cstdio>
#include <chrono>

using namespace std;

//The author is Preston
//creating global arrays so I can use them from my functions and main without having to send them back and forth
double pClassArray[2][3];
double sexArray[2][2];
double ageMean[2];
double ageVar[2];
double priorArray[2];
double rawArray[246][2];

//This function takes in a vector and will return its sum 
double sumOfNumericVector(vector<double> aNumericVector) {
	double sum = 0;
	for (int i = 0; i < aNumericVector.size(); i++) {
		sum += aNumericVector[i];
	}

	return sum;
}
//This function takes in a vector and its size and will return its mean
double meanOfNumericVector(vector<double> aNumericVector, int numberOfObservations) {
	double sum = sumOfNumericVector(aNumericVector);
	double mean = sum / numberOfObservations;
	return mean;
}
//This function takes in a vector and its size and will return its standard deviation
double standardDeviationVector(vector<double> aNumericVector, int numberOfObservations) {
	double stdDeviation = 0;
	double numerator = 0;

	for (int i = 0; i < aNumericVector.size(); i++) {
		numerator = aNumericVector[i] - meanOfNumericVector(aNumericVector, numberOfObservations);
		numerator = numerator * numerator;
		stdDeviation += numerator;
	}
	
	stdDeviation = stdDeviation / numberOfObservations;
	stdDeviation = sqrt(stdDeviation);

	return stdDeviation;
}
//This function takes in a vector and its size and will update the global prior array to the vectors probability
void apriori(vector<double> surviveVector, int numberOfObservations) {
	double counter = 0;
	for (int i = 0; i < surviveVector.size(); i++) {
		if (surviveVector[i] == 1) {
			counter++;
		}
	}
	counter = counter / numberOfObservations;

	priorArray[0] = 1 - counter;
	priorArray[1] = counter;
	return;
}
//This function takes in two vectors and their size, updates the global class array with their probability
void lh_pclass(vector<double> classVector, vector<double> surviveVector, int numberOfObservations) {
	double survivedClassOne = 0;
	double survivedClassTwo = 0;
	double survivedClassThree = 0;
	double perishedClassOne = 0;
	double perishedClassTwo = 0;
	double perishedClassThree = 0;
	
	for (int i = 0; i < classVector.size(); i++) {
		if (surviveVector[i] == 1) {
			if (classVector[i] == 1) {
				survivedClassOne++;
			}
			else if (classVector[i] == 2) {
				survivedClassTwo++;
			}
			else if (classVector[i] == 3) {
				survivedClassThree++;
			}
		}
		else if (surviveVector[i] == 0) {
			if (classVector[i] == 1) {
				perishedClassOne++;
			}
			else if (classVector[i] == 2) {
				perishedClassTwo++;
			}
			else if (classVector[i] == 3) {
				perishedClassThree++;
			}
		}
	}

	double totalSurvived = survivedClassOne + survivedClassTwo + survivedClassThree;
	double probClassOne = survivedClassOne / totalSurvived;
	double probClassTwo = survivedClassTwo / totalSurvived;
	double probClassThree = survivedClassThree / totalSurvived;

	double totalPerished = surviveVector.size() - totalSurvived;
	double probPerishedClassOne = perishedClassOne / totalPerished;
	double probPerishedClassTwo = perishedClassTwo / totalPerished;
	double probPerishedClassThree = perishedClassThree / totalPerished;
	
	pClassArray[0][0] = probPerishedClassOne;
	pClassArray[0][1] = probPerishedClassTwo;
	pClassArray[0][2] = probPerishedClassThree;
	pClassArray[1][0] = probClassOne;
	pClassArray[1][1] = probClassTwo;
	pClassArray[1][2] = probClassThree;

	return;
}
//This function takes in two vectors and their size, updates the global sex array with their probability
void lh_sex(vector<double> sexVector, vector<double> surviveVector, int numberOfObservations) {
	double survivedMale = 0;
	double survivedFemale = 0;
	double perishedMale = 0;
	double perishedFemale = 0;

	for (int i = 0; i < sexVector.size(); i++) {
		if (surviveVector[i] == 1) {
			if (sexVector[i] == 1) {
				survivedMale++;
			}
			else if (sexVector[i] == 0) {
				survivedFemale++;
			}
		}
		else if (surviveVector[i] == 0) {
			if (sexVector[i] == 1) {
				perishedMale++;
			}
			else if (sexVector[i] == 0) {
				perishedFemale++;
			}
		}
	}
	double totalSurvived = survivedMale + survivedFemale;
	double totalPerished = surviveVector.size() - totalSurvived;
	double probMaleSurvived = survivedMale / totalSurvived;
	double probFemaleSurvived = survivedFemale / totalSurvived;
	double probMalePerished = perishedMale / totalPerished;
	double probFemalePerished = perishedFemale / totalPerished;
	sexArray[0][1] = probMalePerished;
	sexArray[0][0] = probFemalePerished;
	sexArray[1][1] = probMaleSurvived;
	sexArray[1][0] = probFemaleSurvived;

	return;
}
//This function takes in two vectors and their size, updates the global mean array
void age_mean(vector<double> ageVector, vector<double> surviveVector, int numberOfObservations) {
	double sumSurviveAge = 0;
	double sumPerishedAge = 0;
	double counter = 0;
	double counter2 = 0;
	double survivedMean = 0;
	double perishedMean = 0;
	

	for (int i = 0; i < surviveVector.size(); i++) {
		if (surviveVector[i] == 1) {
			counter++;
			sumSurviveAge += ageVector[i];
		}
		else if (surviveVector[i] == 0) {
			counter2++;
			sumPerishedAge += ageVector[i];
		}
	}
	
	survivedMean = sumSurviveAge / counter;
	perishedMean = sumPerishedAge / counter2;

	ageMean[1] = survivedMean;
	ageMean[0] = perishedMean;

	return;
}
//This function takes in two vectors and their size, updates the global var array
void age_var(vector<double> ageVector, vector<double> surviveVector, int numberOfObservations) {
	double survivedAgeVar = 0;
	double perishedAgeVar = 0;
	double survivedNumerator = 0;
	double perishedNumerator = 0;
	double counter = 0;
	double counter2 = 0;
	double sumSurviveAge = 0;
	double sumPerishedAge = 0;
	double survivedMean = 0;
	double perishedMean = 0;

	for (int i = 0; i < surviveVector.size(); i++) {
		if (surviveVector[i] == 1) {
			counter++;
			sumSurviveAge += ageVector[i];
		}
		else if (surviveVector[i] == 0) {
			counter2++;
			sumPerishedAge += ageVector[i];
		}
	}

	survivedMean = sumSurviveAge / counter;
	perishedMean = sumPerishedAge / counter2;


	for (int i = 0; i < surviveVector.size(); i++) {
		if (surviveVector[i] == 1) {
			survivedNumerator = ageVector[i] - survivedMean;
			survivedNumerator = survivedNumerator * survivedNumerator;
			survivedAgeVar += survivedNumerator;
		}
		else if (surviveVector[i] == 0) {
			perishedNumerator = ageVector[i] - perishedMean;
			perishedNumerator = perishedNumerator * perishedNumerator;
			perishedAgeVar += perishedNumerator;
		}
	}

	survivedAgeVar = survivedAgeVar / (counter - 1);
	perishedAgeVar = perishedAgeVar / (counter2 - 1);

	ageVar[1] = survivedAgeVar;
	ageVar[0] = perishedAgeVar;

	return;
}
//this function takes three doubles and caculates the likehood of age based off of the parameters
double calc_age_lh(double theAge, double theMean, double theVar) {
	//double temp = 0;
	//double temp2 = 0;
	//double temp2Numerator = 0;
	//double temp2Denominator = 0;
	//double temp3 = 0;
	double temp4 = 0;

	//temp = (1 / sqrt(2 * 3.14159265359 * theVar));
	//temp2Numerator = pow(theAge - theMean, 2);
	//temp2Denominator = 2 * theVar;
	//temp2 = temp2Numerator / temp2Denominator;
	//temp2 = temp2 * -1;
	//temp3 = exp(temp2);

	temp4 = 1 / sqrt(2 * 3.14159265359 * theVar) * exp(-(pow(theAge - theMean, 2) / (2 * theVar)));
	return temp4;
}
//This functions gets and prints accuracy of my Naive Bayes model
void accuracy(vector<double> survivedTest) {
	double thinksSurvived = 0;
	double totalSurvived = 0;
	for (int i = 0; i < 246; i++) {
		if (rawArray[i][1] >= 0.5) {
			thinksSurvived++;
		}
	}
	for (int i = 0; i < 246; i++) {
		if (survivedTest[i] == 1) {
			totalSurvived++;
		}
	}
	
	cout << "Accuracy: " << (thinksSurvived / totalSurvived) * 100 <<"%"<<endl;
	return;
}

int main(int argc, char** argv) {

	ifstream inFS;
	string line;
	string X_in, pclass_in, survived_in, sex_in, age_in;
	const int MAX_LEN = 2000;
	vector<double> age(MAX_LEN);
	vector<int> X(MAX_LEN);
	vector<double> pclass(MAX_LEN);
	vector<double> sex(MAX_LEN);
	vector<double> survived(MAX_LEN);

	//try to open file
	inFS.open("titanic_project.csv");
	if (!inFS.is_open()) {
		cout << "Could not open file titanic_project.csv" << endl;
		return 1; //1 indicates error
	}

	//can now use inFS stream like cin stream
	//titanic_project.csv should contain two doubles
	getline(inFS, line);

	int numObservations = 0;
	while (inFS.good()) {

		getline(inFS, X_in, ',');
		getline(inFS, pclass_in, ',');
		getline(inFS, survived_in, ',');
		getline(inFS, sex_in, ',');
		getline(inFS, age_in, '\n');


		pclass.at(numObservations) = stof(pclass_in);
		survived.at(numObservations) = stof(survived_in);
		sex.at(numObservations) = stof(sex_in);
		age.at(numObservations) = stof(age_in);

		numObservations++;
	}

	X.resize(numObservations);
	pclass.resize(numObservations);
	survived.resize(numObservations);
	sex.resize(numObservations);
	age.resize(numObservations);
	
	//Creates twice as many vectors to split them into train and tes
	vector<double> ageTest(246);
	vector<double> ageTrain(800);
	vector<double> sexTest(246);
	vector<double> sexTrain(800);
	vector<double> pclassTest(246);
	vector<double> pclassTrain(800);
	vector<double> survivedTest(246);
	vector<double> survivedTrain(800);

	//Splits into train and test
	for (int i = 0; i < 800; i++) {
		ageTrain[i] = age[i];
		sexTrain[i] = sex[i];
		pclassTrain[i] = pclass[i];
		survivedTrain[i] = survived[i];
	}
	for (int i = 800; i < 1046; i++) {
		sexTest[i - 800] = sex.at(i);
		ageTest[i - 800] = age.at(i);
		survivedTest[i - 800] = survived.at(i);
		pclassTest[i - 800] = pclass.at(i);
	}

	inFS.close();//done with file, so close it

	//Calls all functions to populate our arrays for train probabilities
	apriori(survivedTrain, 825);
	lh_pclass(pclassTrain, survivedTrain, 800);
	lh_sex(sexTrain, survivedTrain, 800);
	age_mean(ageTrain, survivedTrain, 800);
	age_var(ageTrain, survivedTrain, 800);
	
	//Prints out our data
	cout << "TRAIN DATA" << endl;
	cout << "A-priori probabilities: " << endl;
	cout << "        0" << "      1" << endl;
	cout << priorArray[0] << " " << priorArray[1] << endl;
	cout << "Conditional probabilities:" << endl;
	cout << "       pclass" << endl;
	cout << "               1" << "        2" << "        3" << endl;
	cout <<"      perished "<< pClassArray[0][0] << " " << pClassArray[0][1] << " " << pClassArray[0][2] << endl;
	cout << "      survived " << pClassArray[1][0] << " " << pClassArray[1][1] << " " << pClassArray[1][2] << endl;
	cout << "       sex" << endl;
	cout << "         female" << "     male" << endl;
	cout << "      0 " << sexArray[0][0] << " " << sexArray[0][1] << endl;
	cout << "      1 " << sexArray[1][0] << " " << sexArray[1][1] << endl;
	cout << "       age" << endl;
	cout << "   mean " << ageMean[0] << " " << ageMean[1] << endl;
	cout << "   std  " <<ageVar[0] << " " << ageVar[1] << endl;
	cout << "TEST DATA" << endl;
	auto begin = std::chrono::high_resolution_clock::now();
	//Fills our raw Array
	double num_s = 0;
	double num_p = 0;
	for (int i = 0; i < 246; i++) {
		//class 3, male
		if (pclassTest[i] == 3 && sexTest[i] == 1) {
			num_s = pClassArray[1][2] * sexArray[1][1] * priorArray[1] * calc_age_lh(ageTest[i], ageMean[1], ageVar[1]);
			num_p = pClassArray[0][2] * sexArray[0][1] * priorArray[0] * calc_age_lh(ageTest[i], ageMean[0], ageVar[0]);
			rawArray[i][0] = num_p / (num_p + num_s);
			rawArray[i][1] = num_s / (num_p + num_s);
		}
		//class2, male
		else if (pclassTest[i] == 2 && sexTest[i] == 1) {
			num_s = pClassArray[1][1] * sexArray[1][1] * priorArray[1] * calc_age_lh(ageTest[i], ageMean[1], ageVar[1]);
			num_p = pClassArray[0][1] * sexArray[0][1] * priorArray[0] * calc_age_lh(ageTest[i], ageMean[0], ageVar[0]);
			rawArray[i][0] = num_p / (num_p + num_s);
			rawArray[i][1] = num_s / (num_p + num_s);

		}
		//class1, male
		else if (pclassTest[i] == 1 && sexTest[i] == 1) {
			num_s = pClassArray[1][0] * sexArray[1][1] * priorArray[1] * calc_age_lh(ageTest[i], ageMean[1], ageVar[1]);
			num_p = pClassArray[0][0] * sexArray[0][1] * priorArray[0] * calc_age_lh(ageTest[i], ageMean[0], ageVar[0]);
			rawArray[i][0] = num_p / (num_p + num_s);
			rawArray[i][1] = num_s / (num_p + num_s);
		}
		//class3 female
		else if (pclassTest[i] == 3 && sexTest[i] == 0) {
			num_s = pClassArray[1][2] * sexArray[1][0] * priorArray[1] * calc_age_lh(ageTest[i], ageMean[1], ageVar[1]);
			rawArray[i][1] = pClassArray[0][2] * sexArray[0][0] * priorArray[0] * calc_age_lh(ageTest[i], ageMean[0], ageVar[0]);
			rawArray[i][0] = num_p / (num_p + num_s);
			rawArray[i][1] = num_s / (num_p + num_s);
		}
		//class2 female
		else if (pclassTest[i] == 2 && sexTest[i] == 0) {
			num_s = pClassArray[1][1] * sexArray[1][0] * priorArray[1] * calc_age_lh(ageTest[i], ageMean[1], ageVar[1]);
			num_p = pClassArray[0][1] * sexArray[0][0] * priorArray[0] * calc_age_lh(ageTest[i], ageMean[0], ageVar[0]);
			rawArray[i][0] = num_p / (num_p + num_s);
			rawArray[i][1] = num_s / (num_p + num_s);
		}
		//class1 female
		else if (pclassTest[i] == 1 && sexTest[i] == 0) {
			num_s = pClassArray[1][0] * sexArray[1][0] * priorArray[1] * calc_age_lh(ageTest[i], ageMean[1], ageVar[1]);
			num_p = pClassArray[0][0] * sexArray[0][0] * priorArray[0] * calc_age_lh(ageTest[i], ageMean[0], ageVar[0]);
			rawArray[i][0] = num_p / (num_p + num_s);
			rawArray[i][1] = num_s / (num_p + num_s);
		}
	}
	//Prints 2-6 of our array 
	cout << "Predictions" << endl;
	cout << " perished" << "    survived"<<endl;
	for (int i = 1; i < 6; i++) {
		for (int j = 0; j < 2; j++) {
			cout << rawArray[i][j];
		}
		cout << endl;
	}
	double tp = 0;
	double tn = 0;
	double fn = 0;
	double fp = 0;
	for (int i = 0; i < 246; i++) {
		if (rawArray[i][1] >= .5 && survivedTest[i] == 1) {
			tp++;
		}
		else if (rawArray[i][1] >= .5 && survivedTest[i] == 0) {
			fp++;
		}
	}
	for (int i = 0; i < 246; i++) {
		if (rawArray[i][1] < .5 && survivedTest[i] == 1) {
			fn++;
		}
		else if (rawArray[i][1] < .5 && survivedTest[i] == 0) {
			tn++;
		}
	}
	double sens = tp / (tp + fn);
	double spec = tn / (tn + fp);
	cout << "Sensitivty: " << sens * 100 << "%" << endl;
	cout << "Specificty: " << spec * 100 << "%" << endl;
	accuracy(survivedTest);
	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
	cout << "Time measured: " << elapsed.count() * 1e-9 << " seconds";




	return 0;
}