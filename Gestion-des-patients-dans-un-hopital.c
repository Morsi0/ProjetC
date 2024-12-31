#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOMBRE_CHAMBRES 100

typedef struct Patient {
    char cin[15];
    char nom[50];
    int age;
    char sexe[10];
    char diagnostic[100];
    int chambre;
    struct Patient* suivant;
} Patient;

Patient* ajouterPatient(Patient* tete) {
    Patient* nouveau = (Patient*)malloc(sizeof(Patient));
    if (nouveau == NULL) {
        printf("Erreur d'allocation mémoire.\n");
        return tete;
    }

    printf("CIN : ");
    scanf("%s", nouveau->cin);
    printf("Nom : ");
    scanf("%s", nouveau->nom);
    printf("Âge : ");
    scanf("%d", &nouveau->age);
    printf("Sexe (Homme/Femme) : ");
    scanf("%s", nouveau->sexe);
    printf("Diagnostic : ");
    scanf("%s", nouveau->diagnostic);
    printf("Numéro de chambre (1 à %d, ou 0 si non assignée) : ", NOMBRE_CHAMBRES);
    scanf("%d", &nouveau->chambre);

    nouveau->suivant = tete;
    printf("Patient ajouté avec succès !\n");
    return nouveau;
}

void afficherPatients(Patient* tete) {
    if (tete == NULL) {
        printf("Aucun patient enregistré.\n");
        return;
    }

    printf("\nListe des patients :\n");
    Patient* courant = tete;
    while (courant != NULL) {
        printf("CIN: %s | Nom: %s | Âge: %d | Sexe: %s | Diagnostic: %s | Chambre: %d\n",
               courant->cin, courant->nom, courant->age, courant->sexe, courant->diagnostic, courant->chambre);
        courant = courant->suivant;
    }
}

void rechercherPatient(Patient* tete, char cin[]) {
    Patient* courant = tete;
    while (courant != NULL) {
        if (strcmp(courant->cin, cin) == 0) {
            printf("Patient trouvé :\n");
            printf("CIN: %s | Nom: %s | Âge: %d | Sexe: %s | Diagnostic: %s | Chambre: %d\n",
                   courant->cin, courant->nom, courant->age, courant->sexe, courant->diagnostic, courant->chambre);
            return;
        }
        courant = courant->suivant;
    }
    printf("Aucun patient trouvé avec la CIN %s.\n", cin);
}

void modifierPatient(Patient* tete, char cin[]) {
    Patient* courant = tete;
    while (courant != NULL) {
        if (strcmp(courant->cin, cin) == 0) {
            printf("Modifier les informations du patient (CIN: %s):\n", cin);
            printf("Nom [%s] : ", courant->nom);
            scanf("%s", courant->nom);
            printf("Âge [%d] : ", courant->age);
            scanf("%d", &courant->age);
            printf("Sexe [%s] : ", courant->sexe);
            scanf("%s", courant->sexe);
            printf("Diagnostic [%s] : ", courant->diagnostic);
            scanf("%s", courant->diagnostic);
            printf("Numéro de chambre [%d] : ", courant->chambre);
            scanf("%d", &courant->chambre);
            printf("Patient modifié avec succès !\n");
            return;
        }
        courant = courant->suivant;
    }
    printf("Aucun patient trouvé avec la CIN %s.\n", cin);
}

Patient* supprimerPatient(Patient* tete, char cin[]) {
    if (tete == NULL) {
        printf("Aucun patient à supprimer.\n");
        return NULL;
    }

    if (strcmp(tete->cin, cin) == 0) {
        Patient* temp = tete;
        tete = tete->suivant;
        free(temp);
        printf("Patient avec CIN %s supprimé.\n", cin);
        return tete;
    }

    Patient* courant = tete;
    while (courant->suivant != NULL && strcmp(courant->suivant->cin, cin) != 0) {
        courant = courant->suivant;
    }

    if (courant->suivant != NULL) {
        Patient* temp = courant->suivant;
        courant->suivant = temp->suivant;
        free(temp);
        printf("Patient avec CIN %s supprimé.\n", cin);
    } else {
        printf("Aucun patient trouvé avec la CIN %s.\n", cin);
    }

    return tete;
}

