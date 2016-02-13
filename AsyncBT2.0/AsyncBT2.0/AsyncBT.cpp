#include "AsyncBT.h"
bool CAsyncBT::fGetSolved()
{
	return mSolved;
}

bool CAsyncBT::fGetSolutionExists()
{
	return mSolutionExist;
}

void CAsyncBT::fDoNextStep()
{
	if (mcpMessegeList->IsMessengerEmpty())
	{
		mSolved = true;
		return;
	}
	else
	{
		SMessage temp = mcpMessegeList->GetNextMessege();
		switch (temp.smType)
		{
		case Ok:
			if (temp.smUIDTo == temp.smUIDFrom)
			{
				bool lFoundUid = false;
				for (auto Agent: mmicpAgentList)
				{
					if (lFoundUid)
					{
						mSolutionExist= Agent.second->fReciveMessege(temp);
					}
					if (!lFoundUid&&Agent.first == temp.smUIDTo)
					{
						lFoundUid = true;
					}
				}
			}
			else
			{
				mSolutionExist = mmicpAgentList[temp.smUIDTo]->fReciveMessege(temp);
			}
			break;
		case NoGood:
			mSolutionExist = mmicpAgentList[temp.smUIDTo]->fReciveMessege(temp);
		default:
			break;
		}
	}
}

void CAsyncBT::fDoAlgorithm()
{
	while (mSolutionExist&&!mSolved)
	{
		fDoNextStep();
	}
	if (mSolved&&mSolutionExist)
	{
		std::cout << "Yay" << std::endl;
	}
	else std::cout << "boo" << std::endl;

}

std::map<int, int> CAsyncBT::GetResults()
{
	std::map<int, int> lTemp;
	for (auto map : mmicpAgentList)
	{
		lTemp.insert(std::make_pair(map.second->fGetUID(), map.second->fGetValue()));
	}
	return lTemp;
}

CAsyncBT::CAsyncBT()
{
}


CAsyncBT::~CAsyncBT()
{
}
