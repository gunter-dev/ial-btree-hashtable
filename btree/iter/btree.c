/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {
  // inicializace prázdného seznamu
  *tree = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  // kontrola, že seznam není prázdný
  if (!tree) return false;
  
  bst_node_t *temp = tree;
  while (temp) {
    // pokud najdu hledaný key, vrátím přes *value hodnotu node
    if (temp->key == key) {
      *value = temp->value;
      return true;
    }
    // pokud je menší/větší, uloží do temp potomka a provede kontrolu na potomkovi
    else if (temp->key < key) temp = temp->right;
    else if (temp->key > key) temp = temp->left;
    else return false;
  }
  return false;
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  // pokud je seznam prázdný, vytvoříme ho jako první prvek
  if (!(*tree)) {
    if (!((*tree) = malloc(sizeof(bst_node_t)))) return;
    (*tree)->key = key;
    (*tree)->value = value;
    (*tree)->left = (*tree)->right = NULL;
    
    return;
  }
  
  bst_node_t *temp = *tree;
  
  // podle key najdu, kam je potřeba nový prvek uložit
  while (1) {
    if (temp->key < key) {
      // pokud není kam pokračovat, ulož tam nový prvek
      if (temp->right == NULL) {
        bst_node_t *new = malloc (sizeof(bst_node_t));
        new->key = key;
        new->value = value;
        new->left = new->right = NULL;
        temp->right = new;
        return;
      }
      temp = temp->right;
    }
    else if (temp->key > key) {
      if (temp->left == NULL) {
        bst_node_t *new = malloc (sizeof(bst_node_t));
        new->key = key;
        new->value = value;
        new->left = new->right = NULL;
        temp->left = new;
        return;
      }
      temp = temp->left;
    }
    // výměna value, pokud se již ve stromu nachází
    else {
      temp->value = value;
      return;
    }
  }
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  // uchovávám si aktuální prvek a prvek jemu nadřazený
  bst_node_t *temp = *tree;
  bst_node_t *prev = *tree;
  
  // hledám prvek, který je nejvíce vpravo, tedy nemá žadný další pravý prvek
  while (temp->right) {
    prev = temp;
    temp = temp->right;
  }
  
  target->key = temp->key;
  target->value = temp->value;

  if (prev) prev->right = NULL;

  // pokud měl prvek ještě levého potomka, uložím ho nadřazenému prvku na jeho bývalé místo
  if (temp->left) {
    prev->right = temp->left;
  }

  free(temp);
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
  if (!(*tree)) return;

  bst_node_t *temp = *tree;
  bst_node_t *prev = *tree;
  // jestli je aktuální prvek levým nebo pravým prvkem jemu nadřazenému
  bool fromLeft = false;

  // hledám prvek ke smazání
  while (temp) {
    if (temp->key == key) {
      break;
    }
    else if (temp->key < key) {
      prev = temp;
      temp = temp->right;
      fromLeft = false;
    }
    else if (temp->key > key) {
      prev = temp;
      temp = temp->left;
      fromLeft = true;
    }
  }

  // pokud nebyl prveek nalezen, končím funkci
  if (temp == NULL) return;

  // pokud má prvek oba podstromy, nahradím ho nejpravějším uzlem
  if (temp->left && temp->right) {
    bst_replace_by_rightmost(temp, &(temp->left));
  }
  // pokud má jen jeden, dědí ho otec
  else {
    if (temp->right) {
      if (fromLeft) prev->left = temp->right;
      else prev->right = temp->right;
    }
    else {
      if (fromLeft) prev->left = temp->left;
      else prev->right = temp->left;
    }

    free(temp);
  }
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
  if (!(*tree)) return;

  // inicializace zásobníku
  stack_bst_t *stack = malloc(sizeof(stack_bst_t));
  if (!stack) return;
  stack_bst_init(stack);

  bst_node_t *temp = *tree;

  // jdu k nejlevějšímu prvku, prvky postupně uvolňuji
  // pravé prvky si přitom pushuji na stack, abych je
  // mohl později zpracovat a smazat
  while (!stack_bst_empty(stack) || temp) {
    if (temp == NULL) {
      temp = stack_bst_pop(stack);
    }
    
    if (temp->right) {
      stack_bst_push(stack, temp->right);
    }

    bst_node_t *to_delete = temp;
    temp = temp->left;
    free(to_delete);
  }

  *tree = NULL;
  free(stack);
}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) {
  bst_node_t *temp = tree;
  while (temp) {
    bst_print_node(temp);
    stack_bst_push(to_visit, temp);
    temp = temp->left;
  }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
  stack_bst_t *stack = malloc(sizeof(stack_bst_t));
  stack_bst_init(stack);
  bst_leftmost_preorder(tree, stack);

  while (!stack_bst_empty(stack)) {
    tree = stack_bst_pop(stack);
    bst_leftmost_preorder(tree->right, stack);
  }

  free(stack);
}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
  bst_node_t *temp = tree;

  while (temp) {
    stack_bst_push(to_visit, temp);
    temp = temp->left;
  }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
  stack_bst_t *stack = malloc(sizeof(stack_bst_t));
  stack_bst_init(stack);
  
  bst_leftmost_inorder(tree, stack);

  while(!stack_bst_empty(stack)) {
    tree = stack_bst_pop(stack);
    bst_print_node(tree);
    bst_leftmost_inorder(tree->right, stack);
  }

  free(stack);
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {
  bst_node_t *temp = tree;
  while (temp) {
    stack_bst_push(to_visit, temp);
    stack_bool_push(first_visit, true);
    temp = temp->left;
  }
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
  stack_bst_t *bst_stack = malloc(sizeof(stack_bst_t));
  stack_bool_t *bool_stack = malloc(sizeof(stack_bool_t));
  bool tempBool;
  bst_node_t *tempBst = tree;

  stack_bst_init(bst_stack);
  stack_bool_init(bool_stack);
  bst_leftmost_postorder(tree, bst_stack, bool_stack);

  while (!stack_bst_empty(bst_stack)) {
    tempBst = stack_bst_pop(bst_stack);
    tempBool = stack_bool_pop(bool_stack);
    if (tempBool) {
      stack_bst_push(bst_stack, tempBst);
      stack_bool_push(bool_stack, false);
      bst_leftmost_postorder(tempBst->right, bst_stack, bool_stack);
    }
    else bst_print_node(tempBst);
  }

  free(bst_stack);
  free(bool_stack);
}
