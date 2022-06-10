//Marina Kravcuk (login xkravc02)
//2. projekt IZP
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
//structura pro bunku
typedef struct bunka
{
    int b_len;
    char* bunka;
    int cap;
}bunka_t;

//structura pro radek
typedef struct radek
{
    int bunka_n;
    bunka_t* bunky;
}radek_t;

//structura pro tabulku
typedef struct 
{
    int radek_n;
    radek_t* radky;
}tab_t;

//funkce pamet allocuje pamet pro privni bunku v tabulce
//funguje spolu s funkci tab_ctor, ktera tu bunku inicializuje
void pamet(tab_t* tab)
{
    tab->radky = malloc(sizeof(radek_t));
    tab->radky[0].bunky = malloc(sizeof(bunka_t));
    tab->radky[0].bunky[0].bunka = malloc(10 * sizeof(char));
}
void tab_ctor(tab_t* tab)
{
    tab->radek_n = 0;
    pamet(tab);
    tab->radky[0].bunka_n = 0;
    tab->radky[0].bunky[0].b_len = 0;
    tab->radky[0].bunky[0].cap = 10;
}

//funkce resize bunky dodava pamet pro bunku, kdyz obsahuje vic nez 10 znaku
//v pripade, ze reallokace se nepovede, vypise "error"
void resize_bunky(tab_t* tab)
{
    tab->radky[tab->radek_n].bunky[tab->radky[tab->radek_n].bunka_n].cap += 10;
    void *p;
    p = realloc(tab->radky[tab->radek_n].bunky[tab->radky[tab->radek_n].bunka_n].bunka, tab->radky[tab->radek_n].bunky[tab->radky[tab->radek_n].bunka_n].cap * sizeof(char));
    if (p)
        tab->radky[tab->radek_n].bunky[tab->radky[tab->radek_n].bunka_n].bunka = p;
    else
    {
        fprintf(stderr,"error");
    }    
}

//funkce b_ctor inicializuje novou bunku
void b_ctor(tab_t* tab)
{
    tab->radky[tab->radek_n].bunky[tab->radky[tab->radek_n].bunka_n].bunka = malloc(10 * sizeof(char));
    tab->radky[tab->radek_n].bunky[tab->radky[tab->radek_n].bunka_n].cap = 10;
    tab->radky[tab->radek_n].bunky[tab->radky[tab->radek_n].bunka_n].b_len = 0;
}

//funkce r_ctor inicializuje novy radek a prvni bunku v ni
void r_ctor(tab_t* tab)
{
    tab->radky[tab->radek_n].bunky = malloc(sizeof(bunka_t));
    tab->radky[tab->radek_n].bunka_n = 0;
    b_ctor(tab);
}

//funkce rezise radka dodava pamet pro novou bunku v radku
//kdyz se reallokace nepovede, vypise "error"
void resize_radka(tab_t* tab)
{
    tab->radky[tab->radek_n].bunka_n++;
    void *p;
    p = realloc(tab->radky[tab->radek_n].bunky, (tab->radky[tab->radek_n].bunka_n + 1) * sizeof(bunka_t));
    if (p)
        tab->radky[tab->radek_n].bunky = p;
    else
    {
        fprintf(stderr, "error\n");
    }
    
}

//funkce resize_t dodava pamet pro novy radek v tabulce
//kdyz se reallokace nepovede, vypise "error"
void resize_t(tab_t* tab)
{
    tab->radek_n++;
    void* p;
    p = realloc(tab->radky, (tab->radek_n + 1) * sizeof(radek_t));
    if (p)
        tab->radky = p;
    else
    {
        fprintf(stderr, "error\n");
    }
    
}


//nasledujici 3 funkci se pouzivaji pro vyrovnani vstupni tabulky
//aby vsechny radky meli stejny pocet bunek
void special_resize_radka(tab_t* tab, int i)
{
    tab->radky[i].bunka_n++;
    void *p;
    p = realloc(tab->radky[i].bunky, (tab->radky[i].bunka_n + 1) * sizeof(bunka_t));
    if (p)
        tab->radky[i].bunky = p;
    else
    {
        fprintf(stderr, "error\n");
    }
    
}
void special_b_ctor(tab_t* tab, int i)
{
    tab->radky[i].bunky[tab->radky[i].bunka_n].bunka = malloc(20 * sizeof(char));
    strcpy(tab->radky[i].bunky[tab->radky[i].bunka_n].bunka, "");
    tab->radky[i].bunky[tab->radky[i].bunka_n].cap = 20;
    tab->radky[i].bunky[tab->radky[i].bunka_n].b_len = 0;
}
void special_resize_tabulky(tab_t* tabulka, int pocet_radku, int pocet_sloupcu)
{
    for (int i = 0; i <= pocet_radku; i++)
    {
        for (int j = 0; j <= pocet_sloupcu; j++)
        {
            if (tabulka->radky[i].bunka_n < pocet_sloupcu)
            {
                special_resize_radka(tabulka, i);
                special_b_ctor(tabulka, i);
            }
        }
    }
}


