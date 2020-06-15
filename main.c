#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// 14h 10m work time
#define MAX 101

typedef struct {
    char krstne[MAX];
    char priezvisko[MAX];
} MENO;
typedef struct herec {
    MENO herca;
    int vek;
    struct herec *p_dalsi_herec;
} HEREC;
typedef struct film {
    char nazov[MAX];
    int rok;
    char reziser[MAX];
    HEREC *herec;
    struct film *p_dalsi_film;
} FILM;

FILM *nacitaj();
void vypis(FILM *zaciatok);
FILM *pridaj(FILM *zaciatok);
FILM *vymaz(FILM *zaciatok);
void filmy(FILM *zaciatok);
void herci(FILM *zaciatok);
void rok(FILM *zaciatok);
FILM *uvolni(FILM *zaciatok);
int skontroluj(HEREC *zaciatok_zoznamu, HEREC *p_herec_origo);


FILM *nacitaj() {
    FILE *fr;
    FILM *kniznica_film = NULL;
    FILM *temp_film = NULL;
    FILM *p_film = NULL;
    HEREC *temp_herec = NULL;
    HEREC *p_herec = NULL;


    char meno_filmu[MAX], reziser_filmu[MAX];
    int rok_filmu, i = 0;
    char c;

    if ((fr = fopen("filmy.txt", "r")) == NULL) {
        printf("Chyba pri otvarani suboru.\n");
        exit(-1);
    } else {
        c = getc(
                fr);                                   // potrebujem byť vždy na začiatku o jeden char pozadu nakoľko si potrebuje skontrolovať či to je hviezda alebo názov nového filmu, viacej riadok 64
        while (c != EOF) {
            temp_film = (FILM *) malloc(sizeof(FILM));
            for (i = 0; i < MAX; i++) {
                meno_filmu[i] = 0;
                reziser_filmu[i] = 0;
            }
            i = 0;
            while (c != '\n') {
                meno_filmu[i++] = c;
                c = getc(fr);
            }
            i = 0;
            fscanf(fr, "%d\n", &rok_filmu);
            while ((c = getc(fr)) != '\n') {
                reziser_filmu[i++] = c;
            }
            i = 0;
            strcpy(temp_film->nazov, meno_filmu);
            temp_film->rok = rok_filmu;
            strcpy(temp_film->reziser, reziser_filmu);
            temp_film->p_dalsi_film = NULL;
            while ((c = getc(fr)) ==
                   '*') {                                 //ak načítame hviezdu, vieme že načítavame hercov, ak nenačítavame hercov a není koniec súboru zase sa načítava meno filmu viacej riadok 42
                temp_herec = (HEREC *) malloc(sizeof(HEREC));               //alokovanie
                fscanf(fr, "%s", temp_herec->herca.krstne);
                fscanf(fr, "%s", temp_herec->herca.priezvisko);
                fscanf(fr, "%d\n", &temp_herec->vek);
                if (p_herec ==
                    NULL) {                                      //ak načítaveme prvého herca, uložíme jeho adresu do temp_film->herec
                    p_herec = temp_herec;
                    temp_film->herec = p_herec;
                } else {
                    p_herec->p_dalsi_herec = temp_herec;
                    p_herec = temp_herec;
                }
            }
            p_herec->p_dalsi_herec = NULL;                                  // vynulovanie, aby pri výpise vedel while cyklus kedy skončiť
            p_herec = NULL;
            if (kniznica_film == NULL) {                                    // ak načítavame prvý film
                kniznica_film = temp_film;
            } else {
                p_film = kniznica_film;
                while (p_film->p_dalsi_film != NULL) {                      // pripojíme struct na koniec linked listu
                    p_film = p_film->p_dalsi_film;
                }
                p_film->p_dalsi_film = temp_film;
            }
        }
        if (fclose(fr) == EOF) {
            printf("Chyba zatvarania suboru.");
            exit(-1);
        }
        return kniznica_film;
    }
}

