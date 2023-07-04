#ifndef DEQUE_H
#define DEQUE_H
#include "Point.h"
#include <deque>

#include <vector>

class Deque
{
    public:
        Deque();
        Deque(int (*arr)[10]);
        Deque(int (*arr)[15]);
        Deque(int _height, int _width);
        ~Deque();
        std::deque<std::deque<Point>> table;
        void showDeque();
        void addPoint(const Point& point);
        void changeValue(int x, int y, int value);
        std::vector<Point> recursivePathFinding(int xStart,int yStart,int xEnd,int yEnd, std::vector<Point> currentPath);
        bool isWalkable(int xStart, int yStart);
        bool isAlreadyInPath(int x, int y, const std::vector<Point>& path);
        void showDequePath(std::vector<Point> path);
        void showDequeColored();
        int height;
        int width;
        int posX;
        int posY;
    protected:

    private:

};

#endif // DEQUE_H

