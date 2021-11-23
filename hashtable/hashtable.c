/*
 * Tabuľka s rozptýlenými položkami
 *
 * S využitím dátových typov zo súboru hashtable.h a pripravených kostier
 * funkcií implementujte tabuľku s rozptýlenými položkami s explicitne
 * zreťazenými synonymami.
 *
 * Pri implementácii uvažujte veľkosť tabuľky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptyľovacia funkcia ktorá pridelí zadanému kľúču index z intervalu
 * <0,HT_SIZE-1>. Ideálna rozptyľovacia funkcia by mala rozprestrieť kľúče
 * rovnomerne po všetkých indexoch. Zamyslite sa nad kvalitou zvolenej funkcie.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializácia tabuľky — zavolá sa pred prvým použitím tabuľky.
 */
void ht_init(ht_table_t *table) {
  // pokud není tabulka alokovaná, ukončuji alokaci
  if (!table) return;

  // na všech indexech nastavíme NULL
  for (int i = 0; i < HT_SIZE; i++) {
    (*table)[i] = NULL;
  }
}

/*
 * Vyhľadanie prvku v tabuľke.
 *
 * V prípade úspechu vráti ukazovateľ na nájdený prvok; v opačnom prípade vráti
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
  // kontrola, že byl předán klíč a table není prázdná
  if (!table || !key) return NULL;
  
  // hledám jen na indexu daného klíče
  int index = get_hash(key);

  if (table == NULL) {
    return NULL;
  }

  // pokud ho najdu, vrátím ho, jinak NULL
  for (ht_item_t *temp = (*table)[index]; temp != NULL; temp = temp->next)
    if (strcmp(temp->key, key) == 0) return temp;

  return NULL;
}

/*
 * Vloženie nového prvku do tabuľky.
 *
 * Pokiaľ prvok s daným kľúčom už v tabuľke existuje, nahraďte jeho hodnotu.
 *
 * Pri implementácii využite funkciu ht_search. Pri vkladaní prvku do zoznamu
 * synonym zvoľte najefektívnejšiu možnosť a vložte prvok na začiatok zoznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
  if (!table || !key) return;
  
  // kontrola, jestli daný key už v tabulce není
  ht_item_t *temp = ht_search(table, key);

  // pokud tam již je, zaměním mu akorát hodnotu a ukončím funkci
  if (temp) {
    temp->value = value;
    return;
  }

  // nový prvek uložím na index odvozený z key
  ht_item_t *new = malloc(sizeof(ht_item_t));
  if (!new) return;

  new->key = key;
  new->value = value;

  int index = get_hash(key);
  new->next = (*table)[index];
  (*table)[index] = new;
}

/*
 * Získanie hodnoty z tabuľky.
 *
 * V prípade úspechu vráti funkcia ukazovateľ na hodnotu prvku, v opačnom
 * prípade hodnotu NULL.
 *
 * Pri implementácii využite funkciu ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
  if (!table || !key) return NULL;
  
  // ht_search najde hledaný prvek a uloží jej do temp
  ht_item_t *temp = ht_search(table, key);

  // pokud byl prvek nalezen, vrať jeho hodnotu, jinak NULL
  if (temp) { return &temp->value; }
  
  return NULL;
}

/*
 * Zmazanie prvku z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje priradené k danému prvku.
 * Pokiaľ prvok neexistuje, nerobte nič.
 *
 * Pri implementácii NEVYUŽÍVAJTE funkciu ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
  if (!table || !key) return;
  
  ht_item_t *prev = NULL;

  // najdu, na kterém indexu daná položka bude
  int index = get_hash(key);
  if (!table[index]) {
    return;
  }

  // procházím, dokud nenarazím na správný prvek
  ht_item_t *temp = (*table)[index];
  while (temp && strcmp(temp->key, key) != 0) {
    prev = temp;
    temp = temp->next;
  }

  // pokud nebyl nalezen, ukončuji funkci
  if (temp == NULL) return;
  
  // pokud odstraňovaný prvek je první prvek, je potřeba první prvek nahradit druhým
  else if (temp == (*table)[index])
    (*table)[index] = temp->next;
  
  else
    prev->next = temp->next;

  free(temp);
}

/*
 * Zmazanie všetkých prvkov z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje a uvedie tabuľku do stavu po
 * inicializácii.
 */
void ht_delete_all(ht_table_t *table) {
  if (!table) return;
  
  ht_item_t *temp, *next;

  // projdu celé pole
  for (int i = 0; i < HT_SIZE; i++) {
    temp = (*table)[i];
    if (temp) {
      // projdu veškeré prvky seznamu na daném indexu a uvolním je
      while (temp != NULL) {
        next = temp->next;
        free(temp);
        temp = next;
      }
      (*table)[i] = NULL;
    }
  }
}
