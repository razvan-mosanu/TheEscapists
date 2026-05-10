#include "prison_map.h"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

///when i make the map i have layers and i read them
/// i need to make a better map
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

///optimization for drawing
void PrisonMap::BuildVertexArray(sf::VertexArray& vertices, const std::vector<int>& layerData)
{
    vertices.setPrimitiveType(sf::Quads); // for points
    vertices.resize(static_cast<std::size_t>(m_mapData.width * m_mapData.height) * 4u);
    for (int i = 0; i < m_mapData.width; ++i)
        for (int j = 0; j < m_mapData.height; ++j)
        {
            int tileNumber = layerData[i + j * m_mapData.width];
            if (tileNumber == 0) continue;
            tileNumber--;
            int tilesetWidth = static_cast<int>(m_tileset.getSize().x);
            int tv = tileNumber % (tilesetWidth / m_mapData.tileWidth);
            int tu = tileNumber / (tilesetWidth / m_mapData.tileWidth);
            sf::Vertex* quad = &vertices[static_cast<std::size_t>((i + j * m_mapData.width)) * 4u];

            quad[0].position = sf::Vector2f(static_cast<float>(i * m_mapData.tileWidth),         static_cast<float>(j * m_mapData.tileHeight));
            quad[1].position = sf::Vector2f(static_cast<float>((i + 1) * m_mapData.tileWidth),   static_cast<float>(j * m_mapData.tileHeight));
            quad[2].position = sf::Vector2f(static_cast<float>((i + 1) * m_mapData.tileWidth),   static_cast<float>((j + 1) * m_mapData.tileHeight));
            quad[3].position = sf::Vector2f(static_cast<float>(i * m_mapData.tileWidth),         static_cast<float>((j + 1) * m_mapData.tileHeight));

            quad[0].texCoords = sf::Vector2f(static_cast<float>(tv * m_mapData.tileWidth),         static_cast<float>(tu * m_mapData.tileHeight));
            quad[1].texCoords = sf::Vector2f(static_cast<float>((tv + 1) * m_mapData.tileWidth),   static_cast<float>(tu * m_mapData.tileHeight));
            quad[2].texCoords = sf::Vector2f(static_cast<float>((tv + 1) * m_mapData.tileWidth),   static_cast<float>((tu + 1) * m_mapData.tileHeight));
            quad[3].texCoords = sf::Vector2f(static_cast<float>(tv * m_mapData.tileWidth),         static_cast<float>((tu + 1) * m_mapData.tileHeight));
        }
}

bool PrisonMap::Load(const std::string& mapFilepath, const std::string& tilesetFilepath)
{
    if (!m_tileset.loadFromFile(tilesetFilepath))
    {
        std::cerr << "Error loading tileset: " << tilesetFilepath << "\n";
        return false;
    }
    m_mapData = ParseTMJ(mapFilepath);
    if (!m_mapData.groundLayer.empty()) BuildVertexArray(m_groundVertices, m_mapData.groundLayer);
    if (!m_mapData.wallsLayer.empty())  BuildVertexArray(m_wallsVertices, m_mapData.wallsLayer);
    const auto TW = static_cast<float>(m_mapData.tileWidth);
    const auto TH = static_cast<float>(m_mapData.tileHeight);
    m_zones.clear();
    m_zones.push_back({"Cells",    sf::FloatRect(1*TW,  1*TH, 35*TW, 7*TH)});
    m_zones.push_back({"Cell1",   sf::FloatRect(1*TW,  1*TH,  5*TW, 6*TH)});
    m_zones.push_back({"Cell2",   sf::FloatRect(7*TW,  1*TH,  5*TW, 6*TH)});
    m_zones.push_back({"Cell3",   sf::FloatRect(13*TW, 1*TH,  5*TW, 6*TH)});
    m_zones.push_back({"Cell4",   sf::FloatRect(19*TW, 1*TH,  5*TW, 6*TH)});
    m_zones.push_back({"Cell5",   sf::FloatRect(25*TW, 1*TH,  5*TW, 6*TH)});
    m_zones.push_back({"Cell6",   sf::FloatRect(31*TW, 1*TH,  5*TW, 6*TH)});
    m_zones.push_back({"Corridor", sf::FloatRect(1*TW,  8*TH, 38*TW, 1*TH)});
    m_zones.push_back({"Canteen",  sf::FloatRect(1*TW,  10*TH, 12*TW, 8*TH)});
    m_zones.push_back({"Shower",   sf::FloatRect(14*TW, 10*TH, 12*TW, 8*TH)});
    m_zones.push_back({"Library",  sf::FloatRect(27*TW, 10*TH, 12*TW, 8*TH)});
    m_zones.push_back({"Gym",      sf::FloatRect(1*TW,  19*TH, 12*TW, 9*TH)});
    m_zones.push_back({"Central",  sf::FloatRect(14*TW, 19*TH, 12*TW, 9*TH)});
    m_zones.push_back({"Workshop", sf::FloatRect(27*TW, 19*TH, 12*TW, 9*TH)});
    m_zones.push_back({"Infirmary",sf::FloatRect(14*TW, 19*TH, 12*TW, 9*TH)});
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
    return (id != 0);
}

