#include <vector>
#include "Deque.h"

const int PREDEFINED_DEFAULT_WEIGHT = 2;

struct Node
{
    int x;
    int y;
    int currentDistance;
    int remainingDistance;
    Node* previous;

    Node(int x, int y, int currentDistance, int remainingDistance, Node* previous):
        x(x),
        y(y),
        currentDistance(currentDistance),
        remainingDistance(remainingDistance),
        previous(previous) {}
};

bool compareDistance(const Node* node1, const Node* node2);

std::vector<Node*> dijkstraPath(Deque* dequeSearched, int startX, int startY, int endX, int endY);
void showDijkstraDequePath(const std::vector<Node*>& path, const Deque& dequeSearched);
void showColoredPath(const std::vector<Node*>& path, const Deque& dequeSearched);