Patient* chargerPatients() {
    char nomFichier[50];
    printf("Entrez le nom du fichier à charger : ");
    scanf("%s", nomFichier);

    FILE* fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s.\n", nomFichier);
        return NULL;
    }

    Patient* tete = NULL;
    while (!feof(fichier)) {
        Patient* nouveau = (Patient*)malloc(sizeof(Patient));
        if (nouveau == NULL) {
            printf("Erreur d'allocation mémoire.\n");
            fclose(fichier);
            return tete;
        }

        if (fscanf(fichier, "%s %s %d %s %s %d", nouveau->cin, nouveau->nom, &nouveau->age, 
                   nouveau->sexe, nouveau->diagnostic, &nouveau->chambre) == 6) {
            nouveau->suivant = tete;
            tete = nouveau;
        } else {
            free(nouveau);
        }
    }

    fclose(fichier);
    printf("Patients chargés avec succès depuis le fichier %s.\n", nomFichier);
    return tete;
}

void sauvegarderPatients(Patient* tete) {
    char nomFichier[50];
    printf("Entrez le nom du fichier pour sauvegarder : ");
    scanf("%s", nomFichier);

    FILE* fichier = fopen(nomFichier, "w");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s.\n", nomFichier);
        return;
    }

    Patient* courant = tete;
    while (courant != NULL) {
        fprintf(fichier, "%s %s %d %s %s %d\n", courant->cin, courant->nom, courant->age, 
                courant->sexe, courant->diagnostic, courant->chambre);
        courant = courant->suivant;
    }

    fclose(fichier);
    printf("Patients sauvegardés avec succès dans le fichier %s.\n", nomFichier);
}

void afficherChambres(Patient* tete) {
    int chambresOccupees[NOMBRE_CHAMBRES] = {0};

    Patient* courant = tete;
    while (courant != NULL) {
        if (courant->chambre > 0 && courant->chambre <= NOMBRE_CHAMBRES) {
            chambresOccupees[courant->chambre - 1] = 1;
        }
        courant = courant->suivant;
    }

    printf("\nChambres occupées :\n");
    int chambreTrouvee = 0;
    for (int i = 0; i < NOMBRE_CHAMBRES; i++) {
        if (chambresOccupees[i]) {
            printf("Chambre %d : Occupée\n", i + 1);
            chambreTrouvee = 1;
        }
    }
    if (!chambreTrouvee) {
        printf("Aucune chambre occupée.\n");
    }

    printf("\nChambres libres :\n");
    chambreTrouvee = 0;
    for (int i = 0; i < NOMBRE_CHAMBRES; i++) {
        if (!chambresOccupees[i]) {
            printf("Chambre %d : Libre\n", i + 1);
            chambreTrouvee = 1;
        }
    }
    if (!chambreTrouvee) {
        printf("Aucune chambre libre.\n");
    }
}

int main() {
    Patient* tete = NULL;
    int choix;
    char cin[15];

    do {
        printf("\n================ Menu de gestion des patients =================\n");
        printf("1. Ajouter un patient\n");
        printf("2. Afficher tous les patients\n");
        printf("3. Rechercher un patient\n");
        printf("4. Modifier un patient\n");
        printf("5. Supprimer un patient\n");
        printf("6. Charger les patients\n");
        printf("7. Sauvegarder les patients\n");
        printf("8. Afficher les chambres occupées/libres\n");
        printf("0. Quitter\n");
        printf("==============================================================\n");
        printf("Choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                tete = ajouterPatient(tete);
                break;
            case 2:
                afficherPatients(tete);
                break;
            case 3:
                printf("CIN du patient à rechercher : ");
                scanf("%s", cin);
                rechercherPatient(tete, cin);
                break;
            case 4:
                printf("CIN du patient à modifier : ");
                scanf("%s", cin);
                modifierPatient(tete, cin);
                break;
            case 5:
                printf("CIN du patient à supprimer : ");
                scanf("%s", cin);
                tete = supprimerPatient(tete, cin);
                break;
            case 6:
                tete = chargerPatients();
                break;
            case 7:
                sauvegarderPatients(tete);
                break;
            case 8:
                afficherChambres(tete);
                break;
            case 0:
                printf("Programme terminé.\n");
                break;
            default:
                printf("Choix invalide.\n");
        }
    } while (choix != 0);

    return 0;
}