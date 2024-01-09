#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define NB_ESSAIS 8
#define SCORES_FILE "C:/Users/akira/Documents/projet/bin/scores.txt"
#define MOTS_FILE "C:/Users/akira/Documents/projet/bin/words.txt"

// Fonction pour saisir le nom du joueur
char *nomJoueur(void)
{
    char *nom = (char *)malloc(30 * sizeof(char)); // Alloue dynamiquement de la mémoire pour stocker le nom
    if (nom == NULL)
    {
        perror("Erreur d'allocation de mémoire !");
        exit(EXIT_FAILURE);
    }

    printf("Entrez votre nom de joueur : ");
    scanf("%s", nom);
    printf("Bonjour %s, bienvenue sur le jeu du pendu !\n",nom);

    return nom; // Renvoie un pointeur vers le nom
}

// Fonction pour choisir un mot aléatoire depuis un fichier
char *choisirMot()
{
    char ligne[1000];
    int nombreMots = 0;
    char *mot = NULL;
    FILE *fichier = fopen(MOTS_FILE, "r");
    if (fichier == NULL)
    {
        perror("Impossible d'ouvrir le fichier de mots !");
        return NULL;
    }

    while (fgets(ligne, sizeof(ligne), fichier) != NULL)
    {
        if (strlen(ligne) >= 5)
        {
            nombreMots++;
        }
    }

    if (nombreMots == 0)
    {
        fclose(fichier);
        return NULL;
    }

    rewind(fichier); // Permet de réinitialiser la position du curseur de lecture dans le fichier associé au pointeur fichier

    srand(time(NULL));
    int choix = rand() % nombreMots;

    for (int i = 0; i <= choix; i++) // Parcourt le fichier jusqu'à l'indice aléatoire "choix"
    {
        if (fgets(ligne, sizeof(ligne), fichier) == NULL) // Lecture de la ligne suivante dans le fichier
        {
            perror("Erreur de lecture du fichier de mots !");
            fclose(fichier);
            return NULL;
        }
    }

    size_t len = strlen(ligne);
    if (len > 0 && ligne[len - 1] == '\n')
    {
        ligne[len - 1] = '\0';
    }

    mot = (char *)malloc(len + 1); // Alloue de la mémoire pour stocker le mot choisit
    if (mot == NULL)
    {
        perror("Erreur d'allocation de mémoire !");
        fclose(fichier);
        return NULL;
    }

    strcpy(mot, ligne); // Copie du contenu de la chaîne de caractères "ligne" dans la chaîne de caractères "mot"

    fclose(fichier);
    return mot;
}

// Affiche le support du pendu en fonction du nombre d'erreurs
void afficherPendu(int erreurs)
{
    // Affichage statique du support du pendu
    printf("   ______\n");
    printf("  |      |\n");

    // Utilisation d'une instruction switch pour afficher différentes parties du pendu en fonction du nombre d'erreurs
    switch (erreurs)
    {
    case 0:
        printf("  |\n");
        printf("  |\n");
        printf("  |\n");
        break;
    case 1:
        printf("  |      O\n");
        printf("  |\n");
        printf("  |\n");
        break;
    case 2:
        printf("  |      O\n");
        printf("  |      |\n");
        printf("  |\n");
        break;
    case 3:
        printf("  |      O\n");
        printf("  |     /|\n");
        printf("  |\n");
        break;
    case 4:
        printf("  |      O\n");
        printf("  |     /|\\\n");
        printf("  |\n");
        break;
    case 5:
        printf("  |      O\n");
        printf("  |     /|\\\n");
        printf("  |      |\n");
        break;
    case 6:
        printf("  |      O\n");
        printf("  |     /|\\\n");
        printf("  |      |\n");
        printf("  |     /\n");
        break;
    case 7:
        printf("  |      O\n");
        printf("  |     /|\\\n");
        printf("  |      |\n");
        printf("  |     / \\\n");
        break;
    case 8:
        printf("  |      O\n");
        printf("  |     /|\\\n");
        printf("  |      |\n");
        printf("  |     / \\\n");
        break;
    default:
        // Cas par défaut pour un affichage minimal en cas d'erreurs inattendues
        printf("  |\n");
        printf("  |\n");
        break;
    }
    // Partie inférieure du support du pendu
    printf(" _|_\n");
    printf("|   |");
    printf("_______\n");
    printf("|           |\n");
    printf("|___________|\n");
}

