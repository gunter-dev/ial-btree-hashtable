/*
 * Binárny vyhľadávací strom — rekurzívna varianta
 *
 * S využitím dátových typov zo súboru btree.h a pripravených kostier funkcií
 * implementujte binárny vyhľadávací strom pomocou rekurzie.
 */

#include "../btree.h"
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
void bst_init(bst_node_t **tree) 
{ 
  (*tree) = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) 

{
  if(tree == NULL)  //pokud je ukazatel na koren NULL, ihned ukoncim
  {
    return(false);
  }
  
  else
  {
    if(tree->key == key)  
    {
      *value = tree->value; //zapisu data do promenne
      return(true); //zde to vratim, protoze byl nalezen
    }
    else
    {
      if(tree->key > key)
      {
        return(bst_search(tree->left, key, value)); //rekurzivne volam hledani pro levy uzel
      }
      else
      {
        return(bst_search(tree->right, key, value));  //rekurzivne volam volani pro pravy uzel
      }
    }
  }
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
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) 
{
  if((*tree) == NULL)
  {
    bst_node_t *pomocna = malloc(sizeof(struct bst_node));  //naalokuji si polozku
    if(pomocna == NULL) //pro jistotu, kdyby se nepovedl malloc
    {
      return;
    }
    else
    { //zapisu si klic, data a ukazatele
      pomocna->value = value;
      pomocna->key = key;
      pomocna->left = NULL;
      pomocna->right = NULL;
      (*tree) = pomocna;  //vlozim polozku do stromu
    }
  }
  else  //jestli existuje koren
  {
    if((*tree)->key > key)  
  {
    bst_insert(&(*tree) ->left, key, value);  //rekurzivne volam vkladani pro levy uzel
  }

  else
  {
    if((*tree)->key < key)
    {
      bst_insert(&(*tree)->right, key, value);  //rekurzivne vola mvkladani pro pravy uzel
    }
    else
    {
      (*tree)->value = value;
    }
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
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree)
{
  if((*tree) != NULL)
  {
    if((*tree)->right == NULL)  //jestli je pravy uzel NULL, tak jsem v tom uplne napravo
    {
      target->key = (*tree)->key; //presun uzlu toho nejvic napravo
      target->value = (*tree)->value;
    }
    else
    {
      bst_replace_by_rightmost(target, &(*tree)->right);  //volani rekurzivne s pravym uzlem
    }
  }
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
 * Funkciu implementujte rekurzívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key)
{
  if((*tree) != NULL) //pokud strom neni prazdny
  {
    if((*tree)->key > key)
    {
      bst_delete(&(*tree)->left, key);  //tak zde volame rekurzivne mazani pro ten levy uzel
    }
    
    else
    {
      if((*tree)->key < key)
      {
        bst_delete(&(*tree)->right, key); //zde volame rekurzivne mazani pro pravy uzel
      }

      else
      {
        if((*tree)->left == NULL && (*tree)->right == NULL) //oba ukazatele jsou NULL
        {
          free((*tree));  //uvolnim ukazatele
          (*tree) = NULL; //nastavim si ukazatel na NULL
        }

        else
        {
          if((*tree)->left != NULL && (*tree)->right != NULL) //oba uzkazatele jsou ruzne od NULL
          {
            bst_node_t *pomocna;  //vytvorim si pomocnou promenou
            pomocna=(*tree);  //ulozim si uzel do promenne
            bst_replace_by_rightmost(pomocna,&(*tree)->left); //zavolam si funkci abych se mohl dostat co nejvic v pravo pro levy uzel
            (*tree)->key = pomocna->key;
            (*tree)->value = pomocna->value;
            bst_delete (&(*tree)->left,pomocna->key); //zde si zavolam funkci delete( rekurzivne) pro levy uzel
          }
          else
          {
            bst_node_t *pomocna2; //vytvorim si dalsi pomocnou promennou
            if((*tree)->left == NULL) //levy ukazatel je NULL
            {
              pomocna2 = (*tree);
              (*tree) = (*tree)->right; //prepisu si uzel, polozkou vpravo
            }
            else
            {
              pomocna2 = (*tree);
              (*tree) = (*tree)->left;
            }
            free(pomocna2); //uvolneni promenne
          }
        }
      }
    }
  } 
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) 
{
  if((*tree) != NULL) //jestli strom neni prazdny
  {
    bst_dispose(&(*tree)->left);  //zavolam si funkci rekurzivne pro levy uzel
    bst_dispose(&(*tree)->right); // --||-- pro pravy uzel
    free((*tree));  //uvolnim ukazatele polozky
    (*tree) = NULL;
  }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) 
{
  if(tree != NULL)
  {
    bst_print_node(tree);
    bst_preorder(tree->left);
    bst_preorder(tree->right);

  }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) 
{
  if(tree != NULL)
  {
   bst_inorder(tree->left);
   bst_print_node(tree);
   bst_inorder(tree->right);
  }
}
/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) 
{
  if(tree != NULL)
  {
    bst_postorder(tree->left);
    bst_postorder(tree->right);
    bst_print_node(tree);
  }
}
