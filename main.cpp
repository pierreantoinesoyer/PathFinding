#include <iostream>
#include <deque>
#include <Vector>
#include <windows.h>


#include "generation.h"
#include "Deque.h"
#include "dijkstra.h"


int array1010[10][10] = {
    {1, 1, 1, 0, 1, 1, 0, 0, 0, 1},
    {0, 0, 1, 0, 1, 0, 0, 1, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 1, 1, 1, 0, 1},
    {0, 1, 0, 1, 0, 1, 0, 1, 1, 0},
    {1, 1, 1, 0, 1, 1, 1, 1, 0, 1},
    {0, 0, 0, 1, 1, 0, 0, 1, 1, 1},
    {1, 0, 1, 1, 1, 0, 1, 0, 0, 1},
    {0, 1, 1, 1, 1, 1, 1, 1, 0, 1},
    {1, 0, 0, 1, 0, 0, 0, 1, 1, 1}
};


int array1515[15][15] = {
    {1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1},
    {0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1},
    {0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1},
    {1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1},
    {0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1},
    {1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0},
    {1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0},
    {1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0},
    {1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0}
};



void printPath(std::vector<Point> path)
{
    std::cout << "[BEST PATH]" << std::endl;
    for(Point point : path)
    {
        std::cout << "(" << point.x << ", " << point.y << ")";
    }
    std::cout << std::endl;
}



int main() {

    Deque bigDeque(3, 4);
    bigDeque.showDeque();

    bigDeque.addPoint({8, 8, 1});
    bigDeque.showDeque();

    Deque dequeFromArray(array1010);

    std::vector<Point> path;

    path = dequeFromArray.recursivePathFinding(0,0,9,9, path); //marche seulement sur des petits array avec un nombre limité de chemins
    printPath(path);
    Sleep(100);
    dequeFromArray.showDequePath(path);

    std::vector<Node*> path2;
    Deque dequeFromArray2(array1515);

    path2 = dijkstraPath(&dequeFromArray2, 0, 0, 13, 14);
    showDijkstraDequePath(path2, dequeFromArray2);

    Sleep(100);

    Deque bigMap(100, 100);
    bigMap.showDeque();

    generateTerrain(&bigMap);
    bigMap.showDequeColored();

    std::vector<Node*> path3;
    path3 = dijkstraPath(&bigMap, 0, 70, 99, 30);
    showDijkstraDequePath(path3, bigMap);
    showColoredPath(path3, bigMap);

    return 0;
}
