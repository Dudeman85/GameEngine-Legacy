# Audio Reference

***All audio functions are included through Application.h. No other includes needed***
```cpp
#include <engine/Application.h>
```

## Loading and playing sound effects

To play sound effects in your program, you need to create new sound source. If you want to play multiple sound effects simultaniously, you need to create multiple sources.

```cpp
static SoundSource mySpeaker1;
static SoundSource mySpeaker2;
static SoundSource mySpeaker3;
static SoundSource mySpeaker4;
	
```

To load sound files, use following functions:
```cpp
//load sound from file
uint32_t sound1 = SoundBuffer::getFile()->addSoundEffect("assets/jump.wav");
uint32_t sound2 = SoundBuffer::getFile()->addSoundEffect("assets/sound100.wav");
```
Engine only supports .wav files.

To play the sound, use Soundsource play function. You can set sound to loop with set looping function.
```cpp
//play sound files
mySpeaker1.Play(sound1);
//sets sound to loop, value 1=true
mySpeaker1.SetLooping(1);
```


## Loading and playing Music

Music system is very similar to sound effects:
```cpp
//Load music from file
MusicBuffer myMusic("assets/forest.wav");
//play music
myMusic.Play();
```
To set music volume:
```cpp
//music volume
myMusic.SetVolume(0.5f);
```

music file is divided into buffers, to play whole audio file, you need to add following into the game loop:
```cpp
myMusic.updateBufferStream();
```


## Implementing 3D sounds

Engine supports full 3D sounds. However, .wav files need to be in mono in order to work.
First you need to set listener location and orientation:
```cpp
//sets listener location on map, x,y,z
engine.soundDevice->SetLocation(0, 0, 0);
//sets listener orientation, horizontal x,y,z, vertical x,y,z
engine.soundDevice->SetOrientation(0.f, 1.f, 0.f, 0.f, 0.f, 1.f);
```
If you want to bind listener location to player character, you need to make definition inside game loop, for example:
```cpp
Transform playerTransform = ecs.getComponent<Transform>(player);
engine.soundDevice->SetLocation(playerTransform.x, playerTransform.y, playerTransform.z);
engine.soundDevice->SetOrientation(0.f, 1.f, 0.f, 0.f, 0.f, 1.f);
```



Engine has 3 different distance models, Linear, Inverse and exponential.
Clamped versions are also available.
Please note you can only use one model in your application.
```cpp
// different attenuation models, only use 1
//Linear models
mySpeaker1.setLinearDistance(1, 1.f, 100.f, 600.f, 1.f);
mySpeaker1.setLinearDistanceClamped(1, 1.f, 100.f, 600.f, 1.f);
//Inverse models
mySpeaker1.setInverseDistance(1, 1.f, 100.f, 600.f, 1.f);
mySpeaker1.setInverseDistanceClamped(1, 1.f, 100.f, 600.f, 1.f);
//Exponential models
mySpeaker1.setExponentialDistance(1, 1.f, 100.f, 600.f, 1.f);
mySpeaker1.setExponentialDistanceClamped(1, 1.f, 100.f, 600.f, 1.f);
```
Parameters given in the example are following:
sourceID, volume, reference distance, max distance, rolloff factor
Reference distance is the distance in pixels where sound volume is 1.
In clamped models volume stays at 1 inside reference area, while in unclamped models volume keeps increasing.
Max distance is the limit for roll off factor to affect sound volume.
Roll off dictates how fast volume decreases, larger value = faster drop.




```cpp
koodilaatikon malli
```