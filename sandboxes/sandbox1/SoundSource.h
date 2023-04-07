#pragma once
#include <AL/al.h>
class SoundSource
{
public:
	SoundSource();
	~SoundSource();

	void Play(const ALuint& buffer_to_play);
	void Stop();
	void Pause();
	void Resume();

	void SetBufferToPlay(const ALuint& buffer_to_play);
	void SetLooping(const bool& loop);

	void setPitch(float pitch);
	void setVolume(float volume);
	//void DistanceModel(int key);
	void setVelocity(float x, float y, float z);
	void setInverseDistance(int sourceID, float volume, float refdistance, float maxdistance, float rolloff);
	void setLinearDistance(int sourceID, float volume, float refdistance, float maxdistance, float rolloff);
	void setExponentialDistance(int sourceID, float volume, float refdistance, float maxdistance, float rolloff);


	bool isPlaying();

private:
	ALuint p_Source;
	ALuint p_Buffer = 0;
	float Volume;
	float RollOff;
	float refDistance;
	float maxDistance;
	ALuint sourceID;
};