std::string PrisonMap::GetZoneAt(float x, float y) const
{
    for (const auto& zone : m_zones)
        if (zone.bounds.contains(x, y)) return zone.name;
    return "Outside";
}

//A* for pathfinding
// with manhattan distance
struct AStarNode
{
    int x, y;
    float f, g, h;
    std::shared_ptr<AStarNode> parent;
    AStarNode(int xp, int yp) : x(xp), y(yp), f(0), g(0), h(0), parent(nullptr) {}
    bool operator>(const AStarNode& other) const { return f > other.f; }
};

std::vector<sf::Vector2f> PrisonMap::FindPath(sf::Vector2f startPos, sf::Vector2f targetPos) const
{
    std::vector<sf::Vector2f> path;
    int startX = static_cast<int>(startPos.x) / m_mapData.tileWidth;
    int startY = static_cast<int>(startPos.y) / m_mapData.tileHeight;
    int targetX = static_cast<int>(targetPos.x) / m_mapData.tileWidth;
    int targetY = static_cast<int>(targetPos.y) / m_mapData.tileHeight;

    if (startX == targetX && startY == targetY) return path; //same position
    if (targetX < 0 || targetX >= m_mapData.width || targetY < 0 || targetY >= m_mapData.height) return path; //not in the grid
    if (m_mapData.wallsLayer[targetX + targetY * m_mapData.width] != 0) return path;

    //lambda function
    auto comp = [](const std::shared_ptr<AStarNode>& a, const std::shared_ptr<AStarNode>& b) { return a->f > b->f; };
    //decltype is used because i do not know the secret name generated for the 'comp' lambda function
    std::priority_queue<std::shared_ptr<AStarNode>, std::vector<std::shared_ptr<AStarNode>>, decltype(comp)> openSet(comp);
    std::unordered_map<int, std::shared_ptr<AStarNode>> allNodes;
    auto startNode = std::make_shared<AStarNode>(startX, startY);
    openSet.push(startNode);
    allNodes[startY * m_mapData.width + startX] = startNode;

    // Up, Right, Down, Left
    const int dx[] = {0, 1, 0, -1};
    const int dy[] = {-1, 0, 1, 0};

    while (!openSet.empty())
    {
        auto current = openSet.top();
        openSet.pop();
        if (current->x == targetX && current->y == targetY)
        {
            auto curr = current;
            while (curr != nullptr)
            {
                ///emplace_back instead of push_back
                path.emplace_back(
                    static_cast<float>(curr->x * m_mapData.tileWidth) + static_cast<float>(m_mapData.tileWidth) / 2.f,
                    static_cast<float>(curr->y * m_mapData.tileHeight) + static_cast<float>(m_mapData.tileHeight) / 2.f);
                curr = curr->parent;
            }
            std::reverse(path.begin(), path.end());
            return path;
        }
        for (int i = 0; i < 4; ++i)
        {
            int nx = current->x + dx[i];
            int ny = current->y + dy[i];

            if (nx < 0 || nx >= m_mapData.width || ny < 0 || ny >= m_mapData.height) continue; //if not in grid
            if (m_mapData.wallsLayer[nx + ny * m_mapData.width] != 0) continue;

            int nIndex = ny * m_mapData.width + nx;
            float gCost = current->g + 1.f;
            auto hCost = static_cast<float>(std::abs(nx - targetX) + std::abs(ny - targetY)); ///manhattan distance
            float fCost = gCost + hCost;
            if (allNodes.find(nIndex) == allNodes.end() || fCost < allNodes[nIndex]->f)
            {
                auto neighbor = std::make_shared<AStarNode>(nx, ny);
                neighbor->g = gCost;
                neighbor->h = hCost;
                neighbor->f = fCost;
                neighbor->parent = current;
                allNodes[nIndex] = neighbor;
                openSet.push(neighbor);
            }
        }
    }
    return path;
}