//funkce detected_delim upravuje vstupni retezec delimu tak, 
//aby neobsahoval uvozovky, stejne znaky nebo zpetne lomitko
//vrati spravny retezec delimu
char* detected_delim(char* ret_delimu)
{
    char delimy[20] = "";
    unsigned j = 0;
    for (unsigned i = 0; i < strlen(ret_delimu); i++)
    {
        if((ret_delimu[i]!= '"')&&(!strchr(delimy, ret_delimu[i]))&&(ret_delimu[i]!= '\\')){
            delimy[j] = ret_delimu[i];
            j++;
        }
    }
    strcpy(ret_delimu, delimy);
    return ret_delimu;
}

//struktura pro aktualne vybrane bunky
typedef struct 
{
    int* okno;
    int cap;
}vybrane_bunky;

//inicializace struktury vybrannych bunek
void v_ctor(vybrane_bunky* v)
{
    v->cap = 0;
    v->okno = malloc(5 * sizeof(int));
    if (v->okno == NULL)
    {
        fprintf(stderr, "Nepovedlo se allokovat pamet");
    }
    
}
void v_init(vybrane_bunky* v)
{
    v->cap = 2;
    for (int i = 0; i < 5; i++)
    {
        v->okno[i] = 1;
    }
}

void v_dtor(vybrane_bunky* v)
{
    v->cap = 0;
    free(v->okno);
}

void vyber_bunek(char* bunky, vybrane_bunky* r_c, int sloupci, int radky);
void swap(tab_t* tab, char* bunka, vybrane_bunky* r_c);
void set(tab_t* tab, char* str, vybrane_bunky* r_c);
void len(tab_t* tab, char* bunka, vybrane_bunky* r_c);
void minmax(tab_t* tab, char* bunka, vybrane_bunky* r_c);
void find(tab_t* tab, char* prikaz, vybrane_bunky* r_c);
void clear(tab_t* tab, vybrane_bunky* r_c);
void special_resize_tabulky(tab_t* tabulka, int pocet_radku, int pocet_sloupcu);


//funkce pro prikazy irow a arow, vkladaji prazdne radky
//reallocuje se novy radek, kopiruje se obsah bunek a ocisti portebny radek
void irowarow(tab_t* tab, vybrane_bunky* r_c, int sl, char* prikaz)
{
    resize_t(tab);
    for (int i = 0; i <= sl; i++)
    {
        special_resize_radka(tab, tab->radek_n - 1);
        tab->radky[tab->radek_n-1].bunky[i].bunka = malloc(20 * sizeof(char));
        if (tab->radky[tab->radek_n-1].bunky[i].bunka == NULL)
            fprintf(stderr, "Nepovedlo se allokovat pamet");
        strcpy(tab->radky[tab->radek_n-1].bunky[i].bunka, "");
        tab->radky[tab->radek_n-1].bunky[i].cap = 20;
        tab->radky[tab->radek_n-1].bunky[i].b_len = 0;
    }
    if (strcmp(prikaz, "irow") == 0)
    {
        for (int i = tab->radek_n-1; i >= r_c->okno[0]; i--)
        {
            for (int j = sl; j >= 0; j--)
            {
                strcpy(tab->radky[i].bunky[j].bunka, tab->radky[i-1].bunky[j].bunka);
            }
            
        }
        for (int i = 0; i <= sl; i++)
        {
            memset(tab->radky[r_c->okno[0]-1].bunky[i].bunka, '\0', 1);
        }
    }
    if (strcmp(prikaz, "arow") == 0)
    {
        for (int i = tab->radek_n-1; i > r_c->okno[0]; i--)
        {
            for (int j = sl; j >= 0; j--)
            {
                strcpy(tab->radky[i].bunky[j].bunka, tab->radky[i-1].bunky[j].bunka);
            }
            
        }
        for (int i = 0; i <= sl; i++)
        {
            memset(tab->radky[r_c->okno[0]].bunky[i].bunka, '\0', 1);
        }
    }
    
}

