# F4VR Common Framework

A common framework for Fallout 4 VR F4SE mods using [CommonLibF4VR](https://github.com/ArthurHub/CommonLibF4VR).

## Mods Built on the Framework
- [FRIK - Full Player Body with IK](https://github.com/rollingrock/Fallout-4-VR-Body)
- [Comfort Swim VR](https://github.com/ArthurHub/F4VR-ComfortSwim)
- [Immersive Flashlight VR](https://github.com/ArthurHub/F4VR-ImmersiveFlashlight)
- [Heisenberg - Physical Interactions](https://www.nexusmods.com/fallout4/mods/99105) (author refuses to open source)

## Creating Mod using F4VR-CommonFramework

Follow the [wiki](https://github.com/ArthurHub/F4VR-CommonFramework/wiki)

**TL;DR:**

1. Setup git submodule:

```
mkdir external
git submodule add https://github.com/ArthurHub/F4VR-CommonFramework.git external/F4VR-CommonFramework
git submodule update --init --recursive
```

2. Copy all/relevant files from `mod-template` folder.

3. Rename all occurrences of "MyMod" in the template files.

## Building F4VR-CommonFramework

Clone repo and setup CommonLibF4 submodule:

```
git clone https://github.com/ArthurHub/F4VR-CommonFramework.git
cd F4VR-CommonFramework
git submodule update --init --recursive
```

Create VS2022 solution:

```
cmake --preset default
```

- Open `.../Fallout-4-VR-Body/build/ImmersiveHUD.sln` in VS2022.
  - Build and debug in VS as usual
  - Any project changes should be done in `CMakeLists.txt`


## Credit/Thanks
Modding is built on the community.
I couldn't do it without people public code.
- Ryan-rsm-McKenzie-alandtse and other [CommonLib](https://github.com/alandtse/CommonLibVR/tree/vr) contributors
- RollingRock-alandtse-shizof-CylonSurfer for open source mods like FRIK, VirtualHolsters, etc. to learn and adopt code from.
