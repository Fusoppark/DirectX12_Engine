#pragma once
#include "Component.h"

class Script : public Component
{
public:
	Script();
	virtual ~Script();

private:
	// Final Update ���Ұ�
	virtual void FinalUpdate() sealed {}
};