//funkce drow odstrani vybranne radky
//kopiruji se obsahy bunek z dolu nahoru do potrebneho radku
//a uvolni se posledni n radku
void drow(tab_t* tab, vybrane_bunky* r_c, int sl)
{
    if (r_c->cap == 2)
    {
        for (int i = r_c->okno[0]; i <= tab->radek_n-1; i++)
        {
            for (int j = 0; j <= sl; j++)
            {
                strcpy(tab->radky[i-1].bunky[j].bunka, tab->radky[i].bunky[j].bunka);            
            }
        }
        for (int i = 0; i <= sl; i++)
        {
            free(tab->radky[tab->radek_n-1].bunky[i].bunka);
        }
        free(tab->radky[tab->radek_n].bunky);

    }
    else
    {
        int k = r_c->okno[2] - r_c->okno[0] + 1;

        for (int i = r_c->okno[0]-1; i < tab->radek_n - k; i++)
        {
            for (int j = 0; j <= sl; j++)
            {
                strcpy(tab->radky[i].bunky[j].bunka, tab->radky[i+k].bunky[j].bunka);    
            }
        }
        for (int i = tab->radek_n - k; i > tab->radek_n; i++)
        {
            for (int j = 0; j <= sl; j++)
            {
                free(tab->radky[i].bunky[j].bunka);
            }
            free(tab->radky[i].bunky);
        }
        
    }
}

//funkce icolacol pro prikazy icol nebo acol
//vklada prazdny sloupec do tabulky napravo/nalevo od vybranneho sloupce
//pred volani funkci allokuje se novy (posledni) sloupec
//kopiruje se obsah bunek zprava doleva
//ocisti potrebny sloupec 
void icolacol(tab_t* tab, vybrane_bunky* r_c, int radky, char* prikaz)
{
    //printf("%d\n", r_c->okno[1]);
    for (int i = 0; i <= radky; i++)
    {
        for (int j = tab->radky[1].bunka_n; j > r_c->okno[1] - 1; j--)
        {
            strcpy(tab->radky[i].bunky[j].bunka, tab->radky[i].bunky[j-1].bunka);
        }
        
    }
    if (strcmp(prikaz, "icol")==0)
    {
        for (int i = 0; i <= radky; i++)
        {
            memset(tab->radky[i].bunky[r_c->okno[1]-1].bunka, '\0', 1);
        }
    }
    if (strcmp(prikaz, "acol")==0)
    {
        for (int i = 0; i <= radky; i++)
        {
            memset(tab->radky[i].bunky[r_c->okno[1]].bunka, '\0', 1);
        }
    }
}

// funkce pro prikaz dcol
// odstrani vybranny sloupec
// kopiruje obsah bunek a ocisti se pamet posledniho sloupce
void dcol(tab_t* tab, vybrane_bunky* r_c, int radky)
{
    if (r_c->cap == 2)
    {
        for (int i = 0; i <= radky; i++)
        {
            for (int j = r_c->okno[1]; j <= tab->radky[1].bunka_n; j++)
            {
                strcpy(tab->radky[i].bunky[j-1].bunka, tab->radky[i].bunky[j].bunka);
                if (j == tab->radky[i].bunka_n)
                {
                    free(tab->radky[i].bunky[j].bunka);
                }
                
            }
        }
    }
    else
    {
        
    }
    

}

//struktura pro docasne promneny
typedef struct
{
    int x;
    char** obsah;
}def;

//inicializace struktury pro docasne promnenny
void def_init(def* promnenna)
{
    promnenna->x = 9;
    promnenna->obsah = malloc(10*sizeof(char*));
}

void def_dtor(def* promnenna)
{
    // for (int i = 0; i <= promnenna->x; i++)
    // {
    //     if (promnenna->obsah[i] != NULL)
    //     {
    //         free(promnenna->obsah[i]);
    //     }
    // }
    free(promnenna->obsah);
    promnenna->x = 0;
    
}

