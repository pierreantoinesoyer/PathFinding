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


//affichage chemin console
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

    Deque bigDeque(3, 4);  //création d'un deque de taille 3x4
    bigDeque.showDeque();

    bigDeque.addPoint({8, 8, 1});  //ajout d'un point à l'extérieur
    bigDeque.showDeque(); //On peut vérifier que le point à bien été ajouté

    Deque dequeFromArray(array1010);

    std::vector<Point> path;

    path = dequeFromArray.recursivePathFinding(0,0,9,9, path); //marche seulement sur des petits array avec un nombre limité de chemins
    printPath(path);  //affichage chemin trouvé
    Sleep(100);  //pause pour éviter les problèmes de buffer d'affichage
    dequeFromArray.showDequePath(path);  //afichage deque avec chemin

    std::vector<Node*> path2;
    Deque dequeFromArray2(array1515);  //deque depuis l'array 15*15

    path2 = dijkstraPath(&dequeFromArray2, 0, 0, 13, 14);
    showDijkstraDequePath(path2, dequeFromArray2);  //affichage chemin

    Sleep(100);

    Deque bigMap(100, 100);  //création d'une carte
    bigMap.showDeque();

    generateTerrain(&bigMap);  //génération du terrain

    std::vector<Node*> path3;
    path3 = dijkstraPath(&bigMap, 0, 70, 99, 30);  //point de départ et de fin en paramètre
    showDijkstraDequePath(path3, bigMap);
    showColoredPath(path3, bigMap);  //affichage carte complète, à noter qu'on peut remarquer la génération en diagonale qui est logique
                                        //vu que les cases générées s'inspirent d'au dessus et à gauche
    return 0;
}