void vypis(FILM *zaciatok) {
    if (zaciatok != NULL) {                                                 // kvoli stabilite programu sa spýtame či máme vôbec čo vypisovať
        FILM *p = zaciatok;
        HEREC *f = p->herec;
        do {
            printf("%s (%d) %s\n", p->nazov, p->rok, p->reziser);
            printf("\tHraju: ");
            do {
                printf("%s %s (%d)", f->herca.krstne, f->herca.priezvisko, f->vek);
                if (f->p_dalsi_herec != NULL) {
                    printf(", ");
                } else {
                    printf("\n");
                }
                f = f->p_dalsi_herec;
            } while (f != NULL);                                // vypisujeme dokial je čo vypisovať
            p = p->p_dalsi_film;
            if (p != NULL) {
                f = p->herec;                                   // nastavíme F na nového prvého herca nového filmu
            }
        } while (p != NULL);
    }
}

FILM *pridaj(FILM *zaciatok) {
    int kontrola = 0;
    FILM *p_film = zaciatok;
    FILM *temp_film = NULL;
    HEREC *temp_herec = NULL;
    HEREC *p_herec = NULL;

    char nazov[MAX], reziser[MAX];
    int rok = 0, i = 0;
    char c = ' ';
    for (int k = 0; k < MAX; k++) {
        nazov[k] = 0;
        reziser[k] = 0;
    }
    c = getchar();
    while ((c = getchar()) != '\n') {
        nazov[i++] = c;
    }
    i = 0;
    scanf("%d\n", &rok);

    while ((c = getchar()) != '\n') {
        reziser[i++] = c;
    }
    i = 0;

    if (zaciatok != NULL) {
        while (p_film->p_dalsi_film != NULL) {
            if((strcmp(p_film->nazov, nazov)) ==  0 && (strcmp(p_film->reziser, reziser)) == 0 && p_film->rok == rok){  // kontrola či sa taký film už nenachadza
//                printf("Uz sa nachadza");
                while ((c = getchar()) != '*') {                                                                        //aby to spapalo zvyšok vstupu
                    ungetc(c, stdin);
                    c = getchar(); // na enter
                }
                return zaciatok;
            }
            p_film = p_film->p_dalsi_film;
        }
        p_film = p_film->p_dalsi_film;
        p_film = (FILM *) malloc(sizeof(FILM));
    } else {
        p_film = (FILM *) malloc(sizeof(FILM));
        kontrola = 1;                                       // kvoli stabilite programu zistujeme či pridávame film bez funkcie nacitaj alebo s
    }

    strcpy(p_film->nazov, nazov);
    p_film->rok = rok;
    strcpy(p_film->reziser, reziser);
    while ((c = getchar()) != '*') {
        ungetc(c, stdin);
        temp_herec = (HEREC *) malloc(sizeof(HEREC));               //alokovanie
        scanf("%s", temp_herec->herca.krstne);
        scanf("%s", temp_herec->herca.priezvisko);
        scanf("%d", &temp_herec->vek);

        if (p_herec == NULL) {
            p_herec = temp_herec;
            p_film->herec = p_herec;
        } else {
            p_herec->p_dalsi_herec = temp_herec;
            p_herec = temp_herec;
        }

        c = getchar(); // na enter
    }
    p_herec->p_dalsi_herec = NULL;
    p_herec = NULL;

    if (kontrola == 0) {                                    // kontrola či užívateľ spravil prikaz nacitaj
        temp_film = zaciatok;
        while (temp_film->p_dalsi_film != NULL) {
            temp_film = temp_film->p_dalsi_film;
        }
        temp_film->p_dalsi_film = p_film;
        p_film->p_dalsi_film = NULL;
        return zaciatok;
    } else {                                                // ak nie returneme pointer ako začiatok zoznamu
        p_film->p_dalsi_film = NULL;
        return p_film;
    }
}