//nastavi hodnotu vybrane bunky do docasne promnenny X
void defX(tab_t* tab, vybrane_bunky* r_c, char* prikaz, def* promnenna)
{
    char cislo[1] = "";
    cislo[0] = prikaz[strlen(prikaz)-1];
    int x = atoi(cislo);
    promnenna->obsah[x] = malloc(100 * sizeof(char));
    strcpy(promnenna->obsah[x], tab->radky[r_c->okno[0]-1].bunky[r_c->okno[1]-1].bunka);
}

//vybrana bunka se nastavi na hodnotu z docasne promnenny X
void useX(tab_t* tab, vybrane_bunky* r_c, char* prikaz, def* promnenna)
{
    char cislo[1] = "";
    cislo[0] = prikaz[strlen(prikaz)-1];
    int x = atoi(cislo);
    
    if (r_c->cap == 2)
    {
        strcpy(tab->radky[r_c->okno[0]-1].bunky[r_c->okno[1]-1].bunka, promnenna->obsah[x]);
    }
    else
    {
        for (int i = r_c->okno[0]-1; i < r_c->okno[2]; i++)
        {
            for (int j = r_c->okno[1]-1; j < r_c->okno[3]; j++)
            {
                strcpy(tab->radky[i].bunky[j].bunka, promnenna->obsah[x]);
            }
            
        }
        
    }
    
    
}

//numericka hodnota docasne promnenny X bude zvetsina o 1
//kdyz promnenna X nebude obsahovat cislo, hodnota se nastavi na 1
void incX(char* prikaz, def* promnenna)
{
    char cislo[1] = "";
    cislo[0] = prikaz[strlen(prikaz)-1];
    int x = atoi(cislo);
    bool cisloli = true;
    int tecka = 0;
    for (unsigned i = 0; i < strlen(promnenna->obsah[x]); i++)
    {
        if ((promnenna->obsah[x][i] < 48)||(promnenna->obsah[x][i] > 57))
        {
            if ((i == 0)&&((promnenna->obsah[x][i] == '+')||(promnenna->obsah[x][i] == '-')))
            {
                cisloli = true;
            }
            else if ((promnenna->obsah[x][i] == '.')&&(tecka<1))
            {
                tecka++;
            }
            else
            {
                cisloli = false;
                break;
            }
            
        }
        
    }
    int intx;
    char* end;
    if (cisloli == true)
    {
        intx = strtol(promnenna->obsah[x], &end, 10);
        intx++;
        memset(promnenna->obsah[x], '\0', 1);
        sprintf(promnenna->obsah[x], "%d%s", intx, end);
    }
    else
    {
        strcpy(promnenna->obsah[x], "1");
    }
    
    //printf("%s\n", promnenna->obsah[x]);
    
}

// soucet/aritmeticky prumner vybrannych bunek se ulozi do bunky R C
// ignoruje bunky, ktere neobsahuji cisla
// nejdriv se zjisti R C
// pak se provede zkouska jestli to je nebo neni cislo
// kdyz bunka je cislo, slozi se do promnenne sum
void sum(tab_t* tab, char* bunka, vybrane_bunky* r_c)
{
    int cell;
    int row;
    char cislo[5] = "";
    int i = 0, j = 0;
    while (bunka[i]!='\0')
    {
        if ((bunka[i]>=48)&&(bunka[i]<=57))
        {
            cislo[j] = bunka[i];
            j++;
        }
        else if (bunka[i] == ',')
        {
            row = atoi(cislo);
            j = 0;
            memset(cislo, ' ', 5);
        }
        i++;
        
    }
    cell = atoi(cislo);
    int tecka = 0;
    bool cisloli = true;
    double sum = 0;
    int count = 0;
    if (r_c->cap != 2)
    {
        for (int i = r_c->okno[0]-1; i < r_c->okno[2]; i++)
        {
            for (int j = r_c->okno[1]-1; j < r_c->okno[3]; j++)
            {
                tecka = 0;
                cisloli = true;
                for (unsigned k = 0; k < strlen(tab->radky[i].bunky[j].bunka); k++)
                {
                    if ((tab->radky[i].bunky[j].bunka[k]<48)||(tab->radky[i].bunky[j].bunka[k]>57))
                    {
                        if ((k==0)&&((tab->radky[i].bunky[j].bunka[k]=='+')||(tab->radky[i].bunky[j].bunka[k]=='-')))
                        {
                            cisloli = true;
                        }
                        else if ((tab->radky[i].bunky[j].bunka[k]=='.')&&(tecka<1))
                        {
                            tecka++;
                        }
                        else
                        {
                            cisloli = false;
                            break;
                        }
                        
                    }
                    
                }

                if (cisloli == true){
                    sum = sum + atof(tab->radky[i].bunky[j].bunka);
                    count++;
                }
            }
            
        }
        
    }
    if (strstr(bunka, "sum"))
    {
        sprintf(tab->radky[row-1].bunky[cell-1].bunka, "%g", sum);
    }
    else
    {
        sprintf(tab->radky[row-1].bunky[cell-1].bunka, "%g", sum/count);
    }
    
}

