#include "prison_map.h"
#include <fstream>
#include <iostream>
#include <utility>

// Niels Lohmann are un parser smecher
using json = nlohmann::json;

///when i make the map i have layers and i read them
/// i need to make a better map
MapData PrisonMap::ParseTMJ(const std::string& filepath)
{
    std::ifstream file(filepath);
    json j;
    file >> j;

    MapData map;
    map.SetWidth(j["width"]);
    map.SetHeight(j["height"]);
    map.SetTileWidth(j["tilewidth"]);
    map.SetTileHeight(j["tileheight"]);

    for (const auto& layer : j["layers"])
    {
        if (layer["name"] == "Ground")
            map.SetGroundLayer(layer["data"].get<std::vector<int>>());
        else if (layer["name"] == "Walls")
            map.SetWallsLayer(layer["data"].get<std::vector<int>>());
    }
    return map;
}

///optimization for drawing
void PrisonMap::BuildVertexArray(sf::VertexArray& vertices, const std::vector<int>& layerData)
{
    vertices.setPrimitiveType(sf::Quads); // 4 points
    int width = m_mapData.GetWidth();
    int height = m_mapData.GetHeight();
    int tileW = m_mapData.GetTileWidth();
    int tileH = m_mapData.GetTileHeight();

    vertices.resize(static_cast<std::size_t>(width * height) * 4u);

    for (int i = 0; i < width; ++i)
        for (int j = 0; j < height; ++j)
        {
            int tileNumber = layerData[i + j * width];
            if (tileNumber == 0) continue;
            tileNumber--;

            int tilesetWidth = static_cast<int>(m_tileset.getSize().x);
            int tv = tileNumber % (tilesetWidth / tileW);
            int tu = tileNumber / (tilesetWidth / tileW);
            sf::Vertex* quad = &vertices[static_cast<std::size_t>((i + j * width)) * 4u];

            quad[0].position = sf::Vector2f(static_cast<float>(i * tileW),         static_cast<float>(j * tileH));
            quad[1].position = sf::Vector2f(static_cast<float>((i + 1) * tileW),   static_cast<float>(j * tileH));
            quad[2].position = sf::Vector2f(static_cast<float>((i + 1) * tileW),   static_cast<float>((j + 1) * tileH));
            quad[3].position = sf::Vector2f(static_cast<float>(i * tileW),         static_cast<float>((j + 1) * tileH));

            quad[0].texCoords = sf::Vector2f(static_cast<float>(tv * tileW),       static_cast<float>(tu * tileH));
            quad[1].texCoords = sf::Vector2f(static_cast<float>((tv + 1) * tileW), static_cast<float>(tu * tileH));
            quad[2].texCoords = sf::Vector2f(static_cast<float>((tv + 1) * tileW), static_cast<float>((tu + 1) * tileH));
            quad[3].texCoords = sf::Vector2f(static_cast<float>(tv * tileW),       static_cast<float>((tu + 1) * tileH));
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

    if (!m_mapData.GetGroundLayer().empty())
        BuildVertexArray(m_groundVertices, m_mapData.GetGroundLayer());

    if (!m_mapData.GetWallsLayer().empty())
        BuildVertexArray(m_wallsVertices, m_mapData.GetWallsLayer());

    const auto TW = static_cast<float>(m_mapData.GetTileWidth());
    const auto TH = static_cast<float>(m_mapData.GetTileHeight());

    m_zones.clear();
    // emplace_back este mai rapid si construieste obiectul direct in memorie
    m_zones.emplace_back("Cells",    sf::FloatRect(1*TW,  1*TH, 35*TW, 7*TH));
    m_zones.emplace_back("Cell1",    sf::FloatRect(1*TW,  1*TH,  5*TW, 6*TH));
    m_zones.emplace_back("Cell2",    sf::FloatRect(7*TW,  1*TH,  5*TW, 6*TH));
    m_zones.emplace_back("Cell3",    sf::FloatRect(13*TW, 1*TH,  5*TW, 6*TH));
    m_zones.emplace_back("Cell4",    sf::FloatRect(19*TW, 1*TH,  5*TW, 6*TH));
    m_zones.emplace_back("Cell5",    sf::FloatRect(25*TW, 1*TH,  5*TW, 6*TH));
    m_zones.emplace_back("Cell6",    sf::FloatRect(31*TW, 1*TH,  5*TW, 6*TH));
    m_zones.emplace_back("Corridor", sf::FloatRect(1*TW,  8*TH, 38*TW, 1*TH));
    m_zones.emplace_back("Canteen",  sf::FloatRect(1*TW,  10*TH, 12*TW, 8*TH));
    m_zones.emplace_back("Shower",   sf::FloatRect(14*TW, 10*TH, 12*TW, 8*TH));
    m_zones.emplace_back("Library",  sf::FloatRect(27*TW, 10*TH, 12*TW, 8*TH));
    m_zones.emplace_back("Gym",      sf::FloatRect(1*TW,  19*TH, 12*TW, 9*TH));
    m_zones.emplace_back("Central",  sf::FloatRect(14*TW, 19*TH, 12*TW, 9*TH));
    m_zones.emplace_back("Workshop", sf::FloatRect(27*TW, 19*TH, 12*TW, 9*TH));
    m_zones.emplace_back("Infirmary",sf::FloatRect(14*TW, 19*TH, 12*TW, 9*TH));
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
    int tileX = static_cast<int>(x) / m_mapData.GetTileWidth();
    int tileY = static_cast<int>(y) / m_mapData.GetTileHeight();

    if (tileX < 0 || tileX >= m_mapData.GetWidth() || tileY < 0 || tileY >= m_mapData.GetHeight())
        return true;

    int id = m_mapData.GetWallsLayer()[tileX + tileY * m_mapData.GetWidth()];
    return (id != 0);
}

std::string PrisonMap::GetZoneAt(float x, float y) const
{
    for (const auto& zone : m_zones)
        if (zone.GetBounds().contains(x, y)) return zone.GetName();
    return "Outside";
}

// A* for pathfinding
// with manhattan distance
class AStarNode
{
private:
    int x, y; // coordonate
    float f, g, h; // f = g + h
    std::shared_ptr<AStarNode> parent; // retin parintele pentru a reface drumul

public:
    AStarNode(int xp, int yp) : x(xp), y(yp), f(0), g(0), h(0), parent(nullptr) {}

    [[nodiscard]] int GetX() const { return x; }
    [[nodiscard]] int GetY() const { return y; }
    [[nodiscard]] float GetF() const { return f; }
    [[nodiscard]] float GetG() const { return g; }
    [[nodiscard]] float GetH() const { return h; }
    [[nodiscard]] std::shared_ptr<AStarNode> GetParent() const { return parent; }

    void SetG(float gCost) { g = gCost; f = g + h; }
    void SetH(float hCost) { h = hCost; f = g + h; }
    void SetParent(std::shared_ptr<AStarNode> p) { parent = std::move(p); }

    bool operator>(const AStarNode& other) const { return f > other.GetF(); }
};

std::vector<sf::Vector2f> PrisonMap::FindPath(sf::Vector2f startPos, sf::Vector2f targetPos) const
{
    std::vector<sf::Vector2f> path;
    int tileW = m_mapData.GetTileWidth();
    int tileH = m_mapData.GetTileHeight();
    int width = m_mapData.GetWidth();
    int height = m_mapData.GetHeight();

    int startX = static_cast<int>(startPos.x) / tileW;
    int startY = static_cast<int>(startPos.y) / tileH;
    int targetX = static_cast<int>(targetPos.x) / tileW;
    int targetY = static_cast<int>(targetPos.y) / tileH;

    if (startX == targetX && startY == targetY) return path; //same position
    if (targetX < 0 || targetX >= width || targetY < 0 || targetY >= height) return path; //not in the grid
    if (m_mapData.GetWallsLayer()[targetX + targetY * width] != 0) return path;

    //lambda function
    auto comp = [](const std::shared_ptr<AStarNode>& a, const std::shared_ptr<AStarNode>& b) { return a->GetF() > b->GetF(); };
    //decltype is used because i do not know the secret name generated for the 'comp' lambda function
    std::priority_queue<std::shared_ptr<AStarNode>, std::vector<std::shared_ptr<AStarNode>>, decltype(comp)> openSet(comp); // se sorteaza automat
    std::unordered_map<int, std::shared_ptr<AStarNode>> allNodes;

    auto startNode = std::make_shared<AStarNode>(startX, startY);
    openSet.push(startNode); // adaug pozitia de start
    allNodes[startY * width + startX] = startNode;

    // Up, Right, Down, Left
    const int dx[] = {0, 1, 0, -1};
    const int dy[] = {-1, 0, 1, 0};

    while (!openSet.empty())
    {
        auto current = openSet.top(); // scot cel mai promitator nod
        openSet.pop();

        if (current->GetX() == targetX && current->GetY() == targetY) // daca am ajuns la tinta
        {
            auto curr = current;
            while (curr != nullptr) // facem drumul de la sfarsit la inceput
            {
                ///emplace_back instead of push_back
                path.emplace_back(
                    static_cast<float>(curr->GetX() * tileW) + static_cast<float>(tileW) / 2.f,
                    static_cast<float>(curr->GetY() * tileH) + static_cast<float>(tileH) / 2.f);
                curr = curr->GetParent();
            }
            std::reverse(path.begin(), path.end()); // inversam drumul ca sa obtinem traseul corect
            return path;
        }

        for (int i = 0; i < 4; ++i) // verificam toti vecinii
        {
            int nx = current->GetX() + dx[i];
            int ny = current->GetY() + dy[i];

            if (nx < 0 || nx >= width || ny < 0 || ny >= height) continue; // if not in grid
            if (m_mapData.GetWallsLayer()[nx + ny * width] != 0) continue;

            int nIndex = ny * width + nx;
            float gCost = current->GetG() + 1.f;
            auto hCost = static_cast<float>(std::abs(nx - targetX) + std::abs(ny - targetY)); // manhattan distance
            float fCost = gCost + hCost; // costul total

            if (allNodes.find(nIndex) == allNodes.end() || fCost < allNodes[nIndex]->GetF()) // daca este mai bun costul sau nu am fost
            {
                auto neighbor = std::make_shared<AStarNode>(nx, ny);
                neighbor->SetG(gCost);
                neighbor->SetH(hCost);
                neighbor->SetParent(current);
                allNodes[nIndex] = neighbor;
                openSet.push(neighbor);
            }
        }
    }
    return path;
}

