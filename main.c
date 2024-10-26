#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h> // Pour utiliser qsort
#pragma warning(disable:4996)

// ENUMERATIONS, STRUCTURES
enum { LONGUEUR_MAX = 30, MAX_ETUDIANT = 100, LONGUEUR_CHOIX = 20, LONGUEUR_JOUR = 2, MAX_ABSENCES = 20, LONGUEUR_JUSTIFICATIF = 50, MAX_JUSTIFICATIF = 20, MAX_OPTIONS = 2, MAX_OPTIONS_ABSENCE = 5 };

typedef struct {
    char nom[LONGUEUR_MAX];
    unsigned int num_groupe;
    unsigned int identifiant;

    char mom_journee[LONGUEUR_JOUR];
    unsigned int absences[MAX_ABSENCES][MAX_OPTIONS_ABSENCE]; // [][0] pour jour, [][1] pour moment (0 = am, 1 = pm), [][2] = 0 pour non justifiée, = 1 pour justifiée, [][3] identifiant attribué à l'absence
    unsigned int compteur_absence;

    char motif_justificatif[LONGUEUR_JUSTIFICATIF]; // le motif en tout lettre
    unsigned int justificatif[MAX_JUSTIFICATIF][MAX_OPTIONS]; // [identifiant][]
    unsigned int compteur_justificatif;

} Etudiant;

// CO Exit
void saisir_choix(char *choix) {
    scanf("%s", choix);
}

// C1 Inscription
// Méthode le nom, et le numero de groupe
void saisie_nom_groupe(char nom[], unsigned int *numero_groupe) {
    scanf("%s", nom);
    scanf("%u", numero_groupe);
}
// Méthode pour inscrire un étudiant
void saisir_inscription(Etudiant etudiant[], unsigned int *compteur_etudiant) {
    // Si nombre maximum d'étudiants atteint
    if (*compteur_etudiant < MAX_ETUDIANT) {
        // Appelle de fonction pour s'inscrire
        saisie_nom_groupe(etudiant[*compteur_etudiant].nom, &etudiant[*compteur_etudiant].num_groupe);

        // L'étudiant reçoit son identifiant
        etudiant[*compteur_etudiant].identifiant = *compteur_etudiant + 1;

        // Incrémenter le compteur d'étudiants
        (*compteur_etudiant)++;

    } else {
        printf("Nombre maximum d'etudiant atteint\n");
    }
}
// Vérifie si l'inscription est correcte, sinon décrémente le compteur d'étudiant
void verifier_inscription(Etudiant etudiants[], unsigned int *compteur_etudiant) {
    for (int i = 0; i < *compteur_etudiant - 1; i++) {
        if (strcmp(etudiants[i].nom, etudiants[*compteur_etudiant - 1].nom) == 0
            && etudiants[i].num_groupe == etudiants[*compteur_etudiant - 1].num_groupe) {
            printf("Inscription incorrecte\n");
            // Efface l'inscription
            (*compteur_etudiant)--;
            strcpy(etudiants[*compteur_etudiant].nom, "");
            etudiants[*compteur_etudiant].num_groupe = 0;
            return;
        }
    }
    printf("Inscription enregistree (%u)\n", *compteur_etudiant);
}


// C2 Absence
// Méthode de saisie de l'identifiant
// A CORRIGER ABSOULUMENT
void saisir_identifiant(unsigned int *id) {
    scanf("%u", id);
}
// Méthode de saisie du numéro du jour
void saisir_jour_absence(unsigned int *nb_jour) {
    scanf("%u", nb_jour);

    // Vérifie si le nombre est valide
    if (*nb_jour < 1 || *nb_jour > 40) {
        printf("Date incorrecte\n");
        *nb_jour = 0; // On peut choisir de mettre à zéro pour indiquer l'absence d'enregistrement
    }
}
    /*
    do {

    } while (*nb_jour < 1 || *nb_jour > 40);  // Répéter tant que l'entrée n'est pas valide*/

// Méthode de saisie du moment de la journée
void saisir_moment_journee(char moment_jour[]) {
    do {
        scanf("%s", moment_jour);

        if (strcmp(moment_jour, "am") != 0 && strcmp(moment_jour, "pm") != 0) {
            printf("Date incorrecte.\n");
        }
    } while (strcmp(moment_jour, "am") != 0 && strcmp(moment_jour, "pm") != 0);
}
// Méthode qui vérifie qu'il n'y a pas de doublons
void verifier_absence(Etudiant *etudiant, unsigned int id, unsigned int jour, char *moment, unsigned int *total_absence) {
    for (int i = 0; i < etudiant[id - 1].compteur_absence; i++) {
        // Compare le jour et le moment de la journée (am/pm)
        if (etudiant[id - 1].absences[i][0] == jour &&
            ((strcmp(moment, "am") == 0 && etudiant[id - 1].absences[i][1] == 0) ||
             (strcmp(moment, "pm") == 0 && etudiant[id - 1].absences[i][1] == 1))) {
            printf("Absence déjà enregistrée\n");
            return;  // Si une absence existe déjà pour ce jour et moment, on quitte
             }
    }

    // Enregistrer la nouvelle absence
    int index_absence = etudiant[id - 1].compteur_absence;
    etudiant[id - 1].absences[index_absence][0] = jour;
    etudiant[id - 1].absences[index_absence][1] = (strcmp(moment, "am") == 0) ? 0 : 1;
    etudiant[id - 1].absences[index_absence][2] = 0;  // Absence non justifiée
    etudiant[id - 1].absences[index_absence][3] = ++(*total_absence);  // Identifiant d'absence unique

    etudiant[id - 1].compteur_absence++;  // Incrémenter le compteur d'absences

    printf("Absence enregistree [%u]\n", *total_absence);
}

// C3 Etudiants
// Méthode de saisie du jour
// À CORRIGER ABOSLUMENBT
void saisir_jour(unsigned int *nb_jour) {
    scanf("%u", nb_jour);
    if (*nb_jour < 1) {
        printf("Date incorrecte.\n");
        *nb_jour = 0;
    }
    /*
    do {


    } while (*nb_jour < 1);  // Répéter tant que l'entrée n'est pas valide*/
}
// Pour trier dans le main via qsort
int comparer_etudiants(const void *a, const void *b) {
    Etudiant *etudiant_a = (Etudiant *)a;
    Etudiant *etudiant_b = (Etudiant *)b;

    // Comparer  les numéros de groupe
    if (etudiant_a->num_groupe < etudiant_b->num_groupe) {
        return -1;
    } else if (etudiant_a->num_groupe > etudiant_b->num_groupe) {
        return 1; // etudiant_b vient avant etudiant_a
    } else {
        // Comparer les noms
        return strcmp(etudiant_a->nom, etudiant_b->nom);
    }
}
// Affichage des inscrits
void afficher_toutes_inscriptions(Etudiant *etudiant, unsigned int compteur_etudiant) {
    if (compteur_etudiant == 0) {
        printf("Aucun inscrit.\n");
    }
    else {

        printf("%-30s %10s %10s\n", " ", " ", " ");  // En-tête avec alignement
        for (int i = 0; i < compteur_etudiant; i++) {
            printf("(%u) %-10s %5u %5u\n", (i + 1), etudiant[i].nom, etudiant[i].num_groupe, etudiant[i].compteur_absence);
        }
    }
}



