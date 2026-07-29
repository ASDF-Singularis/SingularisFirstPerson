#pragma once

#include "Modules/ModuleManager.h"

class FSingularisFirstPersonModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
