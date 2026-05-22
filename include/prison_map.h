#ifndef PRISON_MAP_H
#define PRISON_MAP_H

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>
#include <queue>
#include <unordered_map>
#include <cmath>
#include <algorithm>


class MapData
{
private:
    int m_width = 0;
    int m_height = 0;
    int m_tileWidth = 0;
    int m_tileHeight = 0;
    std::vector<int> m_groundLayer;
    std::vector<int> m_wallsLayer;
    std::vector<int> m_usiLayer;
    std::vector<int> m_patLayer;
    std::vector<int> m_afaraLayer;
    std::vector<int> m_muncaLayer;
    std::vector<int> m_dulapLayer;

public:
    MapData() = default;
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }
    int GetTileWidth() const { return m_tileWidth; }
    int GetTileHeight() const { return m_tileHeight; }
    const std::vector<int>& GetGroundLayer() const { return m_groundLayer; }
    const std::vector<int>& GetWallsLayer() const { return m_wallsLayer; }
    const std::vector<int>& GetUsiLayer() const { return m_usiLayer; }
    const std::vector<int>& GetPatLayer() const { return m_patLayer; }
    const std::vector<int>& GetAfaraLayer() const { return m_afaraLayer; }

    void SetWidth(int w) { m_width = w; }
    void SetHeight(int h) { m_height = h; }
    void SetTileWidth(int tw) { m_tileWidth = tw; }
    void SetTileHeight(int th) { m_tileHeight = th; }
    void SetGroundLayer(const std::vector<int>& layer) { m_groundLayer = layer; }
    void SetWallsLayer(const std::vector<int>& layer) { m_wallsLayer = layer; }
    void SetUsiLayer(const std::vector<int>& layer) { m_usiLayer = layer; }
    void SetPatLayer(const std::vector<int>& layer) { m_patLayer = layer; }
    void SetAfaraLayer(const std::vector<int>& layer) { m_afaraLayer = layer; }
    void SetMuncaLayer(const std::vector<int>& layer) { m_muncaLayer = layer; }
    const std::vector<int>& GetMuncaLayer() const { return m_muncaLayer; }
    void SetDulapLayer(const std::vector<int>& layer) { m_dulapLayer = layer; }
    const std::vector<int>& GetDulapLayer() const { return m_dulapLayer; }
};

class MapZone // zonele inchisorii
{
private:
    std::string m_name;
    sf::FloatRect m_bounds;

public:
    MapZone(const std::string& name, const sf::FloatRect& bounds): m_name(name), m_bounds(bounds) {}
    const std::string& GetName() const { return m_name; }
    const sf::FloatRect& GetBounds() const { return m_bounds; }
};

class PrisonMap : public sf::Drawable, public sf::Transformable
{
public:
    struct TilesetInfo
    {
        int firstGid;
        std::shared_ptr<sf::Texture> texture;
        int tileWidth;
        int tileHeight;
        int columns;
        int imageWidth;
        int imageHeight;
    };

    struct LayerRenderData
    {
        std::vector<std::pair<std::shared_ptr<sf::Texture>, sf::VertexArray>> arrays;
    };

private:
    std::unordered_map<int, int> m_wallDurability;
    LayerRenderData m_groundRender;
    LayerRenderData m_wallsRender;
    LayerRenderData m_usiRender;
    LayerRenderData m_patRender;
    LayerRenderData m_afaraRender;
    LayerRenderData m_muncaRender;
    LayerRenderData m_dulapRender;
    std::vector<TilesetInfo> m_tilesets;
    MapData m_mapData;
    std::vector<MapZone> m_zones;
    static MapData ParseTMJ(const std::string& filepath);
    void BuildVertexArray(LayerRenderData& renderData, const std::vector<int>& layerData);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
    PrisonMap() {}
    ~PrisonMap() = default;

    bool Load(const std::string& mapFilepath);
    bool IsSolidWall(float x, float y) const;
    bool IsWardenDoor(float x, float y) const;
    bool IsDulap(float x, float y) const;
    int HitWall(float x, float y, int damage);
    bool IsOutside(float x, float y) const;

    int GetWidthInPixels() const { return m_mapData.GetWidth() * m_mapData.GetTileWidth(); }
    int GetHeightInPixels() const { return m_mapData.GetHeight() * m_mapData.GetTileHeight(); }
    int GetTileWidth() const { return m_mapData.GetTileWidth(); }
    int GetTileHeight() const { return m_mapData.GetTileHeight(); }

    const std::vector<MapZone>& GetZones() const { return m_zones; }
    std::string GetZoneAt(float x, float y) const;

    std::vector<sf::Vector2f> FindPath(sf::Vector2f startPos, sf::Vector2f targetPos) const;
};


#endif // PRISON_MAP_H