FILM *vymaz(FILM *zaciatok) {
    if (zaciatok != NULL) {                             // ak mame vôbec čo vymazať
        FILM *p_film = zaciatok, *pomocny_film = NULL;
        HEREC *p_herec = NULL, *temp_herec = NULL;
        char c, film[MAX] = "";
        int i = 0;
        c = getchar(); //enter po zadaní príkazu
        while ((c = getchar()) != '\n') {
            film[i++] = c;
        }
        while ((strcmp(p_film->nazov, film)) != 0) {        //nastavime sa na film kt chceme vymazat
            pomocny_film = p_film;
            p_film = p_film->p_dalsi_film;
            if (p_film == NULL) {                           // zistime či sa vôbec nachádza v zozname
                return zaciatok;
            }
        }
        if (p_film != NULL) {
            p_herec = p_film->herec;

            while (1) {                                     //ovlnime hercov
                temp_herec = p_herec->p_dalsi_herec;
                free(p_herec);
                p_herec = NULL;
                if (temp_herec == NULL) {
                    break;
                }
                p_herec = temp_herec;
            }
            p_film->herec = NULL;
            if (p_film->p_dalsi_film != NULL && pomocny_film !=
                                                NULL) {         //ak sa film nachádza medzi 2 filmami, prepojime pointre a uvolnime pamäť
                pomocny_film->p_dalsi_film = p_film->p_dalsi_film;
                free(p_film);
                p_film = NULL;
            } else if (pomocny_film ==
                       NULL) {           //Ak vymazavam uplne prvý film tak returnem nový začiatok linked listu
                zaciatok = p_film->p_dalsi_film;
                free(p_film);
                p_film = NULL;
                return zaciatok;
            } else {                                    //Ak vymazavam film na konci linked listu
                pomocny_film->p_dalsi_film = NULL;
                free(p_film);
                p_film = NULL;
            }
            return zaciatok;
        }
    } else {
        return zaciatok;
    }
}

void filmy(FILM *zaciatok) {
    if (zaciatok != NULL) {
        char krstne[MAX], priezvisko[MAX];
        FILM *p = zaciatok;
        HEREC *f = p->herec;
        scanf("%s %s", krstne, priezvisko);
        do {
            do {
                if (strcmp(f->herca.krstne, krstne) == 0 && strcmp(f->herca.priezvisko, priezvisko) ==
                                                            0) { //Ak najdeme film v ktorom hra, vypiseme nazov a rok filmu
                    printf("%s (%d)\n", p->nazov, p->rok);
                }
                f = f->p_dalsi_herec;
            } while (f != NULL);
            p = p->p_dalsi_film;
            if (p != NULL) {
                f = p->herec;
            }
        } while (p != NULL);
    }
}