// Enregistre le nom du joueur, le mot secret et le score dans un fichier
void enregistrerScore(const char *nomJoueur, const char *motSecret, int score)
{
    // Ouverture du fichier des scores en mode ajout ("at" pour ajout texte)
    FILE *fichierScores = fopen(SCORES_FILE, "at");

    if (fichierScores == NULL)
    {
        perror("Impossible d'ouvrir ou de créer le fichier des scores !");
        return;
    }

    fprintf(fichierScores, "%s %s %d\n", nomJoueur, motSecret, score);

    fclose(fichierScores);
}

// Structure pour stocker le score du joueur
struct Score
{
    char nom[30];
    char motSecret[30];
    int score;
};

// Fonction pour afficher le TOP 10 des scores depuis le fichier
void afficherTop10()
{
    FILE *fichierScores = fopen(SCORES_FILE, "r");

    if (fichierScores == NULL)
    {
        perror("Impossible d'ouvrir le fichier des scores !");
        return;
    }

    // Utilisation d'un tableau de structures pour stocker les scores
    struct Score *topScores = NULL;
    int capacite = 10; // Capacité initiale du tableau
    int taille = 0;

    // Allocation initiale de la mémoire pour le tableau
    topScores = (struct Score *)malloc(capacite * sizeof(struct Score));

    if (topScores == NULL)
    {
        fclose(fichierScores);
        perror("Erreur d'allocation de mémoire !");
        return;
    }

    // Lecture de toutes les lignes du fichier
    while (fscanf(fichierScores, "%s %s %d", topScores[taille].nom, topScores[taille].motSecret, &topScores[taille].score) == 3)
    {
        taille++;

        // Si la taille atteint la capacité, réallouer de la mémoire pour le tableau
        if (taille == capacite)
        {
            capacite *= 2; // Double la capacité
            topScores = (struct Score *)realloc(topScores, capacite * sizeof(struct Score));

            if (topScores == NULL)
            {
                fclose(fichierScores);
                perror("Erreur de réallocation de mémoire !");
                return;
            }
        }
    }

    fclose(fichierScores);

    // Tri du tableau en fonction du score
    for (int j = 0; j < taille - 1; j++)
    {
        int maxIndex = j;
        for (int k = j + 1; k < taille; k++)
        {
            if (topScores[k].score > topScores[maxIndex].score)
            {
                maxIndex = k;
            }
        }

        // Échange des positions des éléments dans le tableau
        struct Score temp = topScores[j];
        topScores[j] = topScores[maxIndex];
        topScores[maxIndex] = temp;
    }

    printf("\n_________ TOP 10 _________\n\n");

    // Affichage du vrai top 10
    int affichageMax = (taille < 10) ? taille : 10;
    for (int j = 0; j < affichageMax; j++)
    {
        printf("Joueur: %s, Mot: %s, Score: %d\n\n", topScores[j].nom, topScores[j].motSecret, topScores[j].score);
    }

    // Libération de la mémoire allouée pour le tableau
    free(topScores);
}

