#include "prison_map.h"
#include <fstream>
#include <iostream>
#include <utility>

// Niels Lohmann are un parser smecher
using json = nlohmann::json;

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
        if (layer["name"] == "Ground") map.SetGroundLayer(layer["data"].get<std::vector<int>>());
        else if (layer["name"] == "Walls" || layer["name"] == "walls") map.SetWallsLayer(layer["data"].get<std::vector<int>>());
        else if (layer["name"] == "usi") map.SetUsiLayer(layer["data"].get<std::vector<int>>());
        else if (layer["name"] == "pat") map.SetPatLayer(layer["data"].get<std::vector<int>>());
        else if (layer["name"] == "afara") map.SetAfaraLayer(layer["data"].get<std::vector<int>>());
        else if (layer["name"] == "munca") map.SetMuncaLayer(layer["data"].get<std::vector<int>>());
        else if (layer["name"] == "dulap") map.SetDulapLayer(layer["data"].get<std::vector<int>>());
    }
    return map;
}

void PrisonMap::BuildVertexArray(LayerRenderData& renderData, const std::vector<int>& layerData)
{
    renderData.arrays.clear();
    std::map<int, sf::VertexArray> tempArrays;
    int width = m_mapData.GetWidth();
    int height = m_mapData.GetHeight();
    int tileW = m_mapData.GetTileWidth();
    int tileH = m_mapData.GetTileHeight();

    for (int i = 0; i < width; ++i)
        for (int j = 0; j < height; ++j)
        {
            auto rawTileNumber = static_cast<unsigned int>(layerData[i + j * width]);
            if (rawTileNumber == 0) continue;
            bool flipH = (rawTileNumber & 0x80000000) != 0;
            bool flipV = (rawTileNumber & 0x40000000) != 0;
            bool flipD = (rawTileNumber & 0x20000000) != 0;
            unsigned int tileID = rawTileNumber & 0x1FFFFFFF;
            int tsIndex = -1;
            for (int k = m_tilesets.size() - 1; k >= 0; --k)
                if (tileID >= static_cast<unsigned int>(m_tilesets[k].firstGid))
                {
                    tsIndex = k;
                    break;
                }
            if (tsIndex == -1) continue;
            const auto& ts = m_tilesets[tsIndex];
            unsigned int localID = tileID - ts.firstGid;
            int tv = localID % ts.columns;
            int tu = localID / ts.columns;

            if (tempArrays.find(tsIndex) == tempArrays.end()) tempArrays[tsIndex].setPrimitiveType(sf::Quads);
            sf::Vertex quad[4];
            quad[0].position = sf::Vector2f(static_cast<float>(i * tileW),         static_cast<float>(j * tileH));
            quad[1].position = sf::Vector2f(static_cast<float>((i + 1) * tileW),   static_cast<float>(j * tileH));
            quad[2].position = sf::Vector2f(static_cast<float>((i + 1) * tileW),   static_cast<float>((j + 1) * tileH));
            quad[3].position = sf::Vector2f(static_cast<float>(i * tileW),         static_cast<float>((j + 1) * tileH));

            sf::Vector2f tc[4];
            tc[0] = sf::Vector2f(static_cast<float>(tv * ts.tileWidth),       static_cast<float>(tu * ts.tileHeight));
            tc[1] = sf::Vector2f(static_cast<float>((tv + 1) * ts.tileWidth), static_cast<float>(tu * ts.tileHeight));
            tc[2] = sf::Vector2f(static_cast<float>((tv + 1) * ts.tileWidth), static_cast<float>((tu + 1) * ts.tileHeight));
            tc[3] = sf::Vector2f(static_cast<float>(tv * ts.tileWidth),       static_cast<float>((tu + 1) * ts.tileHeight));

            if (flipD) std::swap(tc[1], tc[3]);
            if (flipH)
            {
                std::swap(tc[0], tc[1]);
                std::swap(tc[2], tc[3]);
            }
            if (flipV)
            {
                std::swap(tc[0], tc[3]);
                std::swap(tc[1], tc[2]);
            }
            quad[0].texCoords = tc[0];
            quad[1].texCoords = tc[1];
            quad[2].texCoords = tc[2];
            quad[3].texCoords = tc[3];
            for (const auto & k : quad)
                tempArrays[tsIndex].append(k);
        }
    for (auto& pair : tempArrays)
        renderData.arrays.emplace_back(m_tilesets[pair.first].texture, pair.second);
}

