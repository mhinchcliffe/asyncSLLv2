#pragma once
#include "Common.h"
#include "Agent.h"
#include "Messenger.h"

class CAsyncBT
{
	std::map<int, CAgent*> mmicpAgentList;
	CMessenger* mcpMessegeList;
	bool mSolved;
	bool mSolutionExist;
public:
	bool fGetSolved();
	bool fGetSolutionExists();

	void fDoNextStep();
	void fDoAlgorithm();
	std::map<int, int> GetResults();
	CAsyncBT();
	~CAsyncBT();
};