void herci(FILM *zaciatok) {
    if (zaciatok != NULL) {
        FILM *p_film_p = zaciatok;
        FILM *p_film_d = zaciatok;

        HEREC *temp_herec = NULL;

        char prvy_film[MAX] = "", druhy_film[MAX] = "";
        char c;
        int i = 0;
        c = getchar(); // na enter po zadani prikazu
        while ((c = getchar()) != '\n') {
            prvy_film[i++] = c;
        }
        i = 0;
        while ((c = getchar()) != '\n') {
            druhy_film[i++] = c;
        }
        while (strcmp(p_film_p->nazov, prvy_film) != 0) {
            p_film_p = p_film_p->p_dalsi_film;
        }
        while (strcmp(p_film_d->nazov, druhy_film) != 0) {       //Nastavime si dva pointre na dane filmy
            p_film_d = p_film_d->p_dalsi_film;
        }
        int kontrola = 0;

        HEREC *p_herec_p = p_film_p->herec;
        HEREC *p_herec_d = p_film_d->herec;
        temp_herec = p_herec_d;
        while (p_herec_p->p_dalsi_herec != NULL) {
            while (p_herec_d->p_dalsi_herec != NULL) {          //skontrolujem každeho herca s každým hercom
                if (strcmp(p_herec_p->herca.krstne, p_herec_d->herca.krstne) == 0 &&
                    strcmp(p_herec_p->herca.priezvisko, p_herec_d->herca.priezvisko) ==
                    0) {        //Vypisem ak najdem zhodu
                        kontrola++;
//                    printf("%s %s (%d)\n", p_herec_p->herca.krstne, p_herec_p->herca.priezvisko, p_herec_p->vek);
                }
                p_herec_d = p_herec_d->p_dalsi_herec;
            }
            p_herec_d = temp_herec;
            p_herec_p = p_herec_p->p_dalsi_herec;
        }

                                                                //2 identicke funkcie nakoľko som nevedel ako inak spraviť výpis s čiarkou tak aby neostala na konci
        p_herec_p = p_film_p->herec;
        p_herec_d = p_film_d->herec;
        temp_herec = p_herec_d;
        while (p_herec_p->p_dalsi_herec != NULL) {
            while (p_herec_d->p_dalsi_herec != NULL) {          //skontrolujem každeho herca s každým hercom
                if (strcmp(p_herec_p->herca.krstne, p_herec_d->herca.krstne) == 0 &&
                    strcmp(p_herec_p->herca.priezvisko, p_herec_d->herca.priezvisko) ==
                    0) {        //Vypisem ak najdem zhodu
                    --kontrola;
                    printf("%s %s (%d)", p_herec_p->herca.krstne, p_herec_p->herca.priezvisko, p_herec_p->vek);
                    if(kontrola>0){
                        printf(", ");
                    }
                }
                p_herec_d = p_herec_d->p_dalsi_herec;
            }
            p_herec_d = temp_herec;
            p_herec_p = p_herec_p->p_dalsi_herec;
        }
    }
    printf("\n");
}

