/*
 * Tabulka s rozptýlenými položkami
 *
 * S využitím datových typů ze souboru hashtable.h a připravených koster
 * funkcí implementujte tabulku s rozptýlenými položkami s explicitně
 * zretězenými synonymy.
 *
 * Při implementaci uvažujte velikost tabulky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptylovací funkce která přidělí zadanému klíči index z intervalu
 * <0,HT_SIZE-1>. Ideální rozptylovací funkce by měla rozprostírat klíče
 * rovnoměrně po všech indexech. Zamyslete sa nad kvalitou zvolené funkce.
 */
int get_hash(char *key)
{
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++)
  {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializace tabulky — zavolá sa před prvním použitím tabulky.
 */
void ht_init(ht_table_t *table)
{
  if (*table != NULL)
  {
    for (int i = 0; i < HT_SIZE; i++)
    {
      (*table)[i] = NULL; // nastaví každý prvek tabulky na NULL
    }
  }
}

/*
 * Vyhledání prvku v tabulce.
 *
 * V případě úspěchu vrací ukazatel na nalezený prvek; v opačném případě vrací
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key)
{
  for (int i = 0; i < HT_SIZE; i++)
  {
    ht_item_t *current = (*table)[i];
    while (current != NULL)
    {
      if (strcmp(current->key, key) == 0)
      {
        return current;
      }
      ht_item_t *tmp = current->next;
      current = tmp;
    }
  }
  return NULL;
}

/*
 * Vložení nového prvku do tabulky.
 *
 * Pokud prvek s daným klíčem už v tabulce existuje, nahraďte jeho hodnotu.
 *
 * Při implementaci využijte funkci ht_search. Pri vkládání prvku do seznamu
 * synonym zvolte nejefektivnější možnost a vložte prvek na začátek seznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value)
{
  ht_item_t *item_key_exists = ht_search(table, key);
  if (item_key_exists != NULL)
  {
    item_key_exists->value = value;
    return;
  }

  ht_item_t *new_item = (ht_item_t *)malloc(sizeof(ht_item_t));
  if (new_item == NULL)
  {
    return;
  }
  new_item->key = (char *)malloc(strlen(key) + 1);
  if (new_item->key != NULL)
  {
    strcpy(new_item->key, key);
  }
  else
  {
    return;
  }
  new_item->next = NULL;
  new_item->value = value;

  int index = get_hash(key) % HT_SIZE;

  new_item->next = (*table)[index];
  (*table)[index] = new_item;

  return;
}

/*
 * Získání hodnoty z tabulky.
 *
 * V případě úspěchu vrací funkce ukazatel na hodnotu prvku, v opačném
 * případě hodnotu NULL.
 *
 * Při implementaci využijte funkci ht_search.
 */
float *ht_get(ht_table_t *table, char *key)
{
  ht_item_t *item_ptr = ht_search(table, key);
  if (item_ptr != NULL)
  {
    return &item_ptr->value;
  }
  return NULL;
}

/*
 * Smazání prvku z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje přiřazené k danému prvku.
 * Pokud prvek neexistuje, funkce nedělá nic.
 *
 * Při implementaci NEPOUŽÍVEJTE funkci ht_search.
 */
void ht_delete(ht_table_t *table, char *key)
{
  for (int i = 0; i < HT_SIZE; i++)
  {
    ht_item_t *current = (*table)[i];
    ht_item_t *prev = NULL;
    while (current != NULL)
    {
      if (strcmp(current->key, key) == 0)
      {
        /*if (prev == NULL && current->next == NULL) není vlastně potřeba jelikož pokud je current->next NULL tak by se stejně uložil NULL do (*table)[i]
        {
          (*table)[i] = NULL;
        }
        else*/
        if (prev == NULL)
        {
          (*table)[i] = current->next;
        }
        else
        {
          prev->next = current->next;
        }
        free(current->key);
        free(current);
        return;
      }
      prev = current;
      current = current->next;
    }
  }
  return;
}

/*
 * Smazání všech prvků z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje a uvede tabulku do stavu po
 * inicializaci.
 */
void ht_delete_all(ht_table_t *table)
{
  for (int i = 0; i < HT_SIZE; i++)
  {
    ht_item_t *current = (*table)[i];
    while (current != NULL)
    {
      ht_item_t *tmp = current->next;
      free(current->key);
      free(current);
      current = tmp;
    }
    (*table)[i] = NULL;
  }
  return;
}
