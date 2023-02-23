#include "SoundDevice.h"
#include "SoundBuffer.h"
#include "SoundSource.h"
#include <iostream>

int main()
{
	std::cout << "starting...\n";

	SoundDevice* mysounddevice = SoundDevice::get();
	uint32_t sound1 = SoundBuffer::get()->addSoundEffect("C:/Users/viksterikap/Desktop/projekti/tiimi4/sandboxes/sandbox1/spring-weather-1.wav");
	uint32_t sound2 = SoundBuffer::get()->addSoundEffect("C:/Users/viksterikap/Desktop/projekti/tiimi4/sandboxes/sandbox1/jump.wav");
	SoundSource mySpeaker;
	mySpeaker.Play(sound1);
	mySpeaker.Play(sound2);
	std::cout << "got here\n";

	return 0;
}