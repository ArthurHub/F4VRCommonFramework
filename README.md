# F4VR Common Framework

A common framework for Fallout 4 VR F4SE mods using [CommonLibF4VR](https://github.com/ArthurHub/CommonLibF4VR).

## Creating Mod using F4VRCommonFramework

Follow the [wiki](https://github.com/ArthurHub/F4VRCommonFramework/wiki)

**TL;DR:**

1. Setup git submodule:

```
mkdir external
git submodule add https://github.com/ArthurHub/F4VRCommonFramework.git external/F4VRCommonFramework
git submodule update --init --recursive
```

2. Copy all/relevant files from `mod-template` folder.

3. Rename all occurrences of "MyMod" in the template files.

## Building F4VRCommonFramework

Clone repo and setup CommonLibF4 submodule:

```
git clone https://github.com/ArthurHub/F4VRCommonFramework.git
cd F4VRCommonFramework
git submodule update --init --recursive
```

Create VS2022 solution:

```
cmake --preset default
```

- Open `.../Fallout-4-VR-Body/build/ImmersiveHUD.sln` in VS2022.
  - Build and debug in VS as usual
  - Any project changes should be done in `CMakeLists.txt`