bool PrisonMap::Load(const std::string& mapFilepath)
{
    m_tilesets.clear();
    std::ifstream file(mapFilepath);
    if (!file.is_open())
    {
        std::cerr << "Error loading map JSON: " << mapFilepath << "\n";
        return false;
    }
    json j;
    file >> j;

    for (const auto& tsData : j["tilesets"])
    {
        TilesetInfo ts;
        ts.firstGid = tsData["firstgid"];
        ts.columns = tsData["columns"];
        ts.tileWidth = tsData["tilewidth"];
        ts.tileHeight = tsData["tileheight"];

        std::string fullPath = tsData["image"];
        std::size_t lastSlash = fullPath.find_last_of("/\\");
        std::string filename = (lastSlash != std::string::npos) ? fullPath.substr(lastSlash + 1) : fullPath;

        ts.texture = std::make_shared<sf::Texture>();
        if (!ts.texture->loadFromFile("assets/" + filename))
            std::cerr << "Error loading tileset texture: assets/" << filename << "\n";
        m_tilesets.push_back(ts);
    }
    file.close();

    m_mapData = ParseTMJ(mapFilepath);

    if (!m_mapData.GetGroundLayer().empty())
        BuildVertexArray(m_groundRender, m_mapData.GetGroundLayer());

    if (!m_mapData.GetWallsLayer().empty())
        BuildVertexArray(m_wallsRender, m_mapData.GetWallsLayer());

    if (!m_mapData.GetUsiLayer().empty())
        BuildVertexArray(m_usiRender, m_mapData.GetUsiLayer());

    if (!m_mapData.GetPatLayer().empty())
        BuildVertexArray(m_patRender, m_mapData.GetPatLayer());

    if (!m_mapData.GetAfaraLayer().empty())
        BuildVertexArray(m_afaraRender, m_mapData.GetAfaraLayer());

    if (!m_mapData.GetMuncaLayer().empty())
        BuildVertexArray(m_muncaRender, m_mapData.GetMuncaLayer());

    if (!m_mapData.GetDulapLayer().empty())
        BuildVertexArray(m_dulapRender, m_mapData.GetDulapLayer());

    const auto TW = static_cast<float>(m_mapData.GetTileWidth());
    const auto TH = static_cast<float>(m_mapData.GetTileHeight());

    m_zones.clear();
    // emplace_back este mai rapid si construieste obiectul direct in memorie
    m_zones.emplace_back("Cells",    sf::FloatRect(1*TW,  1*TH, 38*TW, 6*TH));
    m_zones.emplace_back("Cell1",    sf::FloatRect(1*TW,  1*TH,  6*TW, 6*TH));
    m_zones.emplace_back("Cell2",    sf::FloatRect(7*TW,  1*TH,  6*TW, 6*TH));
    m_zones.emplace_back("Cell3",    sf::FloatRect(13*TW, 1*TH,  6*TW, 6*TH));
    m_zones.emplace_back("Cell4",    sf::FloatRect(19*TW, 1*TH,  6*TW, 6*TH));
    m_zones.emplace_back("Cell5",    sf::FloatRect(25*TW, 1*TH,  6*TW, 6*TH));
    m_zones.emplace_back("Cell6",    sf::FloatRect(31*TW, 1*TH,  8*TW, 6*TH));
    m_zones.emplace_back("Corridor", sf::FloatRect(1*TW,  7*TH, 38*TW, 3*TH));
    m_zones.emplace_back("Canteen",  sf::FloatRect(1*TW,  10*TH, 13*TW, 9*TH));
    m_zones.emplace_back("Infirmary",sf::FloatRect(14*TW, 10*TH, 13*TW, 9*TH));
    m_zones.emplace_back("Gym",      sf::FloatRect(27*TW, 10*TH, 12*TW, 9*TH));
    m_zones.emplace_back("Work",     sf::FloatRect(1*TW,  19*TH, 13*TW, 10*TH));
    m_zones.emplace_back("WardenOffice", sf::FloatRect(14*TW, 19*TH, 13*TW, 10*TH));
    m_zones.emplace_back("Shower",   sf::FloatRect(27*TW, 19*TH, 12*TW, 10*TH));
    return true;
}

void PrisonMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    auto drawLayer = [&](const LayerRenderData& layer)
    {
        for (const auto& pair : layer.arrays)
        {
            states.texture = pair.first.get();
            target.draw(pair.second, states);
        }
    };
    drawLayer(m_groundRender);
    drawLayer(m_muncaRender);
    drawLayer(m_dulapRender);
    drawLayer(m_wallsRender);
    drawLayer(m_afaraRender);
    drawLayer(m_patRender);
    drawLayer(m_usiRender);
}

bool PrisonMap::IsSolidWall(float x, float y) const
{
    int tileX = static_cast<int>(x) / m_mapData.GetTileWidth();
    int tileY = static_cast<int>(y) / m_mapData.GetTileHeight();
    if (tileX < 0 || tileX >= m_mapData.GetWidth() || tileY < 0 || tileY >= m_mapData.GetHeight()) return true;

    int idx = tileX + tileY * m_mapData.GetWidth();
    int wallId = m_mapData.GetWallsLayer()[idx] & 0x1FFFFFFF;
    int bedId = m_mapData.GetPatLayer()[idx] & 0x1FFFFFFF;
    int muncaId = m_mapData.GetMuncaLayer()[idx] & 0x1FFFFFFF;
    int dulapId = m_mapData.GetDulapLayer()[idx] & 0x1FFFFFFF;
    return (wallId != 0 || bedId != 0 || muncaId != 0 || dulapId != 0);
}

bool PrisonMap::IsWardenDoor(float x, float y) const
{
    int tileX = static_cast<int>(x) / m_mapData.GetTileWidth();
    int tileY = static_cast<int>(y) / m_mapData.GetTileHeight();
    if (tileX < 0 || tileX >= m_mapData.GetWidth() || tileY < 0 || tileY >= m_mapData.GetHeight()) return false;
    int idx = tileX + tileY * m_mapData.GetWidth();
    int doorId = m_mapData.GetUsiLayer()[idx] & 0x1FFFFFFF;
    return (doorId == 442);
}

bool PrisonMap::IsDulap(float x, float y) const
{
    int tileX = static_cast<int>(x) / m_mapData.GetTileWidth();
    int tileY = static_cast<int>(y) / m_mapData.GetTileHeight();
    if (tileX < 0 || tileX >= m_mapData.GetWidth() || tileY < 0 || tileY >= m_mapData.GetHeight()) return false;
    int idx = tileX + tileY * m_mapData.GetWidth();
    if (m_mapData.GetDulapLayer().empty()) return false;
    int dulapId = m_mapData.GetDulapLayer()[idx] & 0x1FFFFFFF;
    return (dulapId != 0);
}

std::string PrisonMap::GetZoneAt(float x, float y) const
{
    for (const auto& zone : m_zones)
        if (zone.GetBounds().contains(x, y)) return zone.GetName();
    return "Outside";
}

int PrisonMap::HitWall(float x, float y, int damage)
{
    int tileX = static_cast<int>(x) / m_mapData.GetTileWidth();
    int tileY = static_cast<int>(y) / m_mapData.GetTileHeight();

    if (tileX < 0 || tileX >= m_mapData.GetWidth() || tileY < 0 || tileY >= m_mapData.GetHeight())
        return -1;

    int idx = tileX + tileY * m_mapData.GetWidth();
    std::vector<int> walls = m_mapData.GetWallsLayer();
    
    if (walls[idx] != 0)
    {
        if (m_wallDurability.find(idx) == m_wallDurability.end())
        {
            m_wallDurability[idx] = 100;
        }

        m_wallDurability[idx] -= damage;
        int remainingHP = m_wallDurability[idx];
        if (remainingHP <= 0)
        {
            walls[idx] = 0;
            m_mapData.SetWallsLayer(walls);
            BuildVertexArray(m_wallsRender, walls);
            remainingHP = 0;
        }
        return remainingHP;
    }

    return -1;
}


bool PrisonMap::IsOutside(float x, float y) const
{
    int tileX = static_cast<int>(x) / m_mapData.GetTileWidth();
    int tileY = static_cast<int>(y) / m_mapData.GetTileHeight();
    if (tileX < 0 || tileX >= m_mapData.GetWidth() || tileY < 0 || tileY >= m_mapData.GetHeight()) return true;

    int idx = tileX + tileY * m_mapData.GetWidth();
    int afaraId = m_mapData.GetAfaraLayer()[idx] & 0x1FFFFFFF;
    return (afaraId != 0);
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

    [[nodiscard]] int GetX() const
    {
        return x;
    }
    [[nodiscard]] int GetY() const
    {
        return y;
    }
    [[nodiscard]] float GetF() const
    {
        return f;
    }
    [[nodiscard]] float GetG() const
    {
        return g;
    }
    [[nodiscard]] float GetH() const
    {
        return h;
    }
    [[nodiscard]] std::shared_ptr<AStarNode> GetParent() const
    {
        return parent;
    }
    void SetG(float gCost)
    {
        g = gCost;
        f = g + h;
    }
    void SetH(float hCost)
    {
        h = hCost;
        f = g + h;
    }
    void SetParent(std::shared_ptr<AStarNode> p)
    {
        parent = std::move(p);
    }

    bool operator>(const AStarNode& other) const
    {
        return f > other.GetF();
    }
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
    if (targetX < 0) targetX = 0;
    if (targetX >= width) targetX = width - 1;
    if (targetY < 0) targetY = 0;
    if (targetY >= height) targetY = height - 1;

    if (m_mapData.GetWallsLayer()[targetX + targetY * width] != 0)
    {
        bool found = false;
        const int dx[] = {0, -1, 1, 0, -1, 1, -1, 1};
        const int dy[] = {-1, 0, 0, 1, -1, -1, 1, 1};
        for (int i = 0; i < 8; ++i)
        {
            int nx = targetX + dx[i];
            int ny = targetY + dy[i];
            if (nx >= 0 && nx < width && ny >= 0 && ny < height)
            {
                float cx = static_cast<float>(nx * tileW) + static_cast<float>(tileW) / 2.f;
                float cy = static_cast<float>(ny * tileH) + static_cast<float>(tileH) / 2.f;
                if (!IsSolidWall(cx, cy))
                {
                    targetX = nx;
                    targetY = ny;
                    found = true;
                    break;
                }
            }
        }
        if (!found) return path;
    }
    //lambda function
    auto comp = [](const std::shared_ptr<AStarNode>& a, const std::shared_ptr<AStarNode>& b)
    {
        return a->GetF() > b->GetF();
    };
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
            float cx = static_cast<float>(nx * tileW) + static_cast<float>(tileW) / 2.f;
            float cy = static_cast<float>(ny * tileH) + static_cast<float>(tileH) / 2.f;
            if (IsSolidWall(cx, cy)) continue;

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
