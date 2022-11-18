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

#include "interactive/all.h"
#include "misc/string.h"
#include <stdio.h>

// #########################################################################
// #########################################################################
// #########################################################################

MAKE_NEW_0(Input)
MAKE_DEL_0(Input)

int IMPLEMENT(Input_init)(Input *input)
{
    //return provided_Input_init(input);
    input->current=NULL;
    return 0;
}

void IMPLEMENT(Input_finalize)(Input *input)
{
    //provided_Input_finalize(input);
    if(input->current)
    {
            Cell *droite= input->current->next;
            Cell *gauche= input->current->previous;
            while(droite)
                {
                    Cell *temp=droite->next ;
                    Cell_delete(droite);
                    droite=temp;
                }
            while(gauche)
                {  
                    Cell *temp=gauche->previous ;
                    Cell_delete(gauche);
                    gauche=temp;
                }
                Cell_delete(input->current);
    }
}

void IMPLEMENT(Input_clear)(Input *input)
{
    //provided_Input_clear(input);
    Input_finalize(input);
    input->current=NULL;
    (void)input;
}

size_t IMPLEMENT(Input_size)(const Input *input)
{
    //return provided_Input_size(input);
    size_t size=0;
      if(input->current)
    {
            for(Cell *droite= input->current;droite;droite=droite->next)
                {
                    size +=Bucket_size(&droite->bucket);
                }
            for (Cell *gauche= input->current->previous;gauche;gauche=gauche->previous)
                {  
                     size +=Bucket_size(&gauche->bucket);
                }
    }
    return size;
}

char IMPLEMENT(Input_get)(const Input *input)
{
    //return provided_Input_get(input);
    if(input->current==NULL)
    {
        return '\0';
    }
    if(input->pos >input->current->bucket.top)
        {
        return '\0';
        }
    return input->current->bucket.content[input->pos];
}

int IMPLEMENT(Input_insert)(Input *input, char c)
{
    //return provided_Input_insert(input, c);
    if(!input->current)
    {
        Cell *nCell= Cell_new();
        if(!nCell)
        {
            return 1;
        }
        Bucket_insert(&nCell->bucket,0,c);
        input->current=nCell;
        input->pos=1;
    }else
    {
        if(Bucket_full(&input->current->bucket))
        {
            Cell *nCell= Cell_new();
             if(!nCell)
                    {
                      return 1;
                    }
             Cell_insertAfter(input->current,nCell);
             if(input->pos>input->current->bucket.top)
                      {
                        input->current=nCell;
                        input->pos=0;
                       }else {
                            Bucket_move(&input->current->bucket,input->pos,&nCell->bucket);
                             }
        }
        Bucket_insert(&input->current->bucket,input->pos,c);
        Input_moveRight(input);
    }
    return 0;
}

int IMPLEMENT(Input_backspace)(Input *input)
{
    //return provided_Input_backspace(input);
       //return provided_Input_del(input);
    // cosiderons le cas d'une bucket vide
    if(input->current==NULL)
    {
        return 1;
    }
   if(input->current->previous==NULL && input->pos==0)
        {
            return 1;
        }

     Input_moveLeft(input);
    Input_del(input);
    return 0;
}

int IMPLEMENT(Input_del)(Input *input)
{
    
    //return provided_Input_del(input);
    // cosiderons le cas d'une bucket vide
    /*Il n'y a pas de truc compliqué à faire avec les lettres. En supprimant la lettre avec Bucket_remove, le décalage se fait au niveau du bucket et bien souvent, c'est suffisant ! Le seul truc à vérifier est 1) que le curseur est toujours sur une lettre et 2) que le bucket ne devient pas vide (auquel cas il faut le supprimer).*/
    if(!input->current)
    {
        return 1;
    }
   if(input->pos>input->current->bucket.top)
   {
       return 1;
   }
   else
       {
   Bucket_remove(&input->current->bucket,input->pos);
            if (input->pos>input->current->bucket.top)
            {
                if((Bucket_empty(&input->current->bucket)==1))
                {   Cell* droite=input->current;
                     if(input->current->next)
                    {
                        input->current->next->previous=input->current->previous;
                        if(input->current->previous)
                        {input->current->previous->next=input->current->next;}
                    input->current=input->current->next;}
                       droite->next=NULL;
                       droite->previous=NULL ;
                    Cell_delete(droite);
                }
                else
                {
                   if(input->current->next)
                    {input->current=input->current->next;
                    input->pos=0;
                    }
//printf("%c\n",Input_get(input));
                    //printf("%s\n",Input_getEditedWord(input));
                }
            }
            }
            return 0;
}
            
