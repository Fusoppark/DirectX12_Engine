#pragma once
#include "Component.h"

class Script : public Component
{
public:
	Script();
	virtual ~Script();

private:
	// Final Update 사용불가
	virtual void FinalUpdate() sealed {}
};

