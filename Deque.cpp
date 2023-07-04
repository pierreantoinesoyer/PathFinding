#include <iostream>


#include "Deque.h"
#include <string>

Deque::Deque()  //constructeur vide
{
    height = 0;
    width = 0;
    posX = 0;
    posY = 0;
}

void Deque::changeValue(int x, int y, int value)  //changer la valeur d'un point du tableau
{
    int targetX = posX + x;
    int targetY = posY + y;

    if (targetY >= 0 && targetY < height && targetX >= 0 && targetX < width)
    {
        table[targetY][targetX].value = value;
    }
    else
    {
        std::cout << "OUT OF RANGE : " << targetX << ", " << targetY << " hw: " <<  height << ", " << width << std::endl;
    }
}

Deque::Deque(int _height, int _width)  //initialisation par dimensions
{
    table.resize(_height);

    for (int i = 0; i < _height; i++)
    {
        table[i].resize(_width);
        for (int j = 0; j < _width; j++)
        {
            table[i][j].x = i;
            table[i][j].y = j;
            table[i][j].value = 0;   //valeur par défaut
        }
    }

    height = _height;
    width = _width;
    posX = 0;
    posY = 0;

}

Deque::Deque(int (*arr)[15])  //initialisation par array
{
    width = 15;
    height = 15;
    posX = 0;
    posY = 0;

    table.resize(height);
    for (int i = 0; i < height; i++)
    {
        table[i].resize(width);
        for (int j = 0; j < width; j++)
        {
            table[i][j] = {j, i, arr[i][j]};
        }
    }
}

Deque::Deque(int (*arr)[10]) //initialisation par array
{
    width = 10;
    height = 10;
    posX = 0;
    posY = 0;

    table.resize(height);
    for (int i = 0; i < height; i++)
    {
        table[i].resize(width);
        for (int j = 0; j < width; j++)
        {
            table[i][j] = {j, i, arr[i][j]};
        }
    }
}




void Deque::addPoint(const Point& point)  //ajout d'un point, s'il est à l'extérieur, les lignes et colonnes requises seront ajoutées
{
    if(point.y < posY)  //si le point est au dessus
    {
        int rowsToAdd = posY - point.y;   //nombre de ligne a ajouter
        for(int i = 1; i <= rowsToAdd; i++)
        {
            std::deque<Point> newRow;
            newRow.resize(width);
            for (int j = 0; j < width; ++j)  //on ajoute des lignes de valeur 0
            {
                newRow[j].x =  j + posX;
                newRow[j].y =  posY - i;
                newRow[j].value = 0;
            }
            table.push_front(newRow);
        }
        posY = point.y;
        height += rowsToAdd; //changement hauteur tableau
    }


    else if(point.y > posY + height)  //si le point est en dessous
    {
        int rowsToAdd = point.y - (posY + height - 1); //nombre de ligne a ajouter
        for(int i = 1; i <= rowsToAdd; i++)
        {
            std::deque<Point> newRow;
            newRow.resize(width);
            for (int j = 0; j < width; ++j)   //on ajoute des lignes de valeur 0
            {
                newRow[j].x =  j + posX;
                newRow[j].y =  posY + height + i;
                newRow[j].value = 0;
            }
            table.push_back(newRow); //ajout de la ligne
        }
        height += rowsToAdd;  //changement hauteur tableau
    }

    if (point.x < posX)  //si le point est à gauche
    {
        int columnsToAdd = posX - point.x;  //colonnes à ajouter
        for (int i = 1; i <= columnsToAdd; i++)
        {
            for (int j = 0; j < height; j++)
            {
                table[j].push_front({posX - i, posY + j, 0});  //ajout de points manuellement
            }
        }
        posX = point.x;
        width += columnsToAdd;  //changement largeur tableau
    }
    else if (point.x >= posX + width)  //si le point est à droite
    {
        int columnsToAdd = point.x - (posX + width - 1);
        for (int i = 1; i <= columnsToAdd; i++)
        {
            for (int j = 0; j < height; j++)
            {
                table[j].push_back({posX + width + i, posY + j, 0});
            }
        }
        width += columnsToAdd; //changement largeur tableau
    }


    changeValue(point.x, point.y, point.value); //une fois que les lignes/colonnes sount ajoutées, on change la valeur du point
}

void Deque::showDeque()  //affichage console du deque
{
    std::cout << "[ DEQUE " << width << "x" << height << " ]" <<std::endl;
    for (const std::deque<Point>& row : table)
    {
        std::string line = "{ ";
        for (const Point& point : row)
        {
            line = line +  std::to_string(point.value) + " ";
        }
        line = line + "}";
        std::cout << line <<std::endl;
    }
}

