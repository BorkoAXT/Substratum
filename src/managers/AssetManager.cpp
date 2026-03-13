#include "AssetManager.h"
#include "../defines/Assets.h"

std::unordered_map<std::string, Texture2D> AssetManager::textures;
// std::unordered_map<std::string, Sound> AssetManager::sounds;
// std::unordered_map<std::string, Music> AssetManager::musics;

void AssetManager::LoadAssets()
{
    textures["dirt"]  = LoadTexture(Assets::DIRT_PATH);
    textures["iron"]  = LoadTexture(Assets::IRON_PATH);
    textures["grass"] = LoadTexture(Assets::GRASS_PATH);
    textures["tree_trunk"] = LoadTexture(Assets::WOOD_TRUNK_PATH);
    textures["tree_part2"] = LoadTexture(Assets::WOOD_SECOND_PATH);
    textures["tree_part3"] = LoadTexture(Assets::WOOD_THIRD_PATH);
     textures["tree_leaves_second"] = LoadTexture(Assets::WOOD_SECOND_LEAVES_PATH);
     textures["tree_leaves_third"]  = LoadTexture(Assets::WOOD_THIRD_LEAVES_PATH);

    Image image = LoadImage(Assets::WOOD_CAP);
    ImageResize(&image, 60, 60);
    // textures["tree_cap"] = LoadTexture(Assets::WOOD_CAP);

    textures["tree_cap"] = LoadTextureFromImage(image);

    textures["stone"] = LoadTexture(Assets::STONE);

    // sounds["mine"] = LoadSound(Assets::MINE_SOUND);
    // sounds["place"] = LoadSound(Assets::PLACE_SOUND);
    //
    // musics["background"] = LoadMusicStream(Assets::BG_MUSIC);
}

void AssetManager::UnloadAssets()
{
    for (auto& pair : textures) UnloadTexture(pair.second);
    // for (auto& pair : sounds) UnloadSound(pair.second);
    // for (auto& pair : musics) UnloadMusicStream(pair.second);
}

Texture2D AssetManager::GetTexture(const std::string& name)
{
    auto it = textures.find(name);
    if (it != textures.end())
        return it->second;
    else
        return Texture2D{}; // empty texture to prevent crash
}
Texture2D AssetManager::GetTexture(ItemID itemId)
{
    static const std::unordered_map<ItemID, std::string> itemToTextureKey =
    {
        { ITEM_DIRT,       "dirt" },
        { ITEM_STONE,      "stone" },
        { ITEM_IRON,       "iron" },
    };

    auto it = itemToTextureKey.find(itemId);
    if (it != itemToTextureKey.end())
    {
        return GetTexture(it->second);
    }
    else
    {
        return Texture2D{};
    }
}

// Sound AssetManager::GetSound(const std::string& name)
// {
//     return sounds.at(name);
// }
//
// Music AssetManager::GetMusic(const std::string& name)
// {
//     return musics.at(name);
// }