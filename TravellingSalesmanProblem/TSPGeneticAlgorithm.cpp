#include "AllHeaderFiles.h"

//mtt() is itself an object calling a constructor. Use member init  (after the :)   instead of inside the braces (Uniform init)
GeneAl::GeneAl() : mtt(std::time(0)) {
	iPopulation = 100;
	iMutateRate = 1;
	iGeneration = 0;
	iThisGenBestScore = 0;
	iBestScore = 100000;
	iPoints = 7;
}

//////////////////// STARTING POPULATION ///////////////////////////////////////
void GeneAl::RandomPointSet() {
	int aa, bb;
	std::uniform_int_distribution<int> _tempR(0, 400);
	for (int a = 0; a < iPoints; a++) {
		aa = _tempR(mtt);
		bb = _tempR(mtt);
		myCurrentMap[a] = std::make_pair(aa, bb);
	}
}

void GeneAl::CreatePopulation() {
	vecv_popGroup.clear();
	for (int a = 0; a < iPopulation; a++) {
		std::random_shuffle(vec_temp.begin(), vec_temp.end());
		vecv_popGroup.emplace_back(vec_temp);
	}
}

//////////////////// GET FITNESS ///////////////////////////////////////

int GeneAl::CalcScore(int _a, int _b) {
	return (int)pow(pow(myCurrentMap[_a].first - myCurrentMap[_b].first, 2) + pow(myCurrentMap[_a].second - myCurrentMap[_b].second, 2), 0.5);
}

int GeneAl::GetFitness(std::vector<int> v_pop) {
	int iScore = 0, tempScore = 0;

	for (int a = 0; a < iPoints - 1; a++) {
		//Creating cache for Point to point distance
		if (myCacheScoreMap[{v_pop[a], v_pop[a + 1] }] == NULL) {
			tempScore = CalcScore(v_pop[a], v_pop[a + 1]);
			myCacheScoreMap[{ v_pop[a], v_pop[a + 1] }] = tempScore;
		}
		else {
			tempScore = myCacheScoreMap[{v_pop[a], v_pop[a + 1]}];
		}

		iScore += tempScore;
	}

	if (iScore < iThisGenBestScore) {
		iThisGenBestScore = iScore;
		vec_BestVectorThisGen = v_pop;
	}

	if (iScore > iThisGenBadScore) {
		iThisGenBadScore = iScore;
	}

	if (iScore < iBestScore) {
		iBestScore = iScore;
		iBestGeneration = iGeneration;
		vec_BestVector = v_pop;
		BestMapSoFar.clear();
		for (int a = 0; a < iPoints; a++)
			BestMapSoFar[a] = { myCurrentMap[v_pop[a]].first, myCurrentMap[v_pop[a]].second };
	}

	return iScore;
}

void GeneAl::GetPopFitness() {
	vec_BestVectorThisGen.clear();

	iThisGenBestScore = 9999999;
	iThisGenBadScore = 0;
	for (int a = 0; a < iPopulation; a++) 
		vec_MyFitnessScores.emplace_back(GetFitness(vecv_popGroup[a]));
	
}

//////////////////// CREATE A GENEPOOL from population ///////////////////////////////////////

