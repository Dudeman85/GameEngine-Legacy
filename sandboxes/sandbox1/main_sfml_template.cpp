#include "SoundDevice.h"
#include "SoundBuffer.h"
#include "SoundSource.h"
#include <iostream>

int main()
{
	std::cout << "starting...\n";

	SoundDevice* mysounddevice = SoundDevice::get();
	uint32_t sound1 = SoundBuffer::get()->addSoundEffect("assets/spring-weather-1.wav");
	uint32_t sound2 = SoundBuffer::get()->addSoundEffect("assets/jump.wav");
	SoundSource mySpeaker;
	mySpeaker.Play(sound2);
	mySpeaker.Play(sound1);
	std::cout << "got here\n";

	return 0;
}