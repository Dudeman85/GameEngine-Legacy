#include "SoundDevice.h"
#include "SoundBuffer.h"
#include "MusicBuffer.h"
#include "SoundSource.h"
#include <iostream>

int main()
{
	std::cout << "starting...\n";

	SoundDevice* mysounddevice = SoundDevice::get();
	uint32_t sound1 = SoundBuffer::get()->addSoundEffect("assets/jump.wav");
	

	SoundSource mySpeaker;

	mySpeaker.Play(sound1);

	MusicBuffer myMusic("assets/spring-weather-1.wav");
	myMusic.Play();

	while (1)
	{
		myMusic.updateBufferStream();
	}

	std::cout << "got here\n";

	return 0;
}