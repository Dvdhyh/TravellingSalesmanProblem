#pragma once
#ifndef TSPGENETICALGORITHM_H
#define TSPGENETICALGORITHM_H

struct GeneAl {
	int iPopulation = 100, iMutateRate = 1, iPoints = 7, iGeneration = 0, iHighGeneration = 0,
		iScore = 0, iHighScore = 0, iBestIndex = 0,
		iCurrentBadIndex, iCurrentBadScore,
		iCurrentBestIndex, iCurrentBestScore;
	bool bUpdateHScores = false;

	std::map<std::pair<int, int>, int> myCacheScoreMap;
	std::map<int, std::pair<int, int>> mymap, _BestMap;
	std::vector<int> myFitnessScores;
	std::vector<std::vector<int>> _popGroup, _GenePoolGroup;
	std::vector<int> _temp = { 0,1,2,3,4,5,6 }, _BestString;

	std::mt19937 mtt;

	GeneAl();

	/* HOW GENETIC ALGORITHMS WORK    (GENERALLY)
	1. Initial Population

	2. Get Fitness of each parent
	3. Create Genepool
	4. Create Offpsring from genepool
	5. Mutate Offspring
	Repeat from step 2 until you decide to stop
	*/

	//////////////////// STARTING POPULATION ///////////////////////////////////////
	//Insert random numbers inside the container 
	void RandomPointSet();

	//Create Initial population 
	void CreatePopulation();

	//////////////////// GET FITNESS ///////////////////////////////////////
	int CalcScore(int _a, int _b);

	//Get fitness
	int GetFitness(std::vector<std::vector<int>>& _popG, int _index = 0);

	void GetPopFitness();

	//////////////////// CREATE A GENEPOOL from population ///////////////////////////////////////
	//Create Genepool
	void CreateGenePool();

	///////////////// SELECT PARENTS FROM GENEPOOL TO BREED OFFSPRING //////////////////////////////////
	//  Create Offspring  
	void CreateOffspring();

	// Mutate the offspring slightly
	void SwapMutate(std::vector<int>& myStr, int _length);

	void ShuffleNeighbourMutate(std::vector<int>& myStr, int _length);

	////////////////////////////////////////////////////////////////////////////////
	//Clear data
	void Clear();
};


#endif // !TSPGENETICALGORITHM_H
