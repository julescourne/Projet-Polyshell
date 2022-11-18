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
#include "misc/filesystem.h"
#include "misc/string.h"
#include "interactive/autocomplete.h"
#include "interactive/history.h"

// #########################################################################
// #########################################################################
// #########################################################################

MAKE_NEW_2(History, const char*, unsigned int)
MAKE_DEL_1(History, const char*)

int IMPLEMENT(History_init)(History *history, const char *filename, unsigned int length)
{
    //return provided_History_init(history, filename, length);
    if(Fifo_init(&history->storage,length,COMPOSE)){
        return 1;
    }
    history->position=history->storage.tail;
    if(filename){
        char* path = prependHomeDir(duplicateString(filename));
        if(path)
        {
            FILE *file=fopen(path,"r");
            if(file){
                FileIterator it ;
                if(!FileIterator_init(&it,file))
                {
                    while(!FileIterator_isOver(&it))
                    {
                        History_add(history,FileIterator_get(&it));
                        FileIterator_next(&it);
                    }FileIterator_finalize(&it);
                }fclose(file);
            }free(path);
        }
    }return 0 ;
}

void IMPLEMENT(History_finalize)(History *history, const char *filename)
{
    //provided_History_finalize(history, filename);
   if(filename){
        char *newFilename = prependHomeDir(duplicateString(filename));
        FILE *file=fopen(newFilename,"w+");
            if(file){ char* chaineAajouter=NULL;
          unsigned int i =history->storage.head;
                while( i!=history->storage.tail)
                {  chaineAajouter=getProtString(history->storage.storage[i],'#');
                   if(isNotEmpty(chaineAajouter))
                   { fprintf(file,"%s\n",chaineAajouter);
                    i=(i+1)%history->storage.capacity;
                    free(chaineAajouter);
                   }
                }
                fclose(file);
            }free(newFilename);
    }
    Fifo_finalize(&history->storage);
    
}

void IMPLEMENT(History_clear)(History *history)
{
    //provided_History_clear(history);
    Fifo_clear(&history->storage);
    history->position=history->storage.tail;
}

void IMPLEMENT(History_add)(History *history, const char *cmd)
{
    //provided_History_add(history, cmd);
    if(history!=NULL
    && cmd    !=NULL
    && isNotEmpty(cmd))  
    {if (Fifo_full(&history->storage))
    {
        Fifo_pop(&history->storage);
    }Fifo_push(&history->storage,cmd);
    history->position=history->storage.tail;}
}

const char* IMPLEMENT(History_up)(History *history)
{
    //return provided_History_up(history);
    /*qui retourne la dernière commande saisie par l'utilisateur. Il doit être possible de remonter dans l'historique en appelant plusieurs fois cette fonction. La fonction renvoie NULL si l'utilisateur a déjà consulté la plus vieille commande contenue dans l'historique. */
    if(history->position==history->storage.head)
    {
        return NULL;
    }if(history->position==0)
    {
        history->position=history->storage.capacity-1;
    }
    else{
    --history->position;
    }
    return history->storage.storage[history->position];
}

const char* IMPLEMENT(History_down)(History *history)
{
    //return provided_History_down(history);
    if(history->position==history->storage.tail)
    {
        return NULL;
    }
    history->position=(history->position+1)%history->storage.capacity;
    if(history->position==history->storage.tail)
    {
        return "";
    }
    else {
    return history->storage.storage[history->position];
}
}