// Fonction principale pour jouer au pendu
int jouerPendu(const char *motSecret)
{
    char *nom = nomJoueur();
    int tailleMot = strlen(motSecret);

    char *motCourant = (char *)malloc(tailleMot + 1); // Alloue de la mémoire pour stocker le mot courant
    if (motCourant == NULL)
    {
        perror("Erreur d'allocation de mémoire !");
        return 1;
    }

    for (int i = 0; i < tailleMot; i++) // Initialisation de la chaîne de caractères "motCourant" avec des underscores
    {
        motCourant[i] = '_';
    }
    motCourant[tailleMot] = '\0'; // Ajout du caractère nul ('\0') à la fin de la chaîne de caractères "motCourant"

    // Initialisation du nombre maximal de tentatives et du nombre de tentatives restantes
    int tentativesMax = NB_ESSAIS;
    int tentativesRestantes = tentativesMax;

    char lettre;

    // Initialisation du tableau "lettresDejaDevinees" pour suivre les lettres déjà devinées
    // Chaque élément du tableau est initialisé à 0 (false), indiquant qu'aucune lettre n'a encore été devinée
    char lettresDejaDevinees[26] = {0};

    while (tentativesRestantes > 0)
    {
        afficherPendu(tentativesMax - tentativesRestantes); // Affiche le pendu et le mot courant à chaque étape

        printf("Mot actuel : %s\n", motCourant); // Affichage du mot actuel (partiellement deviné) pendant le jeu

        printf("Entrez une lettre : "); // Affichage du message invitant le joueur à entrer une lettre le joueur doit saisir une seul lettre de l'alphabet français et pas de chiffre

        // Lecture de la lettre entrée par le joueur
        if (scanf(" %c", &lettre) != 1)
        {
            fprintf(stderr, "Erreur de saisie.\n");

            free(motCourant);
            free(nom);

            return 1; // Retourne 1 pour indiquer une erreur
        }

        lettre = toupper(lettre);

        // Vérification si la lettre a déjà été devinée
        if (lettresDejaDevinees[lettre - 'A'] == 1) // Lorsqu'on soustrait 'A' de la lettre, cela permet de normaliser la lettre à la plage des indices du tableau
        {
            printf("Vous avez déjà deviné cette lettre. Essayez une autre.\n");

            continue;
        }

        // Marque la lettre comme déjà devinée en mettant la valeur correspondante à 1 dans le tableau
        lettresDejaDevinees[lettre - 'A'] = 1;

        // Recherche de la lettre dans le mot secret et mise à jour du mot courant
        int lettreTrouvee = 0;
        for (int i = 0; i < tailleMot; i++)
        {
            // Vérification si la lettre correspond à un caractère dans le mot secret
            if (motSecret[i] == lettre)
            {
                // Mise à jour du mot courant en remplaçant l'underscore par la lettre trouvée
                motCourant[i] = lettre;

                // Marque la lettre comme trouvée
                lettreTrouvee = 1;
            }
        }

        if (!lettreTrouvee) // Vérification si la lettre n'a pas été trouvée dans le mot secret
        {
            // Réduction du nombre de tentatives restantes
            tentativesRestantes--;

            printf("Lettre incorrecte. Tentatives restantes : %d\n", tentativesRestantes);
        }

        // Vérification si le mot courant est égal au mot secret, indiquant que le joueur a trouvé le mot
        if (strcmp(motCourant, motSecret) == 0)
        {
            printf("Félicitations, vous avez trouvé le mot : %s\n", motSecret);

            enregistrerScore(nom, motCourant, tentativesRestantes);

            free(motCourant);
            free(nom);

            return 0;
        }
    }

    afficherPendu(tentativesMax);

    printf("Désolé, vous avez épuisé toutes vos tentatives. Le mot était : %s\n", motSecret);

    enregistrerScore(nom, motSecret, tentativesRestantes);

    free(motCourant);
    free(nom);

    return 0;
}

// Fonction principale du programme
int main(void)
{
    const char *motSecret = choisirMot(); // Choix du mot secret

    int resultat = jouerPendu(motSecret); // Appel de la fonction pour jouer au pendu avec le mot choisi

    // Affichage du TOP 10 seulement si le joueur a perdu
    if (resultat == 0)
    {
        afficherTop10();
    }

    return 0; // Retourne 0 pour indiquer la fin du programme sans erreurs
}