//pocet neprazdnych bunek se ulozi do bunky R C
void count(tab_t* tab, char* bunka, vybrane_bunky* r_c)
{
    int cell;
    int row;
    char cislo[5] = "";
    int i = 0, j = 0;
    while (bunka[i]!='\0')
    {
        if ((bunka[i]>=48)&&(bunka[i]<=57))
        {
            cislo[j] = bunka[i];
            j++;
        }
        else if (bunka[i] == ',')
        {
            row = atoi(cislo);
            j = 0;
            memset(cislo, '\0', 1);
        }
        i++;
        
    }
    cell = atoi(cislo);
    int count = 0;
    if (r_c->cap != 2)
    {
        for (int i = r_c->okno[0]-1; i < r_c->okno[2]; i++)
        {
            for (int j = r_c->okno[1]-1; j < r_c->okno[3]; j++)
            {
                if (strlen(tab->radky[i].bunky[j].bunka)!=0)
                {
                    count++;
                }
                
            }
            
        }
    }
    sprintf(tab->radky[row-1].bunky[cell-1].bunka, "%d", count);
}

//funkce pro nacteni prikazu
//kazdy prikaz vola svou funkci
//param sloupci - maximalni sloupec vstupni tabulky
//param radky - maximalni radek vstupni tabulky
void nacteni_prikazu(char* prikazy, tab_t* tab, int *sloupci, int *radky)
{
    vybrane_bunky r_c;
    def promnenna;
    def_init(&promnenna);
    v_ctor(&r_c);
    v_init(&r_c);
    char* prikaz = strtok(prikazy, ";");
    while(prikaz != NULL){
        
        if (strlen(prikaz)>1000)
        {
            fprintf(stderr, "Maximalni delka prikazu je 1000 znaku");
            return;
        }
        
        if ((strchr(prikaz,'['))&&(strchr(prikaz,']'))&&(!strchr(prikaz, ' '))&&(!strchr(prikaz, 'm'))&&(!strchr(prikaz, 'f'))){
            v_init(&r_c);
            r_c.cap = 0;
            vyber_bunek(prikaz, &r_c, *sloupci, *radky);
        
        }
        else if ((strcmp(prikaz, "[min]")== 0)||(strcmp(prikaz, "[max]")== 0))
        {
            minmax(tab, prikaz, &r_c);
        }
        else if (strstr(prikaz, "swap "))
        {
            swap(tab, prikaz, &r_c);
        }
        else if ((strstr(prikaz, "set "))&&(!strchr(prikaz,'[')))
        {
            set(tab, prikaz, &r_c);
        }
        else if (strstr(prikaz, "len "))
        {
            len(tab, prikaz, &r_c);
        }
        else if (strstr(prikaz, "find "))
        {
            find(tab, prikaz, &r_c);
        }
        else if (strcmp(prikaz, "clear")==0)
        {
            clear(tab, &r_c);
        }
        else if ((strcmp(prikaz, "irow")==0)||(strcmp(prikaz, "arow")==0))
        {
            irowarow(tab, &r_c, *sloupci, prikaz);
            int r = *radky;
            *radky = r +1;
        }
        else if ((strcmp(prikaz, "icol")==0)||(strcmp(prikaz, "acol")==0))
        {
            int s = *sloupci;
            *sloupci = s +1;
            special_resize_tabulky(tab, *radky, *sloupci);
            icolacol(tab, &r_c, *radky, prikaz);
        }
        else if (strcmp(prikaz, "drow")==0)
        {
            drow(tab, &r_c, *sloupci);
            int r = *radky;
            if (r_c.cap==2)
            {
                *radky = r - 1;
            }
            else
            {
                *radky = r - (r_c.okno[2]-r_c.okno[0]+1);
            }
            
        }
        else if (strcmp(prikaz, "dcol")==0)
        {
            dcol(tab, &r_c, *radky);
            int s = *sloupci;
            *sloupci = s -1;
        }
        else if (strstr(prikaz, "def "))
        {
            defX(tab, &r_c, prikaz, &promnenna);
        }
        else if (strstr(prikaz, "use "))
        {
            useX(tab, &r_c, prikaz, &promnenna);
        }
        else if (strstr(prikaz, "inc "))
        {
            incX(prikaz, &promnenna);
        }
        else if ((strstr(prikaz, "sum "))||(strstr(prikaz, "avg ")))
        {
            sum(tab, prikaz, &r_c);
        }
        else if (strstr(prikaz, "count "))
        {
            count(tab, prikaz, &r_c);
        }
        else
        {
            fprintf(stderr, "Prikaz %s nenalezen", prikaz);
        }

        prikaz = strtok(NULL, ";");

    }
    def_dtor(&promnenna);
    v_dtor(&r_c);

}


