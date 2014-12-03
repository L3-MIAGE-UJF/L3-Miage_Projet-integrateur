#include <stdio.h>
#include "Kruskal.h"
#include <iostream>
#include <vector>

using namespace std;

vector<int> CompteurFils(int S, vector<vector<int> > const& M);
vector<int> parcour(int Scourant, vector<vector<int> > const& M, vector<int> L);
bool Existe(int E, vector<int> const& L);


vector<int> kru(std::vector<std::vector<int> > *G)


{std::cout << "mainmainmain" << std::endl;
    //vector<vector<int> > G(9);
    //for (int i = 0; i < G.size(); i++)
    //    G[i].assign(G.size(), 0);
/*
    G[0][1] = G[1][0] = 2;
    G[0][2] = G[2][0] = 1;
    G[0][7] = G[7][0] = 1;
    G[1][2] = G[2][1] = 2;
    G[1][3] = G[3][1] = 3;
    G[2][3] = G[3][2] = 2;
    G[2][4] = G[4][2] = 1;
    G[2][7] = G[7][2] = 1;
    G[3][4] = G[4][3] = 1;
    G[3][5] = G[5][3] = 2;
    G[4][5] = G[5][4] = 1;
    G[4][6] = G[6][4] = 1;
    G[5][6] = G[6][5] = 1;
    G[6][7] = G[7][6] = 2;
    G[6][8] = G[8][6] = 2;
    G[7][8] = G[8][7] = 2;

*/

    Kruskal kruskal(*G);
    kruskal.arbre_min(); // calcul de l'arbre min, qui est alors stocké dans kruskal.acv (matrice d'adjacence)

    for (int i = 0; i < G->size(); i++)
        for (int j = 0; j < i; j++)
            if (kruskal.acv[i][j] != 0)
            cout << i << " - " << j <<endl;

/**************************************************************/

	vector<int> L;				//Creation de la liste L

	printf("lancement parcour\n");
	L = parcour(0,kruskal.acv,L); 		//debut de la recusivité avec le sommet 0
	L.push_back(0);                     //rajoute le point pour le retour au point de départ.
	for(int i = 0;i<L.size();i++)
		printf(" L[%d]= %d \n", i, L[i]);


    return L;
}



vector<int> parcour(int Scourant, vector<vector<int> > const& M, vector<int> L)
{
	vector<int> nbFils;
	int i;
	L.push_back(Scourant);					//ajout de du sommet courant dans la liste
	printf("ajout du sommet courant %d\n", Scourant);
	nbFils = CompteurFils(Scourant, M);			//renvoie la liste des sommets adjacent au sommet courant
	printf("recherche du nombre de fils\n");
		for(i = 0;i<nbFils.size(); i++)
		{
			if(!Existe(nbFils[i],L))		//regarde si le sommet n'est pas déja visité
				L = parcour(nbFils[i], M, L);	//parcour le fils suivant
		}


	return L;
}

vector<int> CompteurFils(int S, vector<vector<int> > const& M)
{
	int i;
	vector<int> F;
	for(i=0;i<M.size();i++)
	{
		if(M[i][S] != 0)
			F.push_back(i);		//ajoute le sommet a la liste si il est adjacent au sommet

		if(M[S][i] != 0)
			F.push_back(i);		//ajoute le sommet a la liste si il est adjacent au sommet

	}
	return F;
}

bool Existe(int E, vector<int> const& L)
{
	bool B = false;
	int i = 0;
	while(i<L.size() and B!=true)
	{
		if(L[i] == E)
			B = true;
		i++;
	}
	return B;
}

