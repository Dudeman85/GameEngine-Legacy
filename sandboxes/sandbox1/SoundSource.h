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
	//void DistanceModel(int key);
	void setVelocity(float x, float y, float z);
	void setInverseDistance(float distance);
	void setLinearDistance(float distance);
	void setExponentialDistance(float distance);


	bool isPlaying();

private:
	ALuint p_Source;
	ALuint p_Buffer = 0;
	float refDistance;
	float maxDistance;
	ALuint sourceID;
};