int IMPLEMENT(Input_moveLeft)(Input *input)
{
    //return provided_Input_moveLeft(input);
    //liste vide
    if(input->current==NULL)
    {
        return 1;
    }
    if (input->current->previous==NULL && input->pos==0)
    {
        return 1;
    }
    if(input->pos==0)
    {
        input->current=input->current->previous;
        input->pos=input->current->bucket.top;
    }
    else{
        --input->pos;
    }
    return 0;
}

int IMPLEMENT(Input_moveRight)(Input *input)
{
    //return provided_Input_moveRight(input);
    // si la chaine est vide on renvoit une erreur 
    if(input->current==NULL)
    {
        return 1;
    }
    // si on est dans le dernier cellule et le curseur est à l'extremité
    if (input->current->next==NULL && input->pos>input->current->bucket.top)
    {
        return 1;
    }
    // si on est A la fin d'une cellule  et le curseur est à l'extremité de celle ci on met le curseur au debut d'une nouvelle cellule
    if(input->pos==input->current->bucket.top &&input->current->next!=NULL)
    {
        input->current=input->current->next;
        input->pos=0;
    }
    else{
       ++input->pos;
    }
    return 0;
}

char* IMPLEMENT(Input_toString)(const Input *input)
{
    //return provided_Input_toString(input);
    //qui retourne une nouvelle chaîne de caractères initialisée à partir de input. 
    char *chaineInitDeInput=NULL;
    chaineInitDeInput=malloc(sizeof(char)*(Input_size(input)+1));
    if(!chaineInitDeInput)
    {
        return "erreur d'allocation";
    }
    int i=0;
    InputIterator iterateur;
for(  InputIterator_initIterator(input, &iterateur);
     !InputIterator_isOver(&iterateur); InputIterator_next(&iterateur))
{
    chaineInitDeInput[i]= InputIterator_get(&iterateur);
    ++i;
}
   chaineInitDeInput[i]='\0';
   return chaineInitDeInput;
    
}

void IMPLEMENT(InputIterator_initIterator)(const Input *input, InputIterator *inputIterator)
{
    //provided_InputIterator_initIterator(input, inputIterator);
    *inputIterator=*input;
    if(inputIterator->current)
    {
        while(inputIterator->current->previous)
        {
            inputIterator->current=inputIterator->current->previous;
        }
        inputIterator->pos=0;
    }
}

int IMPLEMENT(InputIterator_equals)(const InputIterator *x, const InputIterator *other)
{
    return provided_InputIterator_equals(x, other);
    return (InputIterator_get(x)==InputIterator_get(other));
}

int IMPLEMENT(InputIterator_isOver)(const InputIterator *inputIterator)
{
    //return provided_InputIterator_isOver(inputIterator);
    return   (inputIterator->current == NULL                                                               )
             ||(inputIterator->current->next==NULL && inputIterator->pos>inputIterator->current->bucket.top);
}

void IMPLEMENT(InputIterator_next)(InputIterator *inputIterator)
{
        //provided_InputIterator_next(inputIterator);
        Input_moveRight(inputIterator);
}

char IMPLEMENT(InputIterator_get)(const InputIterator *inputIterator)
{
    //return provided_InputIterator_get(inputIterator);
     return Input_get(inputIterator);
}

int IMPLEMENT(Input_load)(Input *input, const char *cmd)
{
    //return provided_Input_load(input, cmd);
    /*qui réinitialise input à partir de command (input et command doivent correspondre à la même séquence de caractères après opération). La fonction renvoie une erreur si command est NULL. */
    if(!cmd)
    {
        return 1;
    // il faut penser à liberer l'espace mémoire dans la céllule avant de la réinitialisé pour éviter les fuites mémos
    }Input_finalize(input);
    Input_init(input);
    int i=0;
    while(cmd[i]!='\0')
    {    
     Input_insert(input,cmd[i]);
     ++i;
    }
    Input_insert(input,'\0');
    return 0;
    }
    


char* IMPLEMENT(Input_getEditedWord)(const Input *input)
{
    //return provided_Input_getEditedWord(input);
    char *motEnCours=NULL;
    motEnCours=(char*)malloc(sizeof(input->current->bucket.content)+1);
    // si j'ai une erreur d'allocation
    if(motEnCours==NULL)
    {
        return "Error";
    }
    // si je suis au tout debut de la ligne de commande
    if(input->pos==0 && input->current->previous==NULL)
    {   free(motEnCours);
        return NULL;
    }
    // si il y a une espace au debut de la commande et le curseur se trouve sur 1
    if(input->pos==1 && input->current->bucket.content[0]==' ')
    { free(motEnCours);
        return "EMPTY STRING";
    }
    // et le cas ou je renvoi la chaine
    int i=0;
    for ( i=0;i<input->pos ;++i)
    {
        motEnCours[i]=input->current->bucket.content[i];
    }
    motEnCours[i]='\0';
    return motEnCours;
}