//funkce clear ocisti obsah aktualne vybrannych bunek
void clear(tab_t* tab, vybrane_bunky* r_c)
{
    if (r_c->cap == 2)
    {
        memset(tab->radky[r_c->okno[0]-1].bunky[r_c->okno[1]-1].bunka,'\0', 1) ;
    }
    else
    {
        for (int i = r_c->okno[0]-1; i <= r_c->okno[2]-1; i++)
        {
            for (int j = r_c->okno[1]-1; j <= r_c->okno[3]-1; j++)
            {
                memset(tab->radky[i].bunky[j].bunka,'\0', 1) ;
            }
            
        }
        
    }
    
}

//funkce pro prikazy min a max
//z aktualne vybrannych bunek vybere jednu s minimalnim/maximalnim cislem
void minmax(tab_t* tab, char* bunka, vybrane_bunky* r_c)
{
    if (r_c->cap!=2)
    {
        int k = 0;
        int tmp1 = 0;
        int tmp2 = 0;
        long int tmp3 = 9999999999999999;
        int rowmax, cellmax, rowmin, cellmin;
        bool zkouska = true;
        for (int i = r_c->okno[0]-1; i <= r_c->okno[2]-1; i++)
        {
            for (int j = r_c->okno[1]-1; j <= r_c->okno[3]-1 ; j++)
            {
                k = 0;
                while (tab->radky[i].bunky[j].bunka[k]!='\0')
                {
                    if ((tab->radky[i].bunky[j].bunka[k]<48)||(tab->radky[i].bunky[j].bunka[k]>57))
                    {
                        zkouska = false;
                        break;

                    }
                    k++;
        
                }
                if (zkouska == true)
                {
                    tmp1 = atoi(tab->radky[i].bunky[j].bunka);
                    if (tmp1>tmp2)
                    {
                        tmp2 = tmp1;
                        rowmax = i;
                        cellmax = j;
                    }
                    if (tmp1<tmp3)
                    {
                        tmp3 = tmp1;
                        rowmin = i;
                        cellmin = j;
                    }
                        
                        
                }
                zkouska = true;    
            }
             
        }
        v_init(r_c);
        if (strcmp(bunka, "[max]")==0){
            r_c->okno[0] = rowmax +1;
            r_c->okno[1] = cellmax +1;
        }
        else
        {
            r_c->okno[0] = rowmin +1;
            r_c->okno[1] = cellmin +1;
        }
        
    }
    
}

//funkce pro prikaz find STR
//z aktualne vybrannych bunek vybere bunku obsahujici retezec STR
void find(tab_t* tab, char* prikaz, vybrane_bunky* r_c)
{
    char str[1000] = "";
    for (unsigned i = 6, j = 0; i < strlen(prikaz)-1; i++, j++)
    {
        str[j] = prikaz[i];
    }
    for (int i = r_c->okno[0]-1; i <= r_c->okno[2]-1; i++)
    {
        for (int j = r_c->okno[1]-1; j <= r_c->okno[3]-1 ; j++)
        {
            if (strcmp(str, tab->radky[i].bunky[j].bunka) == 0)
            {
                v_init(r_c);
                r_c->okno[0] = i + 1;
                r_c->okno[1] = j + 1;
                r_c->cap = 2;
                break;
            }
            
        }
    }     
}

