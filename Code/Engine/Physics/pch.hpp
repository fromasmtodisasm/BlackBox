#pragma once

#define PHYSICS_EXPORTS

#define VALIDATOR_LOG(pLog, str) //CryLog("[PhysValidator] %s", str)
#define VALIDATORS_START //CryLog("[PhysValidator] Start")
#define VALIDATOR(member)
#define VALIDATOR_NORM(member)
#define VALIDATOR_NORM_MSG(member,msg,member1)
#define VALIDATOR_RANGE(member,minval,maxval)
#define VALIDATOR_RANGE2(member,minval,maxval)
#define VALIDATORS_END //CryLog("[PhysValidator] End")

#include <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/System/ISystem.hpp>
#include <IPhysics.h>
