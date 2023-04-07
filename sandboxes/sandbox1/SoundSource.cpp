#include "SoundSource.h"
#include <iostream>

SoundSource::SoundSource()
{

	alGenSources(1, &p_Source);
	alSourcei(p_Source, AL_BUFFER, p_Buffer);
}

SoundSource::~SoundSource()
{
	alDeleteSources(1, &p_Source);
}

void SoundSource::Play(const ALuint& buffer_to_play)
{
	if (buffer_to_play != p_Buffer)
	{
		p_Buffer = buffer_to_play;
		alSourcei(p_Source, AL_BUFFER, (ALint)p_Buffer);
	}

	alSourcePlay(p_Source);
}

void SoundSource::Stop()
{
	alSourceStop(p_Source);
}

void SoundSource::Pause()
{
	alSourcePause(p_Source);
}

void SoundSource::Resume()
{
	alSourcePlay(p_Source);
}

void SoundSource::SetBufferToPlay(const ALuint& buffer_to_play)
{
	if (buffer_to_play != p_Buffer)
	{
		p_Buffer = buffer_to_play;
		alSourcei(p_Source, AL_BUFFER, (ALint)p_Buffer);
	}
}


void SoundSource::SetLooping(const bool& loop)
{
	alSourcei(p_Source, AL_LOOPING, (ALint)loop);
}



void SoundSource::setPitch(float pitch)
{
	alSourcef(sourceID, AL_PITCH, pitch);
}

void SoundSource::setVolume(float volume) 
{
	alSourcef(sourceID, AL_GAIN, volume);
}

//void SoundSource::DistanceModel(int key)
//{
//	if (key < 0xD001 || key > 0xD006)
//		throw("bad attunation key");
//
//	alDistanceModel(key);
//}
void SoundSource::setVelocity(float x, float y, float z)
{
	alSource3f(sourceID, AL_VELOCITY, x, y, z);
}

void SoundSource::setInverseDistance(int sourceID, float volume, float refdistance, float maxdistance, float rolloff)
{
	Volume = volume;
	refDistance = refdistance;
	maxDistance = maxdistance;
	RollOff = rolloff;

	alSourcef(sourceID, AL_GAIN, Volume);
	alSourcef(sourceID, AL_REFERENCE_DISTANCE, refDistance);
	alSourcef(sourceID, AL_MAX_DISTANCE, maxDistance);
	alSourcef(sourceID, AL_ROLLOFF_FACTOR, RollOff);
	alDistanceModel(AL_INVERSE_DISTANCE);
}

void SoundSource::setLinearDistance(int sourceID, float volume, float refdistance, float maxdistance, float rolloff)
{
	Volume = volume;
	refDistance = refdistance;
	maxDistance = maxdistance;
	RollOff = rolloff;

	alSourcef(sourceID, AL_GAIN, Volume);
	alSourcef(sourceID, AL_REFERENCE_DISTANCE, refDistance);
	alSourcef(sourceID, AL_MAX_DISTANCE, maxDistance);
	alSourcef(sourceID, AL_ROLLOFF_FACTOR, RollOff);
	alDistanceModel(AL_LINEAR_DISTANCE);
	
}

void SoundSource::setExponentialDistance(int sourceID, float volume, float refdistance, float maxdistance, float rolloff)
{
	Volume = volume;
	refDistance = refdistance;
	maxDistance = maxdistance;
	RollOff = rolloff;

	alSourcef(sourceID, AL_GAIN, Volume);
	alSourcef(sourceID, AL_REFERENCE_DISTANCE, refDistance);
	alSourcef(sourceID, AL_MAX_DISTANCE, maxDistance);
	alSourcef(sourceID, AL_ROLLOFF_FACTOR, RollOff);
	alDistanceModel(AL_EXPONENT_DISTANCE);
}


bool SoundSource::isPlaying()
{
	ALint playState;
	alGetSourcei(p_Source, AL_SOURCE_STATE, &playState);
	return (playState == AL_PLAYING);
}