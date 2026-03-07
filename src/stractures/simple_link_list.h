#ifndef SIMPLE_LINK_LIST_H
#define SIMPLE_LINK_LIST_H


struct Unit;
struct Animation;
struct AnimateState;
struct UnitData;


typedef union 
{
    struct Unit *unit;
    struct Animation *animation;
    struct AnimateState *animationState;
    struct UnitData *unitData;
    void *raw;
}Type;



typedef struct ListNode
{
    Type value;
    struct ListNode * next;
}ListNode;

typedef void(*Free_Node)(Type value);

typedef struct 
{
    ListNode *head;
    int count;
}LinkList;


#endif