//funkce pro prikaz set STR
//vlozi retezec STR do vybranne bunky
void set(tab_t* tab, char* str, vybrane_bunky* r_c)
{
    int i = 4, j = 0;
    char new_str[1000] = "";
    while (str[i]!='\0')
    {
        new_str[j] = str[i];
        i++;
        j++;
    }
    strcpy(tab->radky[r_c->okno[0] - 1].bunky[r_c->okno[1] - 1].bunka, new_str);
}

//funkce pro prikaz swap [R,C]
//vymeni obsah vybranne bunky a bunky [R,C]
//kde R je cislo radku, a C je cislo sloupce
void swap(tab_t* tab, char* bunka, vybrane_bunky* r_c)
{
    //printf("%d,%d\n", r_c->okno[0], r_c->okno[1]);
    int cell;
    int row;
    char cislo[5] = "";
    int i = 0, j = 0;
    while (bunka[i]!='\0')
    {
        if ((bunka[i]>=48)&&(bunka[i]<=57))
        {
            cislo[j] = bunka[i];
            j++;
        }
        else if (bunka[i] == ',')
        {
            row = atoi(cislo);
            j = 0;
            memset(cislo, ' ', 5);
        }
        i++;
        
    }
    cell = atoi(cislo);
    char tmp[1000] = "";
    sprintf(tmp, "%s", tab->radky[r_c->okno[0] - 1].bunky[r_c->okno[1] - 1].bunka);
    strcpy(tab->radky[r_c->okno[0] - 1].bunky[r_c->okno[1] - 1].bunka, tab->radky[row - 1].bunky[cell - 1].bunka);
    strcpy(tab->radky[row - 1].bunky[cell - 1].bunka, tmp);
}

//funkce pro prikaz len [R,C]
//spocita delku vybranne bunky, a vpise vysledok do bunky [R,C]
void len(tab_t* tab, char* bunka, vybrane_bunky* r_c)
{
    int cell;
    int row;
    char cislo[5] = "";
    int i = 4, j = 0;
    while (bunka[i]!='\0')
    {
        if ((bunka[i]>=48)&&(bunka[i]<=57))
        {
            cislo[j] = bunka[i];
            j++;
        }
        else if (bunka[i] == ',')
        {
            row = atoi(cislo);
            j = 0;
            memset(cislo, ' ', 5);
        }
        i++;
        
    }
    cell = atoi(cislo);
    memset(cislo, ' ', 5);
    sprintf(cislo, "%ld", strlen(tab->radky[r_c->okno[0] - 1].bunky[r_c->okno[1]-1].bunka));
    strcpy(tab->radky[row-1].bunky[cell-1].bunka, cislo);
}



//funkce pro vyber bunek
//param sloupci - maximalni slopec
//param radky - maximalni radek 
void vyber_bunek(char* bunky, vybrane_bunky* r_c, int sloupci, int radky)
{
    char str[20] = "";
    for (unsigned i = 1, j = 0; i < strlen(bunky) - 1; i++, j++)
    {
        str[j] = bunky[i];
    }

    int k = 0;
    char cislo[10] = "";
    int j = 0;
    if(strcmp(str, "_,_")!= 0){
        while (str[k]!= '\0')
        {
            if (str[k] == ',')
            {
                if(strchr(cislo, '_')){
                    if (r_c->cap == 2)
                        r_c->okno[r_c->cap] = radky + 1;
                    else
                        r_c->okno[r_c->cap] = sloupci + 1;
                }    
                else    
                    r_c->okno[r_c->cap] = atoi(cislo);
                r_c->cap++;
                j = 0;
            }
            else
            {
                cislo[j] = str[k];
            }
            k++;
        }
        if(strchr(cislo, '_'))
            r_c->okno[r_c->cap] = sloupci + 1;
        else    
            r_c->okno[r_c->cap] = atoi(cislo);
        r_c->cap++;
        if (strstr(str, ",_")&&(r_c->cap==2))
        {
            r_c->okno[1] = 1;
            r_c->okno[2] = r_c->okno[0];
            r_c->okno[3] = sloupci + 1;
            r_c->cap++;
        }
        if (strstr(str, "_,")&&(r_c->cap==2))
        {
            r_c->okno[0] = 1;
            r_c->okno[3] = r_c->okno[1];
            r_c->okno[2] = radky + 1;
            r_c->cap++;
        }
        
    }
    else
    {
        r_c->okno[0] = 1;
        r_c->okno[1] = 1;
        r_c->okno[2] = radky + 1;
        r_c->okno[3] = sloupci + 1;
    }
  
}

