#include "prison_map.h"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

MapData PrisonMap::ParseTMJ(const std::string& filepath)
{
    std::ifstream file(filepath);
    json j;
    file >> j;
    MapData map;
    map.width = j["width"];
    map.height = j["height"];
    map.tileWidth = j["tilewidth"];
    map.tileHeight = j["tileheight"];
    for (const auto& layer : j["layers"])
    {
        if (layer["name"] == "Ground") map.groundLayer = layer["data"].get<std::vector<int>>();
        else if (layer["name"] == "Walls") map.wallsLayer = layer["data"].get<std::vector<int>>();
    }
    return map;
}

void PrisonMap::BuildVertexArray(sf::VertexArray& vertices, const std::vector<int>& layerData)
{
    vertices.setPrimitiveType(sf::Quads);
    vertices.resize(m_mapData.width * m_mapData.height * 4);
    for (int i = 0; i < m_mapData.width; ++i)
    {
        for (int j = 0; j < m_mapData.height; ++j)
        {
            int tileNumber = layerData[i + j * m_mapData.width];
            if (tileNumber == 0) continue;
            tileNumber--;
            int tilesetWidth = static_cast<int>(m_tileset.getSize().x);
            int tv = tileNumber % (tilesetWidth / m_mapData.tileWidth);
            int tu = tileNumber / (tilesetWidth / m_mapData.tileWidth);
            sf::Vertex* quad = &vertices[(i + j * m_mapData.width) * 4];

            // Setam colturile patratului curent
            quad[0].position = sf::Vector2f((float)(i * m_mapData.tileWidth),       (float)(j * m_mapData.tileHeight));
            quad[1].position = sf::Vector2f((float)((i + 1) * m_mapData.tileWidth), (float)(j * m_mapData.tileHeight));
            quad[2].position = sf::Vector2f((float)((i + 1) * m_mapData.tileWidth), (float)((j + 1) * m_mapData.tileHeight));
            quad[3].position = sf::Vector2f((float)(i * m_mapData.tileWidth),       (float)((j + 1) * m_mapData.tileHeight));

            // Taiem bucatica de imagine a tile-ului
            quad[0].texCoords = sf::Vector2f((float)(tv * m_mapData.tileWidth),       (float)(tu * m_mapData.tileHeight));
            quad[1].texCoords = sf::Vector2f((float)((tv + 1) * m_mapData.tileWidth), (float)(tu * m_mapData.tileHeight));
            quad[2].texCoords = sf::Vector2f((float)((tv + 1) * m_mapData.tileWidth), (float)((tu + 1) * m_mapData.tileHeight));
            quad[3].texCoords = sf::Vector2f((float)(tv * m_mapData.tileWidth),       (float)((tu + 1) * m_mapData.tileHeight));

        }
    }
}

bool PrisonMap::Load(const std::string& mapFilepath, const std::string& tilesetFilepath)
{
    if (!m_tileset.loadFromFile(tilesetFilepath))
    {
        std::cerr << "Eroare la incarcarea imaginii tileset!\n";
        return false;
    }
    m_mapData = ParseTMJ(mapFilepath);
    if (!m_mapData.groundLayer.empty()) BuildVertexArray(m_groundVertices, m_mapData.groundLayer);
    if (!m_mapData.wallsLayer.empty())  BuildVertexArray(m_wallsVertices, m_mapData.wallsLayer);
    return true;
}

void PrisonMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = &m_tileset;
    target.draw(m_groundVertices, states);
    target.draw(m_wallsVertices, states);
}

bool PrisonMap::IsSolidWall(float x, float y) const
{
    int tileX = static_cast<int>(x) / m_mapData.tileWidth;
    int tileY = static_cast<int>(y) / m_mapData.tileHeight;
    if (tileX < 0 || tileX >= m_mapData.width || tileY < 0 || tileY >= m_mapData.height) return true;
    int id = m_mapData.wallsLayer[tileX + tileY * m_mapData.width];
    return (id != 0); // Este valid daca e diferit de null (0).
}
