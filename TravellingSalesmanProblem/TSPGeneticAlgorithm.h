#pragma once
#ifndef TSPGENETICALGORITHM_H
#define TSPGENETICALGORITHM_H

struct GeneAl {
	int iPopulation = 100, iMutateRate = 1, iPoints = 7, iGeneration = 0, 
		iBestGeneration = 0, iBestScore = 0,
		iThisGenBadScore, iThisGenBestScore;

	std::map<std::pair<int, int>, int> myCacheScoreMap;
	std::map<int, std::pair<int, int>> myCurrentMap, BestMapSoFar;
	std::vector<int> vec_MyFitnessScores;
	std::vector<std::vector<int>> vecv_popGroup, vecv_GenePoolGroup;
	std::vector<int> vec_temp = { 0,1,2,3,4,5,6 }, vec_BestVector, vec_BestVectorThisGen;

	std::mt19937 mtt;

	GeneAl();

	/* HOW GENETIC ALGORITHMS WORK    (GENERALLY)
	1. Initial Population

	2. Get Fitness of each parent
	3. Create Genepool
	4. Create Offpsring from crossovering genepool 
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
	int GetFitness(std::vector<int> _popG);

	void GetPopFitness();

	//////////////////// CREATE A GENEPOOL from population ///////////////////////////////////////
	//Create Genepool
	void CreateGenePool();

	///////////////// SELECT PARENTS FROM GENEPOOL TO BREED OFFSPRING //////////////////////////////////
	//  Create Offspring  
	void CreateOffspring();

	// Mutate the offspring slightly
	void SwapMutate(std::vector<int>& myStr, int _length);

	int VectorFind(std::vector<int> myStr, int _value);

	int VectorFindNot(std::vector<int> myStr1);

	void GreedyCrossover(std::vector<int> v_Father, std::vector<int> v_Mother, std::vector<int>&v_Child);

	////////////////////////////////////////////////////////////////////////////////
	//Clear data
	void Clear();
};


#endif // !TSPGENETICALGORITHM_H
