#ifndef ASTAR_H
#define ASTAR_H

#include <gameminon.h>

struct node
{
    int x;
    int y;
    node* next;
    node* pre;
};

class AStar
{
public:
    AStar();
    void set_object(GameMinon&);
    void algorithm();

private:
    GameMinon object;
    int target_x;
    int target_y;
    node* path;

    node* add_node(int, int);
    node* delete_node(int, int);

};

#endif // ASTAR_H
