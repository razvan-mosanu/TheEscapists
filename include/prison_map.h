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

// era sa pic examenul si sa uit sa transform
// din prima varianta cu struct in clase

class MapData
{
private:
    int m_width = 0;
    int m_height = 0;
    int m_tileWidth = 0;
    int m_tileHeight = 0;
    std::vector<int> m_groundLayer;
    std::vector<int> m_wallsLayer;

public:
    MapData() = default;
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }
    int GetTileWidth() const { return m_tileWidth; }
    int GetTileHeight() const { return m_tileHeight; }
    const std::vector<int>& GetGroundLayer() const { return m_groundLayer; }
    const std::vector<int>& GetWallsLayer() const { return m_wallsLayer; }

    void SetWidth(int w) { m_width = w; }
    void SetHeight(int h) { m_height = h; }
    void SetTileWidth(int tw) { m_tileWidth = tw; }
    void SetTileHeight(int th) { m_tileHeight = th; }
    void SetGroundLayer(const std::vector<int>& layer) { m_groundLayer = layer; }
    void SetWallsLayer(const std::vector<int>& layer) { m_wallsLayer = layer; }
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
private:
    sf::VertexArray m_groundVertices;
    sf::VertexArray m_wallsVertices;
    sf::Texture m_tileset;
    MapData m_mapData;
    std::vector<MapZone> m_zones;

    static MapData ParseTMJ(const std::string& filepath); /// .tmj e formatul pe care il exportez cand fac harta in tiled
    void BuildVertexArray(sf::VertexArray& vertices, const std::vector<int>& layerData);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
    PrisonMap() {}
    ~PrisonMap() = default;

    bool Load(const std::string& mapFilepath, const std::string& tilesetFilepath);
    bool IsSolidWall(float x, float y) const;

    int GetWidthInPixels() const { return m_mapData.GetWidth() * m_mapData.GetTileWidth(); }
    int GetHeightInPixels() const { return m_mapData.GetHeight() * m_mapData.GetTileHeight(); }
    int GetTileWidth() const { return m_mapData.GetTileWidth(); }
    int GetTileHeight() const { return m_mapData.GetTileHeight(); }

    const std::vector<MapZone>& GetZones() const { return m_zones; }
    std::string GetZoneAt(float x, float y) const;

    std::vector<sf::Vector2f> FindPath(sf::Vector2f startPos, sf::Vector2f targetPos) const;
};

#endif // PRISON_MAP_H