// C4 Justificatif
// CETTE METHODE POSE PROBLEME ET NE DETECTE PAS LA 2E ABSENCE
// Méthode de saisie du numero de l'absence
void saisir_absence_à_justifier(Etudiant *etudiant, unsigned int id, unsigned int* absence_à_justifier, unsigned int total_absence) {
    do {
        scanf("%u", absence_à_justifier);
    } while (*absence_à_justifier <= 0 || *absence_à_justifier > total_absence);
}
void recherche_id(Etudiant *etudiant, unsigned int compteur_etudiant, unsigned int absence_à_justifier, unsigned int *id) {
    for (int i = 0; i < compteur_etudiant; i++) {  // Parcourir tous les étudiants
        for (int j = 0; j < etudiant[i].compteur_absence; j++) {  // Parcourir les absences de chaque étudiant
            if (etudiant[i].absences[j][3] == absence_à_justifier) {  // Vérifie l'identifiant de l'absence
                *id = i + 1;  // L'ID est basé sur l'index + 1
                return;  // Sortir une fois l'étudiant trouvé
            }
        }
    }
}
void saisir_justificatif(char justificatif[], int taille_max) {
    int longueur;
    do {
        fgets(justificatif, taille_max, stdin);

        longueur = strlen(justificatif);

        // Si l'étudiant a saisi plus que la taille max, il n'y a pas de '\n', il faut vider le buffer
        if (justificatif[longueur - 1] != '\n') {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);  // Vide tout ce qui reste dans le buffer jusqu'à la fin de ligne
            printf("Justificatif trop long.\n");
        }
        else {
            // Si un '\n' est présent, on l'enlève
            justificatif[longueur - 1] = '\0';
        }

        longueur = strlen(justificatif);  // Recalcule la longueur

    } while (longueur < 1 || longueur >= taille_max - 1);
}
void verifier_justificatif(Etudiant *etudiant, unsigned int id, unsigned int absence_a_justifier, unsigned int numero_jour_justificatif) {
    if (etudiant[id - 1].absences[id - 1][2] == 1) {
        printf("Un justificatif a déjà été fourni pour cette absence\n");
    }
    else if (((etudiant[id - 1].absences[id - 1][0]) + 4) < numero_jour_justificatif) {
        printf("Absence non-justifiee, vous avez depasse(e) les 3 jours\n");
    }
    else {
        // Enregistrer le justificatif et incrémenter le compteur
        etudiant[id - 1].justificatif[etudiant[id - 1].compteur_justificatif][0] = absence_a_justifier;
        etudiant[id - 1].absences[id - 1][2] = 1;
        etudiant[id - 1].compteur_justificatif++;  // Incrémenter le compteur de justificatifs
        printf("Justificatif enregistree\n\n");
    }
}

// C5 Validations

void validation_en_attente(Etudiant *etudiant, unsigned int compteur_etudiant, unsigned int total_absence) {
    bool absences_à_valider = false;  // Booléen pour vérifier s'il y a des validations en attente

    for (int i = 0; i < compteur_etudiant; i++) {  // Parcourir tous les étudiants
        for (int j = 0; j < total_absence; j++) {  // Parcourir les absences de chaque étudiant
            if (etudiant[i].absences[j][2] == 1) {  // Si l'absence est justifiée (1 = justifiée)
                absences_à_valider = true;

                // Identifiant de l'absence
                unsigned int identifiant_absence = etudiant[i].absences[j][3];  // Corrigé ici pour obtenir l'identifiant

                // Numéro du jour et moment (am/pm)
                unsigned int jour_absence = etudiant[i].absences[j][0];
                const char *moment_journee = (etudiant[i].absences[j][1] == 0) ? "am" : "pm";

                // Justificatif
                char* justificatif = etudiant[i].motif_justificatif;

                // Affichage du résultat demandé
                printf("[%-3u] (%-2u) %-5s %5u %2u/%-2s (%s)\n",
                    identifiant_absence,               // Identifiant de l'absence entre crochets
                    etudiant[i].identifiant,           // Identifiant de l'étudiant entre parenthèses
                    etudiant[i].nom,                   // Nom de l'étudiant
                    etudiant[i].num_groupe,            // Groupe de l'étudiant
                    jour_absence,                      // Numéro du jour de l'absence
                    moment_journee,                    // Moment de la journée (am ou pm)
                    justificatif);                     // Justificatif entre parenthèses
            }
        }
    }
    if (!absences_à_valider) {
        printf("Aucune validation en attente.\n");
    }
}

// C6 Validation
void saisir_identifiant_absence(unsigned int *id_absence) {
    scanf("%u", id_absence);
}
void saisir_verdict(char* verdict) {
    scanf("%s", verdict);
}
void valider_justificatif(Etudiant *etudiant, unsigned int compteur_etudiant, unsigned int *id_absence, unsigned int total_absence) {
    char verdict[3];

    saisir_identifiant_absence(id_absence);

    // Recherche de l'absence à valider
    bool absence_trouvee = false;
    for (int i = 0; i < compteur_etudiant; i++) {  // Parcourir tous les étudiants
        for (int j = 0; j < total_absence; j++) {  // Parcourir les absences de chaque étudiant
            if (etudiant[i].absences[j][2] == 1) {  // Si l'absence est justifiée (1 = justifiée)
                absence_trouvee = true;

                // Vérification si absence en attente de validation
                if (etudiant[i].absences[j][2] == 1) {  // Absence justifiée mais en attente de validation
                    saisir_verdict(verdict);

                    if (strcmp(verdict, "ok") == 0) {
                        printf("validation [%u] ok\n", *id_absence);
                        printf("Validation enregistree\n");
                        etudiant[i].absences[j][2] = 1;  // Validation acceptée
                    }
                    else if (strcmp(verdict, "ko") == 0) {
                        printf("validation [%u] ko\n", *id_absence);
                        printf("Validation enregistree\n");
                        etudiant[i].absences[j][2] = 0;  // Validation refusée
                    }
                    else {
                        printf("Code incorrect\n");
                    }
                }
                else if (etudiant[i].absences[j][2] != -1) {  // Validation déjà faite
                    printf("Validation deja connue\n");
                }
                else {
                    printf("Absence non en attente de validation\n");
                }
                return;
            }
        }
    }

    if (!absence_trouvee) {
        printf("Identifiant incorrect\n");
    }
}

