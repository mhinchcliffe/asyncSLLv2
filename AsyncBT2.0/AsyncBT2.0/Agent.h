#pragma once
#include "Common.h"
#include "Messenger.h"

//#define Constraint bool (*)(std::vector<SAgentV>, SAgentV)
class CAgent
{
	int mUID;
	int mValue;
	std::vector<bool(*)(std::map<int, int>, int, int)> mvfpbConstriants;//local view, uid,value
	std::vector<int> mviValueList;
	std::map<int, int> mmiiLocalView;
	std::vector<std::map<int, int>> mmiiNoGoodList;
	CMessenger *mcpOutMessegges;
	void fRemoveFromLocalView(int fvUID);
	bool fCheckValid();
	bool fCheckValid(int fmValue);
	bool fBackTrack();
	bool fCheckLocalView();
	bool fCheckNogood(int fmValue);
	void fAddLocalView(int uid, int value);
	void fSendMessege(int fvTo, eMessegeType type, std::map<int, int> Messege);

public:
	CAgent(int fvUID, int fvValue, std::vector<bool(*)(std::map<int, int>, int, int)> fvConstriants, 
		std::vector<int> fvValueList,CMessenger *fvMessenger);
	~CAgent();

	bool fReciveMessege(SMessage fmMessege);
	int fGetUID();
	int fGetValue();
};

