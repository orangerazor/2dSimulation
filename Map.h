#include "Sprite.h"
#include "Junction.h"

class Map : public Sprite {
private:
    int width;
    int length;
    std::vector<std::vector<Junction>> map;

public:
    Map(int width, int length);
    Map(std::vector<std::vector<Junction>> map);
    Map();
    ~Map();

};