// v mainu otevira se soubor
// struktura tabulky naplni se hodnotami
// vola se nacteni prikazu 
// a do souboru ulozi se nova tabulka
int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Nenalezeny zadne argumenty\n");     
        return 1;   
    }
    
    char* delimy = "";
    if (strcmp(argv[1], "-d") == 0)
        delimy = detected_delim(argv[2]); 
    else
        delimy = " ";   
    
    
    tab_t tabulka;
    tab_ctor(&tabulka);
    FILE *file;
    file = fopen(argv[argc-1], "r");
    if (file == NULL)
    {
        fprintf(stderr, "Nepovedlo se otevrit soubor");
        return 1;
    }
    
    char c = ' ';
    int pocet_radku = 0, pocet_sloupcu = 0;
    int n_radku = 0;
    int n_sloupcu = 0;
    while ((c = getc(file)) != EOF){
        if (c != '\n'){
            if (!strchr(delimy, c)){
                if (c == '"')
                {
                    tabulka.radky[tabulka.radek_n].bunky[tabulka.radky[tabulka.radek_n].bunka_n].bunka[tabulka.radky[tabulka.radek_n].bunky[tabulka.radky[tabulka.radek_n].bunka_n].b_len] = c;
                    tabulka.radky[tabulka.radek_n].bunky[tabulka.radky[tabulka.radek_n].bunka_n].b_len++;
                    while ((c = getc(file))!= '"')
                    {
                        tabulka.radky[tabulka.radek_n].bunky[tabulka.radky[tabulka.radek_n].bunka_n].bunka[tabulka.radky[tabulka.radek_n].bunky[tabulka.radky[tabulka.radek_n].bunka_n].b_len] = c;
                        tabulka.radky[tabulka.radek_n].bunky[tabulka.radky[tabulka.radek_n].bunka_n].b_len++;
                        if(tabulka.radky[tabulka.radek_n].bunky[tabulka.radky[tabulka.radek_n].bunka_n].b_len == tabulka.radky[tabulka.radek_n].bunky[tabulka.radky[tabulka.radek_n].bunka_n].cap)
                        resize_bunky(&tabulka);
                    }
                }
                
                tabulka.radky[tabulka.radek_n].bunky[tabulka.radky[tabulka.radek_n].bunka_n].bunka[tabulka.radky[tabulka.radek_n].bunky[tabulka.radky[tabulka.radek_n].bunka_n].b_len] = c;
                tabulka.radky[tabulka.radek_n].bunky[tabulka.radky[tabulka.radek_n].bunka_n].b_len++;
                if(tabulka.radky[tabulka.radek_n].bunky[tabulka.radky[tabulka.radek_n].bunka_n].b_len == tabulka.radky[tabulka.radek_n].bunky[tabulka.radky[tabulka.radek_n].bunka_n].cap)
                    resize_bunky(&tabulka);
            }
            else{
                
                resize_radka(&tabulka);
                b_ctor(&tabulka);
                n_sloupcu = tabulka.radky[tabulka.radek_n].bunka_n;
                if (n_sloupcu > pocet_sloupcu)
                {
                    pocet_sloupcu = n_sloupcu;
                }
                
            }
            
        }
        else{
            n_radku = tabulka.radek_n;
            if (n_radku > pocet_radku)
            {
                pocet_radku = n_radku;
            }
            
            
            resize_t(&tabulka);
            r_ctor(&tabulka);
        }
        
    }
    fclose(file);
    special_resize_tabulky(&tabulka, pocet_radku, pocet_sloupcu);

    nacteni_prikazu(argv[argc - 2], &tabulka, &pocet_sloupcu, &pocet_radku);
    file = fopen(argv[argc-1], "w");
    if (file == NULL)
    {
        fprintf(stderr, "Nepovedlo se otevrit soubor");
        return 1;
    }
    for (int i = 0; i <= pocet_radku; i++)
    {
        for (int j = 0; j <= pocet_sloupcu; j++)
        {
            fprintf(file, "%s", tabulka.radky[i].bunky[j].bunka);
            if (j < pocet_sloupcu)
            {
                fprintf(file, "%c", delimy[0]);
            }
            
            free(tabulka.radky[i].bunky[j].bunka);
        }
        
        fprintf(file, "%s", "\n");
    }
    fclose(file);
    free(tabulka.radky);
    return 0;
}