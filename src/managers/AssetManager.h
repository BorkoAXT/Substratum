#pragma once
#include <string>
#include <unordered_map>
#include <raylib.h>

class AssetManager
{
public:
    static void LoadAssets();
    static void UnloadAssets();

    static Texture2D GetTexture(const std::string& name);
    static Sound GetSound(const std::string& name);
    static Music GetMusic(const std::string& name);

private:
    static std::unordered_map<std::string, Texture2D> textures;
    static std::unordered_map<std::string, Sound> sounds;
    static std::unordered_map<std::string, Music> musics;
};