// C7 Etudiant
void afficher_statut_absences_etudiant(Etudiant *etudiant, unsigned int compteur_etudiant, unsigned int id, unsigned int jour_courant) {
    if (id < 1 || id > compteur_etudiant) {
        printf("Identifiant incorrect\n");
        return;
    }

    if (jour_courant < 1) {
        printf("Date incorrecte\n");
        return;
    }


    printf("etudiant %u\n", etudiant->identifiant);
    printf("(%u) %-10s %5u %5u\n\n", etudiant->identifiant, etudiant->nom, etudiant->num_groupe, etudiant->compteur_absence);

    bool section_affichee = false;

    // 1. En attente de justificatif
    for (int j = 0; j < compteur_etudiant; j++) {
        for (int i = 0; i < etudiant[j].compteur_absence; i++) {
            unsigned int jour_absence = etudiant[j].absences[i][0];
            if (jour_absence <= jour_courant && etudiant[j].absences[i][2] == 0) {
                if (!section_affichee) {
                    printf("En attente justificatif:\n");
                    section_affichee = true;
                }
                printf("[%u] %u/%s\n", etudiant[j].absences[i][3], jour_absence,
                    etudiant[j].absences[i][1] == 0 ? "am" : "pm");
            }
        }
    }
    if (section_affichee) printf("\n");
    section_affichee = false;

    // 2. En attente de validation
    for (int j = 0; j < compteur_etudiant; j++) {
        for (int i = 0; i < etudiant[j].compteur_absence; i++) {
            unsigned int jour_absence = etudiant[j].absences[i][0];
            if (jour_absence <= jour_courant && etudiant[j].absences[i][2] == 1) {
                if (!section_affichee) {
                    printf("En attente validation:\n");
                    section_affichee = true;
                }
                printf("[%u] %u/%s (%s)\n", etudiant[j].absences[i][3], jour_absence,
                    etudiant[j].absences[i][1] == 0 ? "am" : "pm", etudiant[j].motif_justificatif);
            }
        }
    }

    if (section_affichee) printf("\n");
    section_affichee = false;

    // 3. Absences justifiées
    for (int j = 0; j < compteur_etudiant; j++) {
        for (int i = 0; i < etudiant[j].compteur_absence; i++) {
            unsigned int jour_absence = etudiant[j].absences[i][0];
            if (jour_absence <= jour_courant && etudiant[j].absences[i][2] == 1) {
                if (!section_affichee) {
                    printf("Justifiees:\n");
                    section_affichee = true;
                }
                printf("[%u] %u/%s (%s)\n", etudiant[j].absences[i][3], jour_absence,
                    etudiant[j].absences[i][1] == 0 ? "am" : "pm", etudiant[j].motif_justificatif);
            }
        }
    }
    if (section_affichee) printf("\n");
    section_affichee = false;

    // 4. Absences non-justifiées
    for (int j = 0; j < compteur_etudiant; j++) {
        for (int i = 0; i < etudiant[j].compteur_absence; i++) {
            unsigned int jour_absence = etudiant[j].absences[i][0];
            if (jour_absence <= jour_courant && etudiant[j].absences[i][2] == 0 && etudiant[j].absences[i][2] == 0) {
                if (!section_affichee) {
                    printf("Non-justifiees:\n");
                    section_affichee = true;
                }
                printf("[%u] %u/%s\n", etudiant[j].absences[i][3], jour_absence,
                    etudiant[j].absences[i][1] == 0 ? "am" : "pm");
            }
        }
    }

    if (section_affichee) printf("\n");
}


