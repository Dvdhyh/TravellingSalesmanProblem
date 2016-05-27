#include "AllHeaderFiles.h"

//mtt() is itself an object calling a constructor. Use member init  (after the :)   instead of inside the braces (Uniform init)
GeneAl::GeneAl() : mtt(std::time(0)) {
	iPopulation = 100;
	iMutateRate = 1;
	iGeneration = 0;
	iScore = 0;
	iHighScore = 100000;
	iPoints = 7;
}

//////////////////// STARTING POPULATION ///////////////////////////////////////
void GeneAl::RandomPointSet() {
	int aa, bb;
	std::uniform_int_distribution<int> _tempR(0, 400);
	for (int a = 0; a < iPoints; a++) {
		aa = _tempR(mtt);
		bb = _tempR(mtt);
		mymap[a] = std::make_pair(aa, bb);
	}
}

void GeneAl::CreatePopulation() {
	_popGroup.clear();
	for (int a = 0; a < iPopulation; a++) {
		std::random_shuffle(_temp.begin(), _temp.end());
		_popGroup.emplace_back(_temp);
	}
}

//////////////////// GET FITNESS ///////////////////////////////////////

int GeneAl::CalcScore(int _a, int _b) {
	//_a -= 48; // 0 is char 48
	//_b -= 48;
	return (int)pow(pow(mymap[_a].first - mymap[_b].first, 2) + pow(mymap[_a].second - mymap[_b].second, 2), 0.5);
}

int GeneAl::GetFitness(std::vector<std::vector<int>>& _popG, int _index) {
	iScore = 0;
	std::vector<int> _tempString = _popG[_index];
	int tempScore;
	for (int a = 0; a < iPoints - 1; a++) {
		//Creating cache for Point to point distance
		if (myCacheScoreMap[{_tempString[a], _tempString[a + 1] }] == 0) {
			tempScore = CalcScore(_tempString[a], _tempString[a + 1]);
			myCacheScoreMap[{ _tempString[a], _tempString[a + 1] }] = tempScore;
		}
		else {
			tempScore = myCacheScoreMap[{_tempString[a], _tempString[a + 1]}];
		}
		iScore += tempScore;
	}

	myFitnessScores.emplace_back(iScore);

	if (iScore < iHighScore) {
		iHighScore = iScore;
		iBestIndex = _index;
		iHighGeneration = iGeneration;
		_BestString = _tempString;
		_BestMap.clear();
		for (int a = 0; a < iPoints; a++)
			_BestMap[a] = { mymap[_tempString[a]].first, mymap[_tempString[a]].second };

		bUpdateHScores = true;
	}

	return iScore;
}

void GeneAl::GetPopFitness() {

	int _tempCurrentFitness;
	iCurrentBestScore = 9999999;
	iCurrentBadScore = 0;
	for (int a = 0; a < iPopulation; a++) {
		_tempCurrentFitness = GetFitness(_popGroup, a);

		if (_tempCurrentFitness < iCurrentBestScore) {
			iCurrentBestScore = _tempCurrentFitness;
			iCurrentBestIndex = a;
		}

		if (_tempCurrentFitness > iCurrentBadScore) {
			iCurrentBadScore = _tempCurrentFitness;
			iCurrentBadIndex = a;
		}
	}
}

//////////////////// CREATE A GENEPOOL from population ///////////////////////////////////////

void GeneAl::CreateGenePool() {
	_GenePoolGroup.clear();

	int _tempfitnessScore, iPercentage;
	std::vector<int> vecPercent;
	for (int a = 0; a < iPopulation; a++) {

		iPercentage = iCurrentBadScore - iCurrentBestScore;
		_tempfitnessScore = iCurrentBadScore - myFitnessScores[a];// / iPercentage * 100;

																  //Some reason negatives number are appearing. must be 0 or above for discrete_distri
		if (_tempfitnessScore <= 0)
			_tempfitnessScore = 0;

		vecPercent.emplace_back(_tempfitnessScore);
	}

	std::discrete_distribution<int> randomWeights(vecPercent.begin(), vecPercent.end());

	for (int b = 0; b < iPopulation; b++) {
		_GenePoolGroup.emplace_back(_popGroup[(int)randomWeights(mtt)]);
	}
}

///////////////// SELECT PARENTS FROM GENEPOOL TO BREED OFFSPRING //////////////////////////////////

void GeneAl::SwapMutate(std::vector<int>& myStr, int _length) {
	int temp1, temp2;
	std::uniform_int_distribution<int> _tempR(0, _length - 1);
	temp1 = _tempR(mtt);
	temp2 = _tempR(mtt);

	int tempChar = myStr[temp1];
	myStr[temp1] = myStr[temp2];
	myStr[temp2] = tempChar;
}

void GeneAl::ShuffleNeighbourMutate(std::vector<int>& myStr, int _length) {
	int temp1, temp2;
	std::uniform_int_distribution<int> _tempR(0, _length - 1);
	temp1 = _tempR(mtt);
	temp2 = temp1 + _length / 4;
	if (temp2 > _length)
		std::random_shuffle(myStr.begin() + temp1, myStr.end());
	else
		std::random_shuffle(myStr.begin() + temp1, myStr.begin() + temp2);
}

void GeneAl::CreateOffspring() {
	_popGroup.clear();
	int iGenePoolSize = _GenePoolGroup.size(), iStringLen = iPoints;
	int iRandStr1;
	std::uniform_int_distribution<int> _tempGene(0, iGenePoolSize - 1);
	std::uniform_int_distribution<int> _tempR(0, 1);
	for (int a = 0; a < iPopulation; a++) {

		iRandStr1 = _tempGene(mtt);

		///// How would you do crossover 
		//????


		/////Mutation
		if (_tempR(mtt) == 1)
			SwapMutate(_GenePoolGroup[iRandStr1], iStringLen);
		else
			ShuffleNeighbourMutate(_GenePoolGroup[iRandStr1], iStringLen);

		_popGroup.emplace_back(_GenePoolGroup[iRandStr1]);
	}
}

///////////////////////////////////////////////////////////////////

void GeneAl::Clear() {
	myCacheScoreMap.clear();
	myFitnessScores.clear();
	iGeneration = 0;
	iScore = 0;
	iHighScore = 9999999;
	_BestMap.clear();
	_BestString.clear();
	_temp.clear();
};
