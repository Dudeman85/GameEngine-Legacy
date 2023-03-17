#pragma once
#include <engine/AL/MusicBuffer.h>
#include <engine/AL/SoundBuffer.h>
#include <engine/AL/SoundDevice.h>
#include <engine/AL/SoundSource.h>
#include <engine/ECSCore.h>

extern ECS ecs;

using namespace std;

namespace engine
{
	struct SoundPlayer
	{
		SoundSource soundsource;
	};
}