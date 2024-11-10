/*
 * Binární vyhledávací strom — rekurzivní varianta
 *
 * S využitím datových typů ze souboru btree.h a připravených koster funkcí
 * implementujte binární vyhledávací strom pomocí rekurze.
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializace stromu.
 *
 * Uživatel musí zajistit, že inicializace se nebude opakovaně volat nad
 * inicializovaným stromem. V opačném případě může dojít k úniku paměti (memory
 * leak). Protože neinicializovaný ukazatel má nedefinovanou hodnotu, není
 * možné toto detekovat ve funkci.
 */
void bst_init(bst_node_t **tree)
{
  *tree = NULL;
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * ukazatel na obsah daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 *
 * Funkci implementujte rekurzivně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, bst_node_content_t **value)
{
  if (tree == NULL)
  {
    return false;
  }
  if (tree->key == key)
  {
    *value = &tree->content;
    return true;
  }
  if (tree->key < key)
  {
    return bst_search(tree->left, key, value);
  }
  else
  {
    return bst_search(tree->right, key, value);
  }
}

/*
 * Vložení uzlu do stromu.
 *
 * Pokud uzel se zadaným klíče už ve stromu existuje, nahraďte jeho hodnotu.
 * Jinak vložte nový listový uzel.
 *
 * Výsledný strom musí splňovat podmínku vyhledávacího stromu — levý podstrom
 * uzlu obsahuje jenom menší klíče, pravý větší.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, bst_node_content_t value)
{
  if (*tree == NULL)
  {
    bst_node_t *new_note = (bst_node_t *)malloc(sizeof(bst_node_t));
    if (new_note == NULL)
    {
      return;
    }
    new_note->content = value;
    new_note->key = key;
    new_note->left = NULL;
    new_note->right = NULL;
    *tree = new_note;
    return;
  }
  else if ((*tree)->key > key)
  {
    bst_insert(&(*tree)->left, key, value);
  }
  else if ((*tree)->key < key)
  {
    bst_insert(&(*tree)->right, key, value);
  }
  else
  {
    if ((*tree)->content.value != NULL)
    {
      free((*tree)->content.value);
    }
    (*tree)->content = value;
  }
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 *
 * Klíč a hodnota uzlu target budou nahrazeny klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 *
 * Tato pomocná funkce bude využitá při implementaci funkce bst_delete.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree)
{
  if ((*tree)->right != NULL)
  // dokud je pravější uzel, tak jdeme hloub2ji do rekurze
  {
    bst_replace_by_rightmost(target, &(*tree)->right);
  }
  else
  {
    if (target->content.value != NULL)
    // uvolnění value, pokud existuje
    {
      free(target->content.value);
    }
    target->key = (*tree)->key;
    target->content.value = (*tree)->content.value;
    bst_node_t *tmp = *tree;
    *tree = (*tree)->left; // prvně nahrazení levým uzlem pokud existuje
    free(tmp);             // uvolnění bývalého nejpravějšího uzlu
  }
}

/*
 * Odstranění uzlu ze stromu.
 *
 * Pokud uzel se zadaným klíčem neexistuje, funkce nic nedělá.
 * Pokud má odstraněný uzel jeden podstrom, zdědí ho rodič odstraněného uzlu.
 * Pokud má odstraněný uzel oba podstromy, je nahrazený nejpravějším uzlem
 * levého podstromu. Nejpravější uzel nemusí být listem.
 *
 * Funkce korektně uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkci implementujte rekurzivně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key)
{
  if (*tree == NULL)
  {
    // Uzel se zadaným klíčem neexistuje.
    return;
  }

  if ((*tree)->key > key)
  {
    // Hledáme uzel v levém podstromu.
    bst_delete(&(*tree)->left, key);
  }
  else if ((*tree)->key < key)
  {
    // Hledáme uzel v pravém podstromu.
    bst_delete(&(*tree)->right, key);
  }
  else
  {
    // Uzel s klíčem key nalezen.
    if ((*tree)->left == NULL)
    {
      // Uzel má pravý nebo žádný podstrom.
      bst_node_t *temp = *tree;
      *tree = (*tree)->right;
      if (temp->content.value != NULL)
      {
        free(temp->content.value);
      }
      free(temp);
    }
    else if ((*tree)->right == NULL)
    {
      // Uzel má pouze levý podstrom.
      bst_node_t *temp = *tree;
      *tree = (*tree)->left;
      if (temp->content.value != NULL)
      {
        free(temp->content.value);
      }
      free(temp);
    }
    else
    {
      // Uzel má oba podstromy.
      bst_replace_by_rightmost(*tree, &(*tree)->left);
    }
  }
}

/*
 * Zrušení celého stromu.
 *
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených
 * uzlů.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree)
{
  if (*tree == NULL)
  {
    // Strom je již prázdný.
    return;
  }

  bst_dispose(&(*tree)->left);
  bst_dispose(&(*tree)->right);

  if ((*tree)->content.value != NULL)
  {
    free((*tree)->content.value);  // Uvolní dynamicky alokovanou paměť
    (*tree)->content.value = NULL; // Nastaví ukazatel na NULL
  }
  (*tree)->content.type = INTEGER; // je to navic, ale chci i contebt.type nastavit na vyzchozi hodnotu
  (*tree)->key = -1;               // je to navic, ale chci i key nastavit na vyzchozi hodnotu
  free(*tree);
  *tree = NULL;
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items)
{
  if (tree == NULL)
  {
    // dosel na konec
    return;
  }

  bst_add_node_to_items(tree, items); // preorder prvne zpracuje aktualni uzel

  bst_preorder(tree->left, items);  // pote zpracuje levy podstrom
  bst_preorder(tree->right, items); // a nakonec pravy podstrom
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items)
{
  if (tree == NULL)
  {
    // dosel na konec
    return;
  }

  bst_inorder(tree->left, items); // inorder prvne zpracuje levy podstrom

  bst_add_node_to_items(tree, items); // pote zpracuje aktualni uzel

  bst_inorder(tree->right, items); // a nakonec pravy podstrom
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items)
{
  if (tree == NULL)
  {
    // dosel na konec
    return;
  }

  bst_postorder(tree->left, items); // postorder prvne zpracuje levy podstrom

  bst_postorder(tree->right, items); // pote zpracuje pravy podstrom

  bst_add_node_to_items(tree, items); // a nakonec aktualni podstrom
}
