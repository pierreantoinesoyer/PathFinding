#include <iostream>
#include <random>
#include <algorithm>

#include "Deque.h"
#include "generation.h"
#include <chrono>


const int DEFAULT_TERRAIN_VALUE = 1; //valeur par défault en haut à gauche
const int DEFAULT_TERRAIN_SIZE = 3;    //taille de la zone d'initialisation

const int MAX_VALUE = 9;  //valeur maximale dde terrain
const int MIN_VALUE = 1;    //minimale

const int OBSTACLE_VALUE = 0;  //valeur des obstacles


unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::mt19937 mt(seed);  //définition d'un module random avec une seed basée sur la clock


//initialisation d'un carré en haut à gauche
void generateDefaultStart(Deque* gDeque)
{


    for(int i = 0; i < DEFAULT_TERRAIN_SIZE; i++)
    {
        for(int j = 0; j < DEFAULT_TERRAIN_SIZE; j++)
        {
            gDeque->changeValue(i, j, DEFAULT_TERRAIN_VALUE);
        }
    }
}

//génération de la première ligne de terrain
void generateFirstRow(Deque* gDeque)
{
    ;

    for(int i = DEFAULT_TERRAIN_SIZE; i < gDeque->width; i++)
    {
        int prev_value = gDeque->table[0][i-1].value;   //valeur voisine gauche

        std::uniform_int_distribution<int> dist(std::max(MIN_VALUE, prev_value - 4), std::min(MAX_VALUE, prev_value + 4) + 1); //introduction d'une variation alatoire
        //toutes les variations aléatoires pousseront le terrain à revenir vers la médiane, par exemple pour une valeur voisine de 9, la varation sera entre 5 et 9, pondéré par la suite par le voisin


        int random_variation = dist(mt);

        gDeque->changeValue(i, 0, (prev_value + random_variation)/2 );
    }
}

//génération de toute les lignes de terrain
void generateRows(Deque* gDeque)
{
    for(int i = 1; i < gDeque->height; i++)
    {
        //cas première valeur de ligne
        int prev_value1 = gDeque->table[i-1][0].value;   //valeur voisine haut-gauche
        int prev_value2 = gDeque->table[i-1][1].value;   //haut-mid
        int prev_value3;                                 //haut-droite
        int prev_value4;                                 //gauche

        int prev_sum = prev_value1 + prev_value2;

        std::uniform_int_distribution<int> dist1(std::max(MIN_VALUE, prev_sum/2 - 1), std::min(MAX_VALUE, prev_sum/2 + 1) + 2);
        int random_variation = dist1(mt);
        //on fait pareil que sur la génération de la premmière ligne mais on fais une moyenne des valeurs voisines
        gDeque->changeValue(0, i, (prev_sum + 2*random_variation)/4 );

        for(int j = 1; j < (gDeque->width - 1); j++)
        {
            //cas milieu terrain
            prev_value1 = gDeque->table[i-1][j-1].value;
            prev_value2 = gDeque->table[i-1][j].value;
            prev_value3 = gDeque->table[i-1][j+1].value;
            prev_value4 = gDeque->table[i][j-1].value;

            prev_sum = prev_value1 + prev_value2 + prev_value3 + prev_value4;

            std::uniform_int_distribution<int> dist2(std::max(MIN_VALUE, prev_sum/4 - 1), std::min(MAX_VALUE, (prev_sum + 2)/4 + 1) + 2);
            random_variation = dist2(mt);

            gDeque->changeValue(j, i, (prev_sum + 4*random_variation)/8 );
        }
        //cas dernière valeur de ligne
        prev_value1 = gDeque->table[i-1][gDeque->width - 2].value;
        prev_value2 = gDeque->table[i-1][gDeque->width - 1].value;
        prev_value4 = gDeque->table[i][gDeque->width - 2].value;

        prev_sum = prev_value1 + prev_value2 + prev_value4;

        std::uniform_int_distribution<int> dist3(std::max(MIN_VALUE, prev_sum/3 - 1), std::min(MAX_VALUE, prev_sum/3 + 1) + 2);
        random_variation = dist3(mt);

        gDeque->changeValue(gDeque->width - 1, i, (prev_sum + 3*random_variation)/6 );

    }
}
//"filtre passe bas" sur le terrain
void smoothTerrain(Deque* gDeque)
{
    Deque newDeque(gDeque->width, gDeque->height);  //utilisation d'un deque auxiliaire pour que les valeurs modifiées n'influent pas le filtre au fur et a mesure

    for (int y = 0; y < gDeque->height; y++)
    {
        for (int x = 0; x < gDeque->width; x++)
        {
            newDeque.table[y][x].value = gDeque->table[y][x].value; //initialisation
        }
    }

    for (int y = 1; y < gDeque->height - 1; y++)
    {
        for (int x = 1; x < gDeque->width - 1; x++)
        {
            int sum = gDeque->table[y][x].value;

            sum += gDeque->table[y-1][x-1].value;
            sum += gDeque->table[y-1][x].value;
            sum += gDeque->table[y-1][x+1].value;
            sum += gDeque->table[y][x-1].value;
            sum += gDeque->table[y][x+1].value;
            sum += gDeque->table[y+1][x-1].value;
            sum += gDeque->table[y+1][x].value;
            sum += gDeque->table[y+1][x+1].value;

            int averageValue = (sum + 8 * gDeque->table[y][x].value) / 16;    //valeur moyenne des voisins
            int newValue = std::max(MIN_VALUE, std::min(MAX_VALUE, averageValue));

            newDeque.table[y][x].value = newValue;
        }
    }

    gDeque->table = newDeque.table;  //changement de la table de notre deque
}


void addObstacles(Deque* gDeque)  //ajout obstacles : rivières
{
    for (int y = 1; y < gDeque->height - 1; y++)
    {
        for (int x = 1; x < gDeque->width - 1; x++)
        {
            int obstacleNeighbours = 0;  //on compte le nombre de voisins obstacles
            if (gDeque->table[y-1][x].value == OBSTACLE_VALUE)
                obstacleNeighbours++;
            if (gDeque->table[y+1][x].value == OBSTACLE_VALUE)
                obstacleNeighbours++;
            if (gDeque->table[y][x-1].value == OBSTACLE_VALUE)
                obstacleNeighbours++;
            if (gDeque->table[y][x+1].value == OBSTACLE_VALUE)
                obstacleNeighbours++;

            std::uniform_real_distribution<float> dist(0.0, 1.0);
            float chance = (1.0 + obstacleNeighbours * 38) / (80); //la probabilité est de 1/80 et augmente de 38/80 pour chaque voisin déjà rivière
            float random_value = dist(mt);

            if (random_value < chance)
                gDeque->table[y][x].value = OBSTACLE_VALUE;  //attribution de l'obstacle
        }
    }
}


void generateTerrain(Deque* gDeque)  //fonction principale
{



    generateDefaultStart(gDeque);

    generateFirstRow(gDeque);

    generateRows(gDeque);

    smoothTerrain(gDeque);  //on appelle 3 fois pour amplifier l'effet
    smoothTerrain(gDeque);
    smoothTerrain(gDeque);

    addObstacles(gDeque);
}