void rok(FILM *zaciatok) {
    if (zaciatok != NULL) {
        FILM *p_film = zaciatok;
        HEREC *p_herec_origo = NULL;
        HEREC *p_herec_zoznam = NULL;
        HEREC *temp_zoznam = NULL;
        HEREC *zaciatok_zoznamu = NULL;
        int rok = 0;
        scanf("%d", &rok);

        while (p_film != NULL) {
            if (p_film->rok == rok) {
                p_herec_origo = p_film->herec;
                while (p_herec_origo != NULL) {
                    if ((skontroluj(zaciatok_zoznamu, p_herec_origo)) == 1) {                       // skontroluje ci sa uz v zozname nenachadza herec aby sme predisli dvom kopiam
                        p_herec_zoznam = (HEREC *) malloc(sizeof(HEREC));                           // zapisem si hercov do noveho linked listu
                        strcpy(p_herec_zoznam->herca.krstne, p_herec_origo->herca.krstne);
                        strcpy(p_herec_zoznam->herca.priezvisko, p_herec_origo->herca.priezvisko);
                        p_herec_zoznam->vek = p_herec_origo->vek;

                        if (temp_zoznam == NULL) {
                            temp_zoznam = p_herec_zoznam;
                            zaciatok_zoznamu = temp_zoznam;
                            zaciatok_zoznamu->p_dalsi_herec = NULL;
                        } else {
                            temp_zoznam->p_dalsi_herec = p_herec_zoznam;
                            temp_zoznam = p_herec_zoznam;
                            p_herec_zoznam->p_dalsi_herec = NULL;
                        }
                    }
                    p_herec_origo = p_herec_origo->p_dalsi_herec;
                }
            }
            p_film = p_film->p_dalsi_film;
        }

        HEREC *pomocna_premenna = NULL;
        int temp_vek = 0;
        char temp_priezvisko[MAX] = "";
        char temp_krstne[MAX] = "";
        p_herec_zoznam = zaciatok_zoznamu;
        temp_zoznam = p_herec_zoznam->p_dalsi_herec;
        while (p_herec_zoznam != NULL) {
            while (temp_zoznam != NULL) {
                if (strcmp(p_herec_zoznam->herca.priezvisko, temp_zoznam->herca.priezvisko) >
                    0) {  //zoradenie podla priezviska
//                printf("%s > %s\n", p_herec_zoznam->herca.priezvisko, temp_zoznam->herca.priezvisko);

                    strcpy(temp_priezvisko, p_herec_zoznam->herca.priezvisko);
                    strcpy(temp_krstne, p_herec_zoznam->herca.krstne);
                    temp_vek = p_herec_zoznam->vek;

                    strcpy(p_herec_zoznam->herca.priezvisko, temp_zoznam->herca.priezvisko);
                    strcpy(p_herec_zoznam->herca.krstne, temp_zoznam->herca.krstne);
                    p_herec_zoznam->vek = temp_zoznam->vek;

                    strcpy(temp_zoznam->herca.priezvisko, temp_priezvisko);
                    strcpy(temp_zoznam->herca.krstne, temp_krstne);
                    temp_zoznam->vek = temp_vek;
                }
                if (strcmp(p_herec_zoznam->herca.priezvisko, temp_zoznam->herca.priezvisko) == 0 &&
                    (strcmp(p_herec_zoznam->herca.krstne, temp_zoznam->herca.krstne) >
                     0)) {            //zoradenie podla krstneho mena ak su priezviska rovnake

//                printf(" KRSTNE: %s > %s\n", p_herec_zoznam->herca.krstne, temp_zoznam->herca.krstne);

                    strcpy(temp_priezvisko, p_herec_zoznam->herca.priezvisko);
                    strcpy(temp_krstne, p_herec_zoznam->herca.krstne);
                    temp_vek = p_herec_zoznam->vek;

                    strcpy(p_herec_zoznam->herca.priezvisko, temp_zoznam->herca.priezvisko);
                    strcpy(p_herec_zoznam->herca.krstne, temp_zoznam->herca.krstne);
                    p_herec_zoznam->vek = temp_zoznam->vek;

                    strcpy(temp_zoznam->herca.priezvisko, temp_priezvisko);
                    strcpy(temp_zoznam->herca.krstne, temp_krstne);
                    temp_zoznam->vek = temp_vek;
                }
                if (strcmp(p_herec_zoznam->herca.priezvisko, temp_zoznam->herca.priezvisko) == 0 &&
                    (strcmp(p_herec_zoznam->herca.krstne, temp_zoznam->herca.krstne) == 0) &&
                    //zoradenie podla roku ak je meno rovnake
                    p_herec_zoznam->vek > temp_zoznam->vek) {

//                printf(" VEK: %d > %d\n", p_herec_zoznam->vek, temp_zoznam->vek);

                    strcpy(temp_priezvisko, p_herec_zoznam->herca.priezvisko);
                    strcpy(temp_krstne, p_herec_zoznam->herca.krstne);
                    temp_vek = p_herec_zoznam->vek;

                    strcpy(p_herec_zoznam->herca.priezvisko, temp_zoznam->herca.priezvisko);
                    strcpy(p_herec_zoznam->herca.krstne, temp_zoznam->herca.krstne);
                    p_herec_zoznam->vek = temp_zoznam->vek;

                    strcpy(temp_zoznam->herca.priezvisko, temp_priezvisko);
                    strcpy(temp_zoznam->herca.krstne, temp_krstne);
                    temp_zoznam->vek = temp_vek;
                }
                temp_zoznam = temp_zoznam->p_dalsi_herec;
            }
            p_herec_zoznam = p_herec_zoznam->p_dalsi_herec;
            temp_zoznam = p_herec_zoznam->p_dalsi_herec;

            if (temp_zoznam == NULL) {
                break;
            }
        }
//    printf("Zmenilo sa nieco?\n");
        p_herec_zoznam = zaciatok_zoznamu;
        while (p_herec_zoznam != NULL) {
            pomocna_premenna = p_herec_zoznam->p_dalsi_herec;
            printf("%s %s (%d)", p_herec_zoznam->herca.krstne, p_herec_zoznam->herca.priezvisko, p_herec_zoznam->vek);
            if (p_herec_zoznam->p_dalsi_herec != NULL) {
                printf(", ");
            }
            free(p_herec_zoznam);
            p_herec_zoznam = NULL;
            p_herec_zoznam = pomocna_premenna;
        }
        printf("\n");
        zaciatok_zoznamu = NULL;
    }
}