void GeneAl::CreateGenePool() {
	vecv_GenePoolGroup.clear();

	int _tempfitnessScore;
	std::vector<int> vecPercent;
	for (int a = 0; a < iPopulation; a++) {

		_tempfitnessScore = iThisGenBadScore - vec_MyFitnessScores[a];

		//Some reason negatives number are appearing. must be 0 or above for discrete_distribution
		if (_tempfitnessScore <= 0) {
			_tempfitnessScore = 1;
		}

		vecPercent.emplace_back(_tempfitnessScore);
	}

	std::discrete_distribution<int> randomWeights(vecPercent.begin(), vecPercent.end());

	for (int b = 0; b < iPopulation; b++) {
		vecv_GenePoolGroup.emplace_back(vecv_popGroup[randomWeights(mtt)]);
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

int GeneAl::VectorFind(std::vector<int> myStr, int _value) {
	for (int b = 0; b < myStr.size(); b++)
		if (myStr[b] == _value) 
			return b;
		
	return -1;
}

int GeneAl::VectorFindNot(std::vector<int> myStr1) {
	for (int a = 0; a < iPoints; a++) 
		if (myStr1[a] != -1)
			return a;
	
	return -1;
}

void GeneAl::GreedyCrossover(std::vector<int> v_Father, std::vector<int> v_Mother, std::vector<int>& v_Child) {
	int _iTempValue, iFatherIndex1, iFatherIndex2, iMotherIndex1, iMotherIndex2;
	bool bFatherGene = false, bMotherGene = false;
	v_Child.emplace_back(v_Mother[0]);
	std::vector<int> v_Not = vec_temp;
	v_Not[v_Mother[0]] = -1;

	for (int a = 0; a < iPoints - 1; a++) {

		iFatherIndex1 = VectorFind(v_Father, v_Child[a]);
		iFatherIndex2 = iFatherIndex1 + 1;

		iMotherIndex1 = VectorFind(v_Mother, v_Child[a]);
		iMotherIndex2 = iMotherIndex1 + 1;

		if (iFatherIndex1 == iPoints - 1)
			iFatherIndex2 = 0;

		if (iMotherIndex1 == iPoints - 1)
			iMotherIndex2 = 0;

		if (VectorFind(v_Child, v_Father[iFatherIndex2]) != -1)
			bFatherGene = true;

		if (VectorFind(v_Child, v_Mother[iMotherIndex2]) != -1)
			bMotherGene = true;


		if (bFatherGene == true || bMotherGene == true) {
			if (bFatherGene != bMotherGene) {
				if (bFatherGene == true) {
					_iTempValue = v_Mother[iMotherIndex2];
					v_Child.emplace_back(_iTempValue);
					v_Not[_iTempValue] = -1;
				}
				else {
					_iTempValue = v_Father[iFatherIndex2];
					v_Child.emplace_back(_iTempValue);
					v_Not[_iTempValue] = -1;
				}
			}
			else {
				_iTempValue = VectorFindNot(v_Not);
				v_Child.emplace_back(_iTempValue);
				v_Not[_iTempValue] = -1;
			}
		}
		else {
			if (myCacheScoreMap[{v_Mother[iMotherIndex1], v_Mother[iMotherIndex2]}] <= myCacheScoreMap[{v_Father[iFatherIndex1], v_Father[iFatherIndex2]}]) {
				_iTempValue = v_Mother[iMotherIndex2];
				v_Child.emplace_back(_iTempValue);
				v_Not[_iTempValue] = -1;
			}
			else if (myCacheScoreMap[{v_Mother[iMotherIndex1], v_Mother[iMotherIndex2]}] > myCacheScoreMap[{v_Father[iFatherIndex1], v_Father[iFatherIndex2]}]) {
				_iTempValue = v_Father[iFatherIndex2];
				v_Child.emplace_back(_iTempValue);
				v_Not[_iTempValue] = -1;
			}
		}


		bFatherGene = false;
		bMotherGene = false;
	}
}

void GeneAl::CreateOffspring() {
	vecv_popGroup.clear();
	int iGenePoolSize = vecv_GenePoolGroup.size(), iRandStr1, iRandStr2;
	std::uniform_int_distribution<int> _tempGene(0, iGenePoolSize - 1);
	std::vector<int> v_Offspring1;

	for (int a = 0; a < iPopulation; a++) {

		iRandStr1 = _tempGene(mtt);
		iRandStr2 = _tempGene(mtt);
		
		while (iRandStr1 == iRandStr2) {
			iRandStr2 = _tempGene(mtt);
		}

		///// Crossover 
		v_Offspring1.clear();
		GreedyCrossover(vecv_GenePoolGroup[iRandStr1], vecv_GenePoolGroup[iRandStr2], v_Offspring1);

		/////Mutation
		for (int a = 0; a < iMutateRate; a++) 
			SwapMutate(v_Offspring1, iPoints);
		
		vecv_popGroup.emplace_back(v_Offspring1);
	}
}

///////////////////////////////////////////////////////////////////

void GeneAl::Clear() {
	myCacheScoreMap.clear();
	myCurrentMap.clear();
	BestMapSoFar.clear();
	vec_MyFitnessScores.clear();
	vec_BestVector.clear();
	vec_BestVectorThisGen.clear();
	vec_temp.clear();

	iGeneration = 0;
	iThisGenBestScore = 0;
	iBestScore = 9999999;

};
