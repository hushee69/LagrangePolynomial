/*
 * Author : Demba Cisse, Harry Jandu
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// instruction pour chercher la taille de n'impore
// quelle type de tableau facilement
#ifndef ARRAYSIZE
#define ARRAYSIZE(A) (sizeof(A)/sizeof(A[0]))
#endif

typedef struct DATA_PAIRS
{
	double x, y;
}data_pair;

data_pair *allocate_buffer(unsigned int size)
{
	return (data_pair*)calloc(size, sizeof(data_pair));
}

void deallocate_buffer(void **buffer)
{
	if( *buffer!=NULL )
	{
		free(*buffer);
		*buffer=NULL;
	}

	return;
}

// dp array of (x, y) points
// data_array ne verifie pas si il existe des valeurs dans le tableau ( peut crasher si on donne pas des valeurs mais je m'en fiche )
void fill_data_values(data_pair *dp, int dp_size, double *data_array)
{
	int i=0, j=0;

	for( i=0, j=0; i<dp_size; ++i )
	{
		dp[i].x=data_array[j];
		dp[i].y=data_array[j+1];
		j+=2;
	}
}

// x est valeur a trouver
// peut trouver n'importe quelle valeur x en utilisant le polynomial associe
double find_polynomial(data_pair *dp, int size, double x)
{
	int i=0, j=0;
	double y=0.0f;
	double prod=1.0f;
	double haut=1.0f, bas=1.0f, div=1.0f;
	double sum=0.0f;

	for( i=0; i<size; ++i )
	{
		haut=1.0f, bas=1.0f;
		//printf("i=%d (%f, %f)\n", i, dp[i].x, dp[i].y);
		//printf("( %f )", dp[i].y);

		for( j=0; j<size; ++j )
		{
			if( i!=j )
			{
				haut=haut*(x-dp[j].x);
				bas=bas*(dp[i].x-dp[j].x);
				//printf("( x - %f )", dp[j].x);
				//printf("haut=%f\n", haut);
				//printf("bas=%f\n", bas);
				//printf("haut/bas=%f\n", haut/bas);
			}
		}
		// cherche l[i] appelle div
		//printf("\n___________________________________________________________________________\n\t%f\n", bas);
		div=haut/bas;
		//printf("div=%f\n", div);
		sum=sum+(dp[i].y*div);
		//printf("sum=%f\n", sum);
	}

	return sum;
}

// cette fonction remplit la structure dp_complet avec des valeurs de x manquant
// et les valeurs de y / f(x) associe aux x manquant
// les valeurs de x sont passee par un tableau (ce tableau aura des valeurs de x manquant uniquement)
void remplit_dp_complet(data_pair *dp_observations, int observations_taille, data_pair *dp_complet, int complet_taille, double *x_tableau)
{
	// sum est le valeur trouve pour chaque y correspondant a x
	// par la fonction find_polynomial
	double sum=0.0f;

	// i est pour remplir des valeurs en dp_complet
	// j est pour l'indice de x_tableau
	int i=0, j=0;

	for( i=0; i<complet_taille; ++i )
	{
		sum = find_polynomial(dp_observations, observations_taille, x_tableau[j]);
		dp_complet[i].x = x_tableau[j];
		dp_complet[i].y = sum;
		j++;
	}

	return;
}

void display_data_pair(data_pair *dp, int dp_size)
{
	int i=0;

	printf("x\t\tf(x)\n");
	for( i=0; i<dp_size; ++i )
	{
		printf("%f\t%f\n", dp[i].x, dp[i].y);
	}
	printf("\n");
}

// stabilite de la fonction
// change une valeur y en utilisant
// l'indice passe dans la 2eme parametre
void stabilite(data_pair *dp, int indice, double delta)
{
	dp[indice].y += delta;
}

int main()
{
	// PARTIE 3.1
	/*
	 * data_pair_array[i] = x si i est pair
	 * data_pair_array[i] = f(x) si x est impaire
	 * eg. data_pair_array[0] = 1.0f est x
	 * data_pair_array[1] = 2.0f est f(x)
	*/
	double densite_1[]={0.0f, 0.99987f, 2.0f, 0.99997f, 4.0f, 1.0f, 6.0f, 0.99997f, 8.0f, 0.99988f, 10.0f, 0.99973f, 12.0f, 0.99953f, 14.0f, 0.99927f, 16.0f, 0.99897f, 18.0f, 0.99846f, 20.0f, 0.99805, 22.0f, 0.999751, 24.0f, 0.99705, 26.0f, 0.99650f, 28.0f, 0.99664f, 30.0f, 0.99533f, 32.0f, 0.99472, 34.0f, 0.99472f, 36.0f, 0.99333, 38.0f, 0.99326f};
	
	// valeurs de x qu'on veut chercher
	// peut ajouter ou enlever des valeurs ici
	double densite_1_cherche_valeurs[]=
	{
		1.0f, 3.0f, 5.0f, 9.0f, 11.0f, 13.0f, 15.0f, 17.0f, 19.0f, 21.0f, 23.0f, 25.0f, 27.0f, 29.0f, 31.0f, 33.0f, 35.0f, 37.0f, 39.0f
	};

	// dp_observations est les donnees dans l'enonce
	// dp_complet est une structure avec des valeurs de x
	// qu'on veut chercher, on va afficher ces valeurs
	// facilements plus tard
	data_pair *dp_observations=NULL, *dp_complet=NULL;
	int dp_observations_taille=0, dp_complet_taille=0;
	double delta = 0.00001f;				// pour la stabilite du fonction

	dp_observations_taille=ARRAYSIZE(densite_1)/2;
	dp_complet_taille=ARRAYSIZE(densite_1_cherche_valeurs); // on divise pas par 2 ici parce que c n'est que des valeurs de x dans cet tableau

	// allocation pour le tableau donne dans l'enonce
	dp_observations=allocate_buffer(dp_observations_taille);

	// allocation pour le tableau dp_complet qui aura tous les valeurs manquant de l'enonce
	dp_complet=allocate_buffer(dp_complet_taille);

	// on remplit les valeurs donnees dans l'enonce dans notre structure d'observations
	fill_data_values(dp_observations, dp_observations_taille, densite_1);

	// affichage d'observations
	printf("OBSERVATIONS DENSITE\n");
	display_data_pair(dp_observations, dp_observations_taille);

	// ici, on remplit la structure dp_complet avec les valeurs x manquant
	// et on va aussi remplir les valeurs y / f(x) correspondant aux valeurs x manquant
	// printf("real sum = %f\n", find_polynomial(dp_observations, , 38.0f));	// test pour verifier si les valeurs sont correctes
	remplit_dp_complet(dp_observations, dp_observations_taille, dp_complet, dp_complet_taille, densite_1_cherche_valeurs);

	// une fois remplit, on peut afficher des valeurs ici en utilisant `display_data_pair`
	printf("DENSITE VALEURS X MANQUANT\n");
	display_data_pair(dp_complet, dp_complet_taille);

	// pour la stabilite de la fonction
	printf("EN AJOUTANT %f POUR LA STABILITE, RESULTAT\n", delta);
	stabilite(dp_observations, 0, delta);

	// si necessaire d'afficher les valeurs apres changement, decommente la ligne suivante
	//display_data_pair(dp_observations, dp_observations_taille);

	printf("APRES LA FONCTION STABILITE\n");
	remplit_dp_complet(dp_observations, dp_observations_taille, dp_complet, dp_complet_taille, densite_1_cherche_valeurs);
	display_data_pair(dp_complet, 1);

	// on libere la memoire
	deallocate_buffer((void**)&dp_observations);
	deallocate_buffer((void**)&dp_complet);

	// ON PEUT REUTILISER LES VARIABLES EN HAUT UNE FOIS
	// LA DEALLOCATION EST COMPLET, IL SUFFIT DE CHANGER
	// LES TABLEAU DE DONNEES
	// PARTIE 3.2
	
	/*
	 * data_pair_array[i] = x si i est pair
	 * data_pair_array[i] = f(x) si x est impaire
	 * eg. data_pair_array[0] = 1.0f est x
	 * data_pair_array[1] = 2.0f est f(x)
	*/
	
	double trois_series_1[]=
	{
		10.0f, 9.14f, 8.0f, 8.14f, 13.0f, 8.74f, 9.0f, 8.77f, 11.0f, 9.26f, 14.0f, 8.1f, 6.0f, 6.13f, 4.0f, 3.1f, 12.0f, 9.13f, 7.0f, 7.26f, 5.0f, 4.74f
	};
	
	// valeurs de x qu'on veut chercher
	// peut ajouter ou enlever des valeurs ici
	// ceci est pour les Xi S1 et S2
	double trois_series_1_2_cherche_valeurs[]=
	{
		5.5f, 7.5f, 12.5f, 4.5f, 6.5f, 11.5f, 9.5f, 13.5f, 8.5f, 10.5f
	};

	// dp_observations est les donnees dans l'enonce
	// dp_complet est une structure avec des valeurs de x
	// qu'on veut chercher, on va afficher ces valeurs
	// facilements plus tard
	dp_observations_taille=ARRAYSIZE(trois_series_1)/2;
	dp_complet_taille=ARRAYSIZE(trois_series_1_2_cherche_valeurs); // on divise pas par 2 ici parce que c n'est que des valeurs de x dans cet tableau

	// allocation pour le tableau donne dans l'enonce
	dp_observations=allocate_buffer(dp_observations_taille);

	// allocation pour le tableau dp_complet qui aura tous les valeurs manquant de l'enonce
	dp_complet=allocate_buffer(dp_complet_taille);

	// on remplit les valeurs donnees dans l'enonce dans notre structure d'observations
	fill_data_values(dp_observations, dp_observations_taille, trois_series_1);

	// affichage d'observations
	printf("OBSERVATIONS S1\n");
	display_data_pair(dp_observations, dp_observations_taille);

	// ici, on remplit la structure dp_complet avec les valeurs x manquant
	// et on va aussi remplir les valeurs y / f(x) correspondant aux valeurs x manquant
	remplit_dp_complet(dp_observations, dp_observations_taille, dp_complet, dp_complet_taille, trois_series_1_2_cherche_valeurs);

	// une fois remplit, on peut afficher des valeurs ici en utilisant `display_data_pair`
	printf("S1 VALEURS X MANQUANT\n");
	display_data_pair(dp_complet, dp_complet_taille);

	// pour la stabilite de la fonction
	printf("EN AJOUTANT %f POUR LA STABILITE, RESULTAT\n", delta);
	stabilite(dp_observations, 0, delta);

	// si necessaire d'afficher les valeurs apres changement, decommente la ligne suivante
	//display_data_pair(dp_observations, dp_observations_taille);

	printf("APRES LA FONCTION STABILITE\n");
	remplit_dp_complet(dp_observations, dp_observations_taille, dp_complet, dp_complet_taille, trois_series_1_2_cherche_valeurs);
	display_data_pair(dp_complet, 1);

	// on libere la memoire
	deallocate_buffer((void**)&dp_observations);
	deallocate_buffer((void**)&dp_complet);

	double trois_series_2[]=
	{
		10.0f, 7.46f, 8.0f, 6.77f, 13.0f, 12.74f, 9.0f, 7.11f, 11.0f, 7.81f, 14.0f, 8.84f, 6.0f, 6.08f, 4.0f, 5.39f, 12.0f, 8.15f, 7.0f, 6.42f, 5.0f, 5.73f
	};

	// dp_observations est les donnees dans l'enonce
	// dp_complet est une structure avec des valeurs de x
	// qu'on veut chercher, on va afficher ces valeurs
	// facilements plus tard
	dp_observations_taille=ARRAYSIZE(trois_series_2)/2;
	dp_complet_taille=ARRAYSIZE(trois_series_1_2_cherche_valeurs); // on divise pas par 2 ici parce que c n'est que des valeurs de x dans cet tableau

	// allocation pour le tableau donne dans l'enonce
	dp_observations=allocate_buffer(dp_observations_taille);

	// allocation pour le tableau dp_complet qui aura tous les valeurs manquant de l'enonce
	dp_complet=allocate_buffer(dp_complet_taille);

	// on remplit les valeurs donnees dans l'enonce dans notre structure d'observations
	fill_data_values(dp_observations, dp_observations_taille, trois_series_2);

	// affichage d'observations
	printf("OBSERVATIONS S2\n");
	display_data_pair(dp_observations, dp_observations_taille);

	// ici, on remplit la structure dp_complet avec les valeurs x manquant
	// et on va aussi remplir les valeurs y / f(x) correspondant aux valeurs x manquant
	remplit_dp_complet(dp_observations, dp_observations_taille, dp_complet, dp_complet_taille, trois_series_1_2_cherche_valeurs);

	// une fois remplit, on peut afficher des valeurs ici en utilisant `display_data_pair`
	printf("S2 VALEURS X MANQUANT\n");
	display_data_pair(dp_complet, dp_complet_taille);

	// pour la stabilite de la fonction
	printf("EN AJOUTANT %f POUR LA STABILITE, RESULTAT\n", delta);
	stabilite(dp_observations, 0, delta);

	// si necessaire d'afficher les valeurs apres changement, decommente la ligne suivante
	//display_data_pair(dp_observations, dp_observations_taille);

	printf("APRES LA FONCTION STABILITE\n");
	remplit_dp_complet(dp_observations, dp_observations_taille, dp_complet, dp_complet_taille, trois_series_1_2_cherche_valeurs);
	display_data_pair(dp_complet, 1);

	// on libere la memoire
	deallocate_buffer((void**)&dp_observations);
	deallocate_buffer((void**)&dp_complet);
	
	double trois_series_3[]=
	{
		8.0f, 6.58f, 8.0f, 5.76f, 8.0f, 7.71f, 8.0f, 8.84f, 8.0f, 8.47f, 8.0f, 7.04f, 8.0f, 5.25f, 19.0f, 12.50f, 8.0f, 5.56f, 8.0f, 7.91f, 8.0f, 6.89f
	};
	
	// valeurs de x qu'on veut chercher
	// peut ajouter ou enlever des valeurs ici
	// ceci est pour les Xi S3
	double trois_series_3_cherche_valeurs[]=
	{
		8.1f, 8.2f, 8.3f, 8.4f, 8.5f, 8.6f, 8.7f, 8.8f, 8.9f, 11.1f, 11.2f, 18.5f
	};

	// dp_observations est les donnees dans l'enonce
	// dp_complet est une structure avec des valeurs de x
	// qu'on veut chercher, on va afficher ces valeurs
	// facilements plus tard
	dp_observations_taille=ARRAYSIZE(trois_series_3)/2;
	dp_complet_taille=ARRAYSIZE(trois_series_1_2_cherche_valeurs); // on divise pas par 2 ici parce que c n'est que des valeurs de x dans cet tableau

	// allocation pour le tableau donne dans l'enonce
	dp_observations=allocate_buffer(dp_observations_taille);

	// allocation pour le tableau dp_complet qui aura tous les valeurs manquant de l'enonce
	dp_complet=allocate_buffer(dp_complet_taille);

	// on remplit les valeurs donnees dans l'enonce dans notre structure d'observations
	fill_data_values(dp_observations, dp_observations_taille, trois_series_3);

	// affichage d'observations
	printf("OBSERVATIONS S3\n");
	display_data_pair(dp_observations, dp_observations_taille);

	// ici, on remplit la structure dp_complet avec les valeurs x manquant
	// et on va aussi remplir les valeurs y / f(x) correspondant aux valeurs x manquant
	remplit_dp_complet(dp_observations, dp_observations_taille, dp_complet, dp_complet_taille, trois_series_3_cherche_valeurs);

	// une fois remplit, on peut afficher des valeurs ici en utilisant `display_data_pair`
	printf("S3 VALEURS X MANQUANT\n");
	display_data_pair(dp_complet, dp_complet_taille);

	// pour la stabilite de la fonction
	printf("EN AJOUTANT %f POUR LA STABILITE, RESULTAT\n", delta);
	stabilite(dp_observations, 0, delta);

	// si necessaire d'afficher les valeurs apres changement, decommente la ligne suivante
	//display_data_pair(dp_observations, dp_observations_taille);

	printf("APRES LA FONCTION STABILITE\n");
	remplit_dp_complet(dp_observations, dp_observations_taille, dp_complet, dp_complet_taille, trois_series_3_cherche_valeurs);
	display_data_pair(dp_complet, 1);

	// on libere la memoire
	deallocate_buffer((void**)&dp_observations);
	deallocate_buffer((void**)&dp_complet);
	
	// PARTIE 3.3
	double revenus_depense_1[]=
	{
		752.0f, 85.0f, 855.0f, 83.0f, 871.0f, 162.0f, 734.0f, 79.0f, 610.0f, 81.0f, 582.0f, 83.0f, 921.0f, 281.0f, 492.0f, 81.0f, 569.0f, 81.0f, 462.0f, 80.0f, 907.0f, 243.0f
	};
	
	// valeurs de x qu'on veut chercher
	// peut ajouter ou enlever des valeurs ici
	// ceci est pour les Xi S3
	double revenus_depense_valeurs_cherche_1[]=
	{
		803.5f, 861.0f, 700.0f, 500.0f, 615.0f
	};

	// dp_observations est les donnees dans l'enonce
	// dp_complet est une structure avec des valeurs de x
	// qu'on veut chercher, on va afficher ces valeurs
	// facilements plus tard
	dp_observations_taille=ARRAYSIZE(revenus_depense_1)/2;
	dp_complet_taille=ARRAYSIZE(revenus_depense_valeurs_cherche_1); // on divise pas par 2 ici parce que c n'est que des valeurs de x dans cet tableau

	// allocation pour le tableau donne dans l'enonce
	dp_observations=allocate_buffer(dp_observations_taille);

	// allocation pour le tableau dp_complet qui aura tous les valeurs manquant de l'enonce
	dp_complet=allocate_buffer(dp_complet_taille);

	// on remplit les valeurs donnees dans l'enonce dans notre structure d'observations
	fill_data_values(dp_observations, dp_observations_taille, revenus_depense_1);

	// affichage d'observations
	printf("OBSERVATIONS REVENUS 1\n");
	display_data_pair(dp_observations, dp_observations_taille);

	// ici, on remplit la structure dp_complet avec les valeurs x manquant
	// et on va aussi remplir les valeurs y / f(x) correspondant aux valeurs x manquant
	remplit_dp_complet(dp_observations, dp_observations_taille, dp_complet, dp_complet_taille, revenus_depense_valeurs_cherche_1);

	// une fois remplit, on peut afficher des valeurs ici en utilisant `display_data_pair`
	printf("REVENUS 1 VALEURS X MANQUANT\n");
	display_data_pair(dp_complet, dp_complet_taille);

	// pour la stabilite de la fonction
	printf("EN AJOUTANT %f POUR LA STABILITE, RESULTAT\n", delta);
	stabilite(dp_observations, 0, delta);

	// si necessaire d'afficher les valeurs apres changement, decommente la ligne suivante
	//display_data_pair(dp_observations, dp_observations_taille);

	printf("APRES LA FONCTION STABILITE\n");
	remplit_dp_complet(dp_observations, dp_observations_taille, dp_complet, dp_complet_taille, revenus_depense_valeurs_cherche_1);
	display_data_pair(dp_complet, 1);

	// on libere la memoire
	deallocate_buffer((void**)&dp_observations);
	deallocate_buffer((void**)&dp_complet);
	
	// PARTIE 3.3
	double revenus_depense_2[]=
	{
		643.0, 84.0f, 862.0f, 84.0f, 524.0f, 82.0f, 679.0f, 80.0f, 902.0f, 226.0f, 918.0f, 260.0f, 828.0f, 82.0f, 875.0f, 186.0f, 809.0f, 77.0f, 894.0f, 223.0f
	};
	
	// valeurs de x qu'on veut chercher
	// peut ajouter ou enlever des valeurs ici
	// ceci est pour les Xi S3
	double revenus_depense_valeurs_cherche_2[]=
	{
		900.0f, 679.0f, 830.0f, 890.0f
	};

	// dp_observations est les donnees dans l'enonce
	// dp_complet est une structure avec des valeurs de x
	// qu'on veut chercher, on va afficher ces valeurs
	// facilements plus tard
	dp_observations_taille=ARRAYSIZE(revenus_depense_2)/2;
	dp_complet_taille=ARRAYSIZE(revenus_depense_valeurs_cherche_2); // on divise pas par 2 ici parce que c n'est que des valeurs de x dans cet tableau

	// allocation pour le tableau donne dans l'enonce
	dp_observations=allocate_buffer(dp_observations_taille);

	// allocation pour le tableau dp_complet qui aura tous les valeurs manquant de l'enonce
	dp_complet=allocate_buffer(dp_complet_taille);

	// on remplit les valeurs donnees dans l'enonce dans notre structure d'observations
	fill_data_values(dp_observations, dp_observations_taille, revenus_depense_2);

	// affichage d'observations
	printf("OBSERVATIONS REVENUS 2\n");
	display_data_pair(dp_observations, dp_observations_taille);

	// ici, on remplit la structure dp_complet avec les valeurs x manquant
	// et on va aussi remplir les valeurs y / f(x) correspondant aux valeurs x manquant
	remplit_dp_complet(dp_observations, dp_observations_taille, dp_complet, dp_complet_taille, revenus_depense_valeurs_cherche_2);

	// une fois remplit, on peut afficher des valeurs ici en utilisant `display_data_pair`
	printf("REVENUS 2 VALEURS X MANQUANT\n");
	display_data_pair(dp_complet, dp_complet_taille);

	// pour la stabilite de la fonction
	printf("EN AJOUTANT %f POUR LA STABILITE, RESULTAT\n", delta);
	stabilite(dp_observations, 0, delta);

	// si necessaire d'afficher les valeurs apres changement, decommente la ligne suivante
	//display_data_pair(dp_observations, dp_observations_taille);

	printf("APRES LA FONCTION STABILITE\n");
	remplit_dp_complet(dp_observations, dp_observations_taille, dp_complet, dp_complet_taille, revenus_depense_valeurs_cherche_2);
	display_data_pair(dp_complet, 1);

	// on libere la memoire
	deallocate_buffer((void**)&dp_observations);
	deallocate_buffer((void**)&dp_complet);	

	getchar();
	return 0;
}