// C8 Defaillant
void afficher_defaillants(Etudiant *etudiant, unsigned int compteur_etudiant, unsigned int jour_courant) {
    // Valider la date du jour courant
    if (jour_courant < 1) {
        printf("Date incorrecte\n");
        return;
    }

    bool defaillant_trouve = false;

    for (int i = 0; i < compteur_etudiant; i++) {
        unsigned int total_absences_non_justifiees = 0;

        // Compter les absences non justifiées pour chaque étudiant avant le jour courant
        for (int j = 0; j < etudiant[i].compteur_absence; j++) {
            unsigned int jour_absence = etudiant[i].absences[j][0];
            bool absence_non_justifiee = etudiant[i].absences[j][2] == 0;

            // Si l'absence est antérieure à la date du jour courant et non justifiée
            if (jour_absence < jour_courant && absence_non_justifiee) {
                total_absences_non_justifiees++;
            }
        }

        // Si l'étudiant a 5 absences ou plus (non justifiées), il est considéré comme défaillant
        if (total_absences_non_justifiees >= 5) {
            if (!defaillant_trouve) {
                printf("Liste des étudiants défaillants:\n");
                printf("%-30s %10s %10s\n", "Nom", "Groupe", "Total absences");
                defaillant_trouve = true;
            }
            printf("%-30s %10u %10u\n", etudiant[i].nom, etudiant[i].num_groupe, etudiant[i].compteur_absence);
        }
    }

    // Si aucun défaillant n'est trouvé
    if (!defaillant_trouve) {
        printf("Aucun defaillant\n");
    }
}

int main() {
    Etudiant etudiant[MAX_ETUDIANT];
    char choix[LONGUEUR_CHOIX];
    unsigned int compteur_étudiant = 0;
    unsigned int id = 0;
    unsigned int numero_jour_absence = 0;
    unsigned int total_absence = 0;
    unsigned int nb_jour = 1;
    unsigned int absence_a_justifier = 0;
    unsigned int numero_jour_justificatif = 0;
// gerer les erreurs de si pas inscrit etc...
    while (1) {
        saisir_choix(choix);
        if (strcmp(choix, "exit") == 0) {
            break;
        }
        else if (strcmp(choix, "inscription") == 0) {
            saisir_inscription(etudiant, &compteur_étudiant);
            verifier_inscription(etudiant, &compteur_étudiant);
        }
        else if (strcmp(choix, "absence") == 0) {
            saisir_identifiant(&id);
            saisir_jour_absence(&numero_jour_absence);
            saisir_moment_journee(etudiant[(id - 1)].mom_journee);
            verifier_absence(etudiant, id, numero_jour_absence, etudiant[(id - 1)].mom_journee, &total_absence);
        }
        else if (strcmp(choix, "etudiants") == 0) {
            saisir_jour(&nb_jour);
            qsort(etudiant, compteur_étudiant, sizeof(Etudiant), comparer_etudiants);
            afficher_toutes_inscriptions(etudiant, compteur_étudiant);

        }
        else if (strcmp(choix, "justificatif") == 0) {
            saisir_absence_à_justifier(etudiant, id, &absence_a_justifier, total_absence);
            recherche_id(etudiant, compteur_étudiant, absence_a_justifier, &id);
            saisir_jour(&numero_jour_justificatif);
            saisir_justificatif(etudiant[id - 1].motif_justificatif, LONGUEUR_JUSTIFICATIF);
            verifier_justificatif(etudiant, id, absence_a_justifier, numero_jour_justificatif);
        }
        else if (strcmp(choix, "validations") == 0) {
            validation_en_attente(etudiant, compteur_étudiant, total_absence);
        }
        else if (strcmp(choix, "validation") == 0) {
            valider_justificatif(etudiant, compteur_étudiant, &absence_a_justifier, total_absence);
        }
        else if (strcmp(choix, "etudiant") == 0) {
            unsigned int jour_courant;
            saisir_identifiant(&id);;
            saisir_jour(&jour_courant);
            afficher_statut_absences_etudiant(etudiant, compteur_étudiant, id, jour_courant);
        }
        else if (strcmp(choix, "defaillants") == 0) {
            unsigned int jour_courant;
            saisir_jour(&jour_courant);
            afficher_defaillants(etudiant, compteur_étudiant, jour_courant);
        }
        else {
            printf("Choix incorrect\n");
        }
    }
}