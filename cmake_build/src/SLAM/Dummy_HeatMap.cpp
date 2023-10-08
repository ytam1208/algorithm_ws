#include <iostream>
#include <queue>
#include <cmath>
#include <cstdlib>
#include <cstring>

// Define map structure and constants
const int MAP_SIZE_X = 10;
const int MAP_SIZE_Y = 10;

typedef struct
{
  int size_x;
  int size_y;
  double scale;
  double max_occ_dist;
  double **distances_;
  // Define other map-related members as needed
} map_t;

#define MAP_INDEX(map, i, j) ((i) * (map)->size_y + (j))

#define MAP_VALID(map, i, j) ((i >= 0) && (i < map->size_x) && (j >= 0) && (j < map->size_y))

class CellData
{
public:
  map_t *map_;
  unsigned int i_, j_;
  unsigned int src_i_, src_j_;
  unsigned int state;
};

class CachedDistanceMap
{
public:
  CachedDistanceMap(double scale, double max_dist) : distances_(nullptr), scale_(scale), max_dist_(max_dist)
  {
    cell_radius_ = max_dist / scale;
    distances_ = new double *[cell_radius_ + 2];
    for (int i = 0; i <= cell_radius_ + 1; i++)
    {
      distances_[i] = new double[cell_radius_ + 2];
      for (int j = 0; j <= cell_radius_ + 1; j++)
      {
        distances_[i][j] = sqrt(i * i + j * j);
      }
    }
  }

  ~CachedDistanceMap()
  {
    if (distances_)
    {
      for (int i = 0; i <= cell_radius_ + 1; i++)
        delete[] distances_[i];
      delete[] distances_;
    }
  }

  double **distances_;
  double scale_;
  double max_dist_;
  int cell_radius_;
};

bool operator<(const CellData &a, const CellData &b)
{
  return a.map_->distances_[a.i_][a.j_] > b.map_->distances_[b.i_][b.j_];
}

CachedDistanceMap *get_distance_map(double scale, double max_dist)
{
  static CachedDistanceMap *cdm = nullptr;

  if (!cdm || (cdm->scale_ != scale) || (cdm->max_dist_ != max_dist))
  {
    if (cdm)
      delete cdm;
    cdm = new CachedDistanceMap(scale, max_dist);
  }

  return cdm;
}

void enqueue(map_t *map, int i, int j,
             int src_i, int src_j,
             std::priority_queue<CellData> &Q,
             CachedDistanceMap *cdm,
             unsigned char *marked)
{
  if (marked[MAP_INDEX(map, i, j)])
    return;

  int di = abs(i - src_i);
  int dj = abs(j - src_j);
  double distance = cdm->distances_[di][dj];

  if (distance > cdm->cell_radius_)
    return;

  map->distances_[i][j] = distance;

  CellData cell;
  cell.map_ = map;
  cell.i_ = i;
  cell.j_ = j;
  cell.src_i_ = src_i;
  cell.src_j_ = src_j;

  std::cout << "src("<<src_i<<","<<src_j<<")" << " i(" << i << "," << j <<") dis(" << distance << ")" << std::endl;
  Q.push(cell);

  marked[MAP_INDEX(map, i, j)] = 1;
}

// Update the cspace distance values
void map_update_cspace(map_t *map, double max_occ_dist)
{
  unsigned char *marked;
  std::priority_queue<CellData> Q;

  marked = new unsigned char[map->size_x * map->size_y];
  memset(marked, 0, sizeof(unsigned char) * map->size_x * map->size_y);

  map->max_occ_dist = max_occ_dist;

  CachedDistanceMap *cdm = get_distance_map(map->scale, map->max_occ_dist);

  // Initialize the map distances
  map->distances_ = new double *[map->size_x];
  for (int i = 0; i < map->size_x; i++)
  {
    map->distances_[i] = new double[map->size_y];
    for (int j = 0; j < map->size_y; j++)
    {
      map->distances_[i][j] = max_occ_dist;
    }
  }

  // Enqueue all the obstacle cells in the outer border
  CellData cell;
  cell.map_ = map;
  for (int i = 0; i < map->size_x; i++)
  {
    cell.src_i_ = cell.i_ = i;
    for (int j = 0; j < map->size_y; j++)
    {
      if (i == 0 || i == map->size_x - 1 || j == 0 || j == map->size_y - 1)
      {
        map->distances_[i][j] = 0.0;
        cell.src_j_ = cell.j_ = j;
        cell.state=1;
        marked[MAP_INDEX(map, i, j)] = 1;
        Q.push(cell);
      }
    }
  }

  while (!Q.empty())
  {
    CellData current_cell = Q.top();
    std::cout << "Qurry Src(" << current_cell.src_i_ << "," << current_cell.src_j_ << ") i(" << current_cell.i_ << "," << current_cell.j_ << ")" << std::endl;
    if (current_cell.i_ > 0)
      enqueue(map, current_cell.i_ - 1, current_cell.j_,
              current_cell.src_i_, current_cell.src_j_,
              Q, cdm, marked);
    if (current_cell.j_ > 0)
      enqueue(map, current_cell.i_, current_cell.j_ - 1,
              current_cell.src_i_, current_cell.src_j_,
              Q, cdm, marked);
    if ((int)current_cell.i_ < map->size_x - 1)
      enqueue(map, current_cell.i_ + 1, current_cell.j_,
              current_cell.src_i_, current_cell.src_j_,
              Q, cdm, marked);
    if ((int)current_cell.j_ < map->size_y - 1)
      enqueue(map, current_cell.i_, current_cell.j_ + 1,
              current_cell.src_i_, current_cell.src_j_,
              Q, cdm, marked);
    std::cout << std::endl;
    Q.pop();
  }

  delete[] marked;
}

int main()
{
  // Create a map
  map_t map;
  map.size_x = MAP_SIZE_X;
  map.size_y = MAP_SIZE_Y;
  map.scale = 1.0;
  map.max_occ_dist = 5.0;

  // Update the cspace distances
  map_update_cspace(&map, map.max_occ_dist);

  // Query a distance at a specific coordinate, e.g., (0, 0)
  int query_x = 0;
  int query_y = 0;
  double distance = map.distances_[query_x][query_y];

    for(int i=0; i<MAP_SIZE_X; i++){
        for(int j=0; j<MAP_SIZE_Y; j++){
            std::cout << map.distances_[i][j] << ", ";
        }
        std::cout << std::endl;
    }

  // Print the result
  std::cout << "Distance at (" << query_x << ", " << query_y << ") = " << distance << std::endl;

  // Clean up memory
  for (int i = 0; i < map.size_x; i++)
  {
    delete[] map.distances_[i];
  }
  delete[] map.distances_;

  return 0;
}
