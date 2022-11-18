/*-------------------------------------------------------------------------*
 | Copyright (C) 2018 Département Informatique de PolyTech Tours.          |
 |                                                                         |
 | This file is part of PolyShell, yet another shell.                      |
 |                                                                         |
 | PolyShell is free software; you can redistribute it and/or modify       |
 | it under the terms of the GNU General Public License as published by    |
 | the Free Software Foundation; either version 3 of the License,          |
 | or (at your option) any later version.                                  |
 |                                                                         |
 | PolyShell is distributed in the hope that it will be useful,            |
 | but WITHOUT ANY WARRANTY; without even the implied warranty of          |
 | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            |
 | GNU General Public License for more details.                            |
 |                                                                         |
 | You should have received a copy of the GNU General Public License       |
 | along with this program. If not, see <http://www.gnu.org/licenses/>.    |
 |                                                                         |
 | Additional permission under GNU GPL version 3 section 7 ---- If you     |
 | modify PolyShell, or any covered work, by linking or combining it with  |
 | libprovided (or a modified version of that library), containing parts   |
 | covered by the terms of the Creative Commons BY-NC-ND 4.0 International |
 | License, the licensors of  PolyShell grant you additional permission    |
 | to convey the resulting work.                                           |
 *-------------------------------------------------------------------------*/

#include "misc/fifo.h"
#include "misc/string.h"

// #########################################################################
// #########################################################################
// #########################################################################

MAKE_NEW_2(Fifo, unsigned int, FifoMode)
MAKE_DEL_0(Fifo)

int IMPLEMENT(Fifo_init)(Fifo *fifo, unsigned int capacity, FifoMode mode)
{
    //return provided_Fifo_init(fifo, capacity, mode);
    //qui initialise une file vide permettant de stocker capacity éléments au plus. Ces derniers seront composés ou agrégés selon le mode spécifié. 
    if(capacity)
        {
            fifo->storage=(char**)malloc((capacity+1)*sizeof(char*));
            fifo->capacity=capacity+1;
            if(fifo->storage==NULL)
                   {
                               exit(1);
                    }
                    fifo->head=0;
                    fifo->tail=0;
                    fifo->mode=mode;
                    return 0;}
        return 1;
}

void IMPLEMENT(Fifo_finalize)(Fifo *fifo)
{
    //provided_Fifo_finalize(fifo);
    Fifo_clear(fifo);
    free(fifo->storage);
}

void IMPLEMENT(Fifo_clear)(Fifo *fifo)
{
    //provided_Fifo_clear(fifo);
    //{sizeof(FIFo==0 && fiFo->HEAD==fifo->TAiL}
    while(fifo->head!=fifo->tail)
    {
        Fifo_pop(fifo);
    }
}

int IMPLEMENT(Fifo_push)(Fifo *fifo, const char *str)
{
    //return provided_Fifo_push(fifo, str);
    //qui rajoute la chaîne de caractères str à la fin de la file (attention au mode choisi !) et retourne un code d'erreur (0 en cas de succès). 
    if(Fifo_full(fifo))
    {
    return 1;
    }
     char *aAjouter=(char*)str;
    if (fifo->mode==COMPOSE)
    {
       aAjouter=duplicateString(str);
         if (!aAjouter)
         {
             return 1;
         }
    }
    fifo->storage[fifo->tail]=aAjouter;
    fifo->tail=(fifo->tail+1)%fifo->capacity;
        return 0;
}

const char* IMPLEMENT(Fifo_front)(const Fifo *fifo)
{//qui retourne un pointeur constant sur le plus vieil élément de la file ou NULL si celle-ci est vide. 
    //return provided_Fifo_front(fifo);
    if (Fifo_empty(fifo))
    {
        return NULL;
    }
    return fifo->storage[fifo->head];
}

int IMPLEMENT(Fifo_pop)(Fifo *fifo)
{
    //return provided_Fifo_pop(fifo);
    //qui supprime le plus vieil élément de fifo et retourne un code d'erreur. 
    if (Fifo_empty(fifo))
    {
        return 1;
    }
    if(fifo->mode==COMPOSE)
    {
        char* temp=fifo->storage[fifo->head];
        free(temp);
    }
    fifo->storage[fifo->head]=NULL;
    fifo->head=(fifo->head+1)%fifo->capacity;
    return 0;
}

int IMPLEMENT(Fifo_full)(const Fifo *fifo)
{
    //return provided_Fifo_full(fifo);
    //qui retourne vrai si fifo est pleine et faux sinon. 
    return ((fifo->tail+1)%fifo->capacity)==fifo->head;
}

int IMPLEMENT(Fifo_empty)(const Fifo *fifo)
{
    //return provided_Fifo_empty(fifo);
    //qui retourne vrai (1) si fifo est vide et faux (0) sinon. 
    if(fifo->head==fifo->tail)
    {
        return 1;
    }
    return 0;
}