FILM *uvolni(FILM *zaciatok) {
    if (zaciatok != NULL) {
        FILM *p_film = zaciatok, *pomocny_film = NULL;
        HEREC *p_herec = NULL, *temp_herec = NULL;
        p_herec = p_film->herec;
        while (1) {
            pomocny_film = p_film->p_dalsi_film;
            while (1) {
                temp_herec = p_herec->p_dalsi_herec;
                free(p_herec);
                p_herec = NULL;
                if (temp_herec == NULL) {
                    break;
                }
                p_herec = temp_herec;
            }
            free(p_film);
            p_film = NULL;
            if (pomocny_film == NULL) {
                break;
            }
            p_film = pomocny_film;
            p_herec = p_film->herec;
        }
        zaciatok = NULL;
        return zaciatok;
    } else {
        return zaciatok;
    }
}

int skontroluj(HEREC *zaciatok_zoznamu, HEREC *p_herec_origo) {
    HEREC *z_z = zaciatok_zoznamu;
    HEREC *p_h = p_herec_origo;
    if (z_z != NULL) {
        while (z_z != NULL) {
            if ((strcmp(z_z->herca.priezvisko, p_h->herca.priezvisko)) == 0 &&
                (strcmp(z_z->herca.krstne, p_h->herca.krstne)) == 0 &&
                z_z->vek == p_h->vek) {
//                printf("Nasiel som dvojnika");
                return 0;                                                       // Ak zistí že už sa v zozname daný herec nachádza vráti 0 čo znamená že sa zápis do linked listu nevykoná
            }
            z_z = z_z->p_dalsi_herec;
        }
        return 1;                   // ak nenajde zhodu vráti 1 = môže zapísať
    } else {
        return 1;                   // ak linked list ešte nebol vytvorený vráti 1 = môže zapísať
    }
}

int main() {
    int kontrola = 0;
    FILM *film = NULL;
    char argument[MAX];
    char argument_nacitaj[MAX] = "nacitaj";
    char argument_vypis[MAX] = "vypis";
    char argument_pridaj[MAX] = "pridaj";
    char argument_vymaz[MAX] = "vymaz";
    char argument_filmy[MAX] = "filmy";
    char argument_herci[MAX] = "herci";
    char argument_rok[MAX] = "rok";
    char argument_koniec[MAX] = "koniec";


    while (kontrola == 0) {
        scanf("%s", argument);
        if (strcmp(argument, argument_nacitaj) == 0) {
            film = uvolni(film);
            film = nacitaj();
        } else if (strcmp(argument, argument_vypis) == 0) {
            vypis(film);
        } else if (strcmp(argument, argument_pridaj) == 0) {
            film = pridaj(film);
        } else if (strcmp(argument, argument_vymaz) == 0) {
            film = vymaz(film);
        } else if (strcmp(argument, argument_filmy) == 0) {
            filmy(film);
        } else if (strcmp(argument, argument_herci) == 0) {
            herci(film);
        } else if (strcmp(argument, argument_rok) == 0) {
            rok(film);
        } else if (strcmp(argument, argument_koniec) == 0) {
            film = uvolni(film);
            kontrola = 1;
//            printf("koniec\n");
        } else {
            printf("Zly vstup.\n");
        }
    }
    return 0;
}