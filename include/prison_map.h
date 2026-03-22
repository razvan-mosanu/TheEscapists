#ifndef PRISON_MAP_H
#define PRISON_MAP_H

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>

struct MapData
{
    int width, height;
    int tileWidth, tileHeight;
    std::vector<int> groundLayer;
    std::vector<int> wallsLayer;
};

class PrisonMap : public sf::Drawable, public sf::Transformable
{
private:
    sf::VertexArray m_groundVertices;
    sf::VertexArray m_wallsVertices;
    sf::Texture m_tileset;
    MapData m_mapData;
    static MapData ParseTMJ(const std::string& filepath);
    void BuildVertexArray(sf::VertexArray& vertices, const std::vector<int>& layerData);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
    PrisonMap() {}
    ~PrisonMap() = default;
    bool Load(const std::string& mapFilepath, const std::string& tilesetFilepath);
    bool IsSolidWall(float x, float y) const;
    int GetWidthInPixels() const { return m_mapData.width * m_mapData.tileWidth; }
    int GetHeightInPixels() const { return m_mapData.height * m_mapData.tileHeight; }
};

#endif // PRISON_MAP_H
