#include <iostream>
#include <vector>
#include <deque>
#include <queue>
#include "Deque.h"
#include "dijkstra.h"

std::vector<std::vector<int>> bestDistance;  //tableau indiquant la distance parcouru pour le meilleur chemin menant à la case en question


bool compareDistance(const Node* node1, const Node* node2)  //comparaison pour la liste de priorité
{
    bool node1IsCloser = node1->currentDistance + node1->remainingDistance > node2->currentDistance + node2->remainingDistance;
    return node1IsCloser;
}

bool isWalkable(Deque* dequeSearched, int x, int y)  //la case est elle navigable ?
{
    int targetX = dequeSearched->posX + x;
    int targetY = dequeSearched->posY + y;

    if(!(targetY >= 0 && targetY < dequeSearched->height && targetX >= 0 && targetX < dequeSearched->width))
    {
        return false;  //non si elle n'est pas dans le tableau
    }
    if(dequeSearched->table[targetY][targetX].value == 0)  //non si c'est un obstacle
    {
        return false;
    }

    return true;
}


int estimateDistance(int x1, int y1, int x2, int y2)  //estimation de la distance restante pour prioriser les case plus proches de la sortie
{
    return (std::abs(x2 - x1) + std::abs(y2 - y1)) * PREDEFINED_DEFAULT_WEIGHT;
}

std::vector<Node*> dijkstraPath(Deque* dequeSearched, int startX, int startY, int endX, int endY)  //fonction principale
{
    bestDistance.resize(dequeSearched->height);  //initialisation de bestDistance
    for(int i = 0; i < dequeSearched->height; i++)
    {
        bestDistance[i].resize(dequeSearched->width);
        for (int j = 0; j < dequeSearched->width; j++) {
            bestDistance[i][j] = 30000;  //valeur par défaut très grande
        }
    }

    std::priority_queue<Node*, std::vector<Node*>, bool(*)(const Node*, const Node*)> pq(compareDistance);  //définition de notre liste de priorité avec la fonction comparatrice

    Node* firstNode = new Node(startX, startY, 0, estimateDistance(startX, startY, endX, endY), nullptr);
    pq.push(firstNode); //ajout du premier noeud de départ

    while (pq.size() != 0)
    {
        Node* current = pq.top();  //récupération du noeud prioritaire
        pq.pop(); //on le retire

        int x = current->x;
        int y = current->y;

        if (x == endX && y == endY)  //si one est à l'arrivée
        {
            std::cout << "FOUND PATH" << std::endl;
            std::vector<Node*> path;

            while (current != nullptr) //ajout des Nodes dans le vecteur
            {
                path.push_back(current);
                current = current->previous;
            }
            return path;
        }




        if(isWalkable(dequeSearched,x - 1,y)) //si le point adjacent est navigable
        {
            int tempCurrentDistance = current->currentDistance + dequeSearched->table[y][x - 1].value;
            int tempRemainingDistance = estimateDistance(x - 1, y, endX, endY);  //on calcule les distances parcouru et restante
            if(tempCurrentDistance + tempRemainingDistance < bestDistance[y][x - 1])  //si la distance est meilleure que celle actuelle
            {
                Node* newNode = new Node(x - 1, y, tempCurrentDistance, tempRemainingDistance, current);
                bestDistance[y][x - 1] = tempCurrentDistance + tempRemainingDistance;
                pq.push(newNode);  //ajout du node et changement de la valeur de la meilleure distance
            }

        }
        if(isWalkable(dequeSearched,x + 1,y)) //pareil pour les autres cases adjacentes
        {
            int tempCurrentDistance = current->currentDistance + dequeSearched->table[y][x + 1].value;
            int tempRemainingDistance = estimateDistance(x + 1, y, endX, endY);
            if(tempCurrentDistance + tempRemainingDistance < bestDistance[y][x + 1])
            {
                Node* newNode = new Node(x + 1, y, tempCurrentDistance, tempRemainingDistance, current);
                bestDistance[y][x + 1] = tempCurrentDistance + tempRemainingDistance;
                pq.push(newNode);
            }

        }
        if(isWalkable(dequeSearched,x,y - 1))
        {
            int tempCurrentDistance = current->currentDistance + dequeSearched->table[y - 1][x].value;
            int tempRemainingDistance = estimateDistance(x , y - 1, endX, endY);
            if(tempCurrentDistance + tempRemainingDistance < bestDistance[y - 1][x])
            {
                Node* newNode = new Node(x , y - 1, tempCurrentDistance, tempRemainingDistance, current);
                bestDistance[y - 1][x] = tempCurrentDistance + tempRemainingDistance;
                pq.push(newNode);
            }

        }
        if(isWalkable(dequeSearched,x ,y + 1))
        {
            int tempCurrentDistance = current->currentDistance + dequeSearched->table[y + 1][x].value;
            int tempRemainingDistance = estimateDistance(x, y + 1, endX, endY);
            if(tempCurrentDistance + tempRemainingDistance < bestDistance[y + 1][x])
            {
                Node* newNode = new Node(x, y + 1, tempCurrentDistance, tempRemainingDistance, current);
                bestDistance[y + 1][x] = tempCurrentDistance + tempRemainingDistance;
                pq.push(newNode);
            }

        }

    }
    std::cout << "PATH NOT FOUND" << std::endl;  //si on ne renvoie rien, le chemin n'a pas été trouvé
    return std::vector<Node*>();
}



bool isNodeInPath(int x, int y, const std::vector<Node*>& path)  //le node est il dans le chemin ?
{
    for (const Node* node : path)
    {
        if (node->x == x && node->y == y)
        {
            return true;  //oui
        }
    }
    return false;
}


void showDijkstraDequePath(const std::vector<Node*>& path, const Deque& dequeSearched)  //affichage deque avec chemin en bleu
{
    std::cout << "[ DEQUE DIJKSTRA PATH " << dequeSearched.width << "x" << dequeSearched.height << " ]" << std::endl;
    for (int y = 0; y < dequeSearched.height; ++y)
    {
        std::string line = "{ ";

        for (int x = 0; x < dequeSearched.width; ++x)
        {
            if (isNodeInPath(x, y, path)) //si le node est dans le path
            {
                line += "\033[34m" + std::to_string(dequeSearched.table[y][x].value) + " \033[0m";   //couleur bleu
            }
            else
            {
                line += std::to_string(dequeSearched.table[y][x].value) + " ";
            }
        }

        line += "}";
        std::cout << line << std::endl;
    }
}

void showColoredPath(const std::vector<Node*>& path, const Deque& dequeSearched) //affichage terrain en couleur et chemin en bleu
{
    std::cout << "[ DEQUE " << dequeSearched.width << "x" << dequeSearched.height << " ]" <<std::endl;
    for (const std::deque<Point>& row : dequeSearched.table)
    {
        std::string line = "{ ";
        for (const Point& point : row)
        {
            std::string colorCode;
            switch (point.value) {  //selon la valeur on choisit un code couleur
            case 0:
                colorCode = "\033[36m";   //cyan : obstacles
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
                colorCode =  "\033[91m"; //rouge
                break;
            case 8:
            case 9:
                colorCode =  "\033[95m"; //magenta
                break;
            default:
                colorCode =  "\033[0m"; //blanc
                break;
            }
            if(isNodeInPath(point.y, point.x, path))
            {
                colorCode = "\033[34m";
            }

            line = line + colorCode + "##" + "\033[0m";
        }
        line = line + "}";
        std::cout << line <<std::endl;
    }
}