void Deque::showDequePath(std::vector<Point> path)  //affichage du deque avec un chemin
{
    std::cout << "[ DEQUE " << width << "x" << height << " ]" <<std::endl;
    for (const std::deque<Point>& row : table)
    {
        std::string line = "{ ";

        for (const Point& point : row)
        {
            if(isAlreadyInPath(point.x, point.y, path))
            {
                line = line + "\033[32m" + std::to_string(point.value) + " \033[0m";   //on met une couleur bleue si le point fair parti du chemin
            }
            else
            {
                line = line +  std::to_string(point.value) + " ";
            }
        }
        line = line + "}";
        std::cout << line <<std::endl;
    }
}



void Deque::showDequeColored()  //affichage coloré du deque
{
    std::cout << "[ DEQUE " << width << "x" << height << " ]" <<std::endl;
    for (const std::deque<Point>& row : table)
    {
        std::string line = "{ ";
        for (const Point& point : row)
        {
            std::string colorCode;
            switch (point.value)  //attribution d'une couleur selon la valeur
            {
            case 0:
                colorCode = "\033[36m";  //cyan
                break;
            case 1:
            case 2:
                colorCode = "\033[92m";  //vert
                break;
            case 3:
            case 4:
                colorCode =  "\033[93m";  //jaune
                break;
            case 5:
                colorCode =  "\033[38;5;208m";  //orange
                break;
            case 6:
            case 7:
                colorCode =  "\033[91m";  //rouge
                break;
            case 8:
            case 9:
                colorCode =  "\033[95m"; //magenta
                break;
            default:
                colorCode =  "\033[0m";  //blanc
                break;
            }


            line = line + colorCode + "##" + "\033[0m";
        }
        line = line + "}";
        std::cout << line <<std::endl;
    }
}



bool Deque::isWalkable(int xStart, int yStart) //la case est elle navigable
{
    int targetX = posX + xStart;
    int targetY = posY + yStart;

    if(!(targetY >= 0 && targetY < height && targetX >= 0 && targetX < width))  //non si elle n'est pas dans le tableau
    {
        return false;
    }
    if(table[targetY][targetX].value == 0)  //non si c'est un obstacle
    {
        return false;
    }
    return true;
}

bool Deque::isAlreadyInPath(int x, int y, const std::vector<Point>& path)  //le point est is déjà dans le chemin ?
{
    for(Point point : path)
    {
        if(x == point.x && y == point.y)
        {
            return true;  //oui
        }
    }
    return false;
}

std::vector<Point> Deque::recursivePathFinding(int xStart,int yStart,int xEnd,int yEnd, std::vector<Point> currentPath) //recherche de chemin par recursivité
{
    if(!isWalkable(xStart, yStart))  //si la case n'est pas navigable
    {
        return std::vector<Point>();  //retourne un vecteur vide
    }

    currentPath.push_back(table[yStart - posY][xStart - posX]);  //ajout du point dans le chemin
    if(xStart == xEnd && yStart == yEnd)  //si on est à la fin
    {
        return currentPath;  //retourne le chemin
    }

    std::vector<Point> xP;  //case droite
    std::vector<Point> xN;   //gauche
    std::vector<Point> yP;  //bas
    std::vector<Point> yN;  //haut

    if(!isAlreadyInPath(xStart + 1, yStart, currentPath)) //si le point adjacent n'est pas dans le chemin on lance la recurssivité
    {
        xP = recursivePathFinding(xStart + 1, yStart, xEnd, yEnd, currentPath);
    }
    if(!isAlreadyInPath(xStart - 1, yStart, currentPath))
    {
        xN = recursivePathFinding(xStart - 1, yStart, xEnd, yEnd, currentPath);
    }
    if(!isAlreadyInPath(xStart, yStart + 1, currentPath))
    {
        yP = recursivePathFinding(xStart, yStart + 1, xEnd, yEnd, currentPath);
    }
    if(!isAlreadyInPath(xStart, yStart - 1, currentPath))
    {
        yN = recursivePathFinding(xStart, yStart - 1, xEnd, yEnd, currentPath);
    }



    int shortestDistance = 30000;

    if(xP.size() == 0 && xN.size() == 0 && yP.size() == 0 && yN.size() == 0) //si toutes les case adjacentes n'ont pas d'issues
    {
        return std::vector<Point>();  //la case actuelle n'a pas d'issue
    }

    if (xP.size() != 0 && xP.size() < shortestDistance)  //on récupère le chemin le plus court parmi les 4 potentiels
    {
        shortestDistance = xP.size();
        currentPath = xP;
    }
    if (xN.size() != 0 && xN.size() < shortestDistance)
    {
        shortestDistance = xN.size();
        currentPath = xN;
    }
    if (yP.size() != 0 && yP.size() < shortestDistance)
    {
        shortestDistance = yP.size();
        currentPath = yP;
    }
    if (yN.size() != 0 && yN.size() < shortestDistance)
    {
        shortestDistance = yN.size();
        currentPath = yN;
    }

    return currentPath;


}




Deque::~Deque() //destructeur du deque
{
    for (std::deque<Point> row : table)
    {
        row.clear();
    }
    table.clear();
}

