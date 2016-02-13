#include "Agent.h"



void CAgent::fRemoveFromLocalView(int fvUID)
{
}

bool CAgent::fCheckValid()
{
	return fCheckValid(mUID);
}

bool CAgent::fCheckValid(int fvValue)
{
	bool lValid = true;
	SAgentV lTemp;
	lTemp.UID = mUID;
	lTemp.Value = fvValue;
	for (int i = 0; i < mvfpbConstriants.size() && lValid; i++)
	{
		lValid= mvfpbConstriants[i](mmiiLocalView, mUID,fvValue);
	}
	lValid = fCheckNogood(fvValue);
	return lValid;
}

bool CAgent::fBackTrack()
{
	int lNogood = mmiiLocalView.size() - 1;
	if (lNogood==-1) return false;
	std::map<int, int> lmmiiNoGood = mmiiLocalView;
	fRemoveFromLocalView(lmmiiNoGood[lNogood]);
	fSendMessege(lmmiiNoGood[lNogood], NoGood, lmmiiNoGood);
	fCheckLocalView();
	return true;
}

bool CAgent::fCheckLocalView()
{
	if (fCheckValid()) return true;
	for (int i = 0; i < mviValueList.size(); i++)
	{
		if (fCheckValid(mviValueList[i]))
		{
			mValue = i;
			std::map<int,int> temp;
			temp.insert(std::make_pair(mUID, mValue));	
			fSendMessege(mUID, Ok, temp);
			return true;
		}
	}
	return fBackTrack();
}

bool CAgent::fCheckNogood(int fmValue)
{
	bool lValid = true;
	for (int i = 0; i < mmiiNoGoodList.size()&&lValid; i++)
	{
		bool match = true;
		if (mmiiNoGoodList[i].size() != (mmiiLocalView.size() + 1)) match=false;
		else
		{
			for (auto const UID : mmiiLocalView)
			{
				if (UID.second != mmiiNoGoodList[i][UID.first]) match = false;
			}
			if (match)
			{
				if (mmiiNoGoodList[i][mUID] == fmValue) lValid = false;
			}
		}
	}
	return lValid;
}

void CAgent::fAddLocalView(int uid, int value)
{
	mmiiLocalView.insert_or_assign(uid, value);
}

void CAgent::fSendMessege(int fvTo, eMessegeType type, std::map<int, int> Messege)
{
	mcpOutMessegges->AddMessage(fvTo, mUID, Messege, type);
}

CAgent::CAgent(int fvUID, int fvValue, std::vector<bool(*)(std::map<int, int>, int, int)> fvConstriants,
	std::vector<int> fvValueList, CMessenger *fvMessenger)
{
	mUID = fvUID;
	mValue = fvValue;
	mvfpbConstriants = fvConstriants;
	mviValueList = fvValueList;
	mcpOutMessegges = fvMessenger;
	std::map<int, int> temp;
	temp.insert(std::make_pair(mUID, mValue));
	fSendMessege(mUID, Ok, temp);
}


CAgent::~CAgent()
{
}

bool CAgent::fReciveMessege(SMessage fvMessege)
{
	int lOldValue = 0;
	bool lValid = false;
	switch (fvMessege.smType)
	{
	case Ok:
		fAddLocalView(fvMessege.smUIDFrom, fvMessege.smMessage[fvMessege.smUIDFrom]);
		return fCheckLocalView();
		break;
	case NoGood:
		mmiiNoGoodList.push_back(fvMessege.smMessage);
		lOldValue = mValue;
		lValid = fCheckLocalView();
		if (lValid&&lOldValue==mValue)
		{
			std::map<int, int> temp;
			temp.insert(std::make_pair(mUID, mValue));
			fSendMessege(fvMessege.smUIDFrom, Ok, temp);
		}
		return lValid;
		break;
	default:
		break;
	}
	return false;
}

int CAgent::fGetUID()
{
	return mUID;
}

int CAgent::fGetValue()
{
	return mValue;
}
