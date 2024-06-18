#pragma once
#include "Script.h"

class TestCameraScript : public Script
{
public:
	TestCameraScript();
	virtual ~TestCameraScript();

	virtual void LateUpdate() override;

private:
	float		_speed = 100.f;
};

