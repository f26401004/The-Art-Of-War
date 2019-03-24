#include "astar.h"

AStar::AStar()
{
    path->pre = NULL;
    path->next = NULL;
}

node* AStar::add_node(int x, int y)
{
    node* loc = (node*)malloc(sizeof(node));
    loc->x = x;
    loc->y = y;
    loc->next = NULL;
    loc->pre = NULL;
    node* head = path;
    while(head->next != NULL)
        head = head->next;
    head->next = loc;
    loc->pre = head;
    return head;
}

node* AStar::delete_node(int x, int y)
{
    node* head = path;
    node* orign = path;
    while(head->x != x && head->y != y && head != NULL)
        head = head->next;
    if (head != NULL)
    {
        head->pre->next = head->next;
        head->next->pre = head->pre;
        free(head);
    }
    return orign;
}

void AStar::set_object(GameMinon &obj)
{
    object = obj;
}

void AStar::algorithm()
{

}
