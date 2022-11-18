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

//#include "interactive/autocomplete.h"
//#include "misc/string.h"
//#include "misc/filesystem.h"
//#include "misc/fifo.h"
#include "misc/all.h"
#include "interactive/all.h"
#include "system/info.h"
#include <stdio.h>

// #########################################################################
// #########################################################################
// #########################################################################

char* IMPLEMENT(prependHomeDir)(char *str)
{
    return provided_prependHomeDir(str);
}
/**----------------------------------------------------------------------------------------------------------------------------------*/
/**@brief : je crée un comportorement à partir du squelette fourni qui va à chaque fois fouillé dans un sous dossiers pour remplir results et extend*/
static int parcours_et_remplissage_autocomplete(const char *prefix,unsigned int *nbItems, char **extend, Fifo **results,const char *dossier){
     if(  !prefix
        || !dossier
        || !extend
        || !nbItems
        || !results )
        { 
            return 1;
        }
        FolderIterator it;
        if(FolderIterator_init(&it,dossier,1))
        { printf(" ce dossier n'as pas éte ouverte-> {%s}\n",dossier);
             return 1;
        }
        //parcours du repertoire courrant
        while(!FolderIterator_isOver(&it))
        {   //je doit ajouter le charactère slash a la fin des fichier 
            const char *current= FolderIterator_get(&it);
            const char* suffix = startWith(current,prefix,1);
            if(suffix)
            {
                (*nbItems)++;
                //on garde les limites premiers propositions
                if(!Fifo_full(*results))
                {DIR *tmp=opendir(current);
                    if(tmp!=NULL){
                                    char* slash =concatenateStrings(current,"/",stringLength(current)+2);
                                     Fifo_push(*results,slash);
                                     free(slash);
                                      closedir(tmp);
                                  } 
                   else
                       { Fifo_push(*results,current);}
                }
                //si prefix = doc
                // si current = document
                //alors suffix=    ument
                if(!*extend){
                    //comment extend peux exister 2 fois mais diferent
                    //premiere fois que l'on ai trouver un dossier fichier commençant par prefix
                     DIR *tmp=opendir(current);
                     if(tmp!=NULL){
                                    *extend=concatenateStrings(suffix,"/",stringLength(suffix)+2);
                                      closedir(tmp);
                                  } 
                      else{
                          *extend= duplicateString(suffix);}
                }else
                {
                    mkCommon(*extend,suffix);
                }
            }FolderIterator_next(&it);
        }FolderIterator_finalize(&it);
        return 0;
}

/**----------------------------------------------------------------------------------------------------------------------------------*/
int IMPLEMENT(autocomplete)(const char *prefix, unsigned int limit, unsigned int *nbItems, char **extend, Fifo **results)
{
    //return provided_autocomplete(prefix, limit, nbItems, extend, results);
    if(  !prefix
        || !limit  
        || !extend
        || !nbItems
        || !results )
        { 
            return 1;
        }
        //initialisation
        *nbItems=0;
        *extend=NULL;
        *results=Fifo_new(limit,COMPOSE);
        //recuperation du repertoire courrant
         // cas particulier ou l'on doit chercher dans un sous dossier
         const char *dossier;
         const char * temp=prefix;
        if(belongs('/',prefix)){ size_t n=stringLength(prefix);
           Tokenizer* tokenizer= Tokenizer_new(prefix,"/");
               char *chaine=NULL;
               char* new_dossier;
                while(tokenizer->next){
                       Tokenizer_next(tokenizer);}
                       chaine=Tokenizer_get(tokenizer);
                       new_dossier=subString(prefix,n-(stringLength(chaine)+1));
                Tokenizer_delete(tokenizer);
                dossier=new_dossier;  
               prefix=chaine;
               if(parcours_et_remplissage_autocomplete (prefix,nbItems,extend,results,dossier))
                   {
                       free(new_dossier);free(chaine);return 1;
                       }
                 free(new_dossier);free(chaine);
                 prefix=temp;
          }
          
       else { //prise en charge du doubble étoile
                       if(belongs('*',prefix)){
                                      char* firstNoescaped;
                                      char *str=getRealString(prefix,'*',&firstNoescaped);
                                       if(!firstNoescaped){
                                                          prefix=str                 ;}
                                       
                                        else{ prefix=NULL;
                                                       }
                                                       dossier =getCurrentDirectory(0);
                                       if (parcours_et_remplissage_autocomplete(prefix,nbItems,extend,results,dossier)){free(str);
                                                 return 1;
                                                           }free(str);}
                        //dossier executable avec . ou reperpoitre ~ ou ..;               
                        else
                        {   if(!stringCompare(prefix,".") || !stringCompare(prefix,"..") || !stringCompare(prefix,"~")){
                            *extend= duplicateString("/");
                            ++(*nbItems);
                        }else {
                           dossier =getCurrentDirectory(0);
                          if (parcours_et_remplissage_autocomplete(prefix,nbItems,extend,results,dossier)){
                                                     return 1;
       }
       }} }
       
        //à noter que l'utilisateur peut toutefois tapper une commande que de vouloir naviguer dans un fichier 
        dossier=getPATHVar();
        //ne surtout pas parcourir les dossiers déjà parcouru dans getcurrent directory
        Tokenizer* tokenizer=Tokenizer_new(dossier,"/");
        char* str= duplicateString(dossier);
        size_t sizeStr= stringLength(str);
       // const char *Ne;char *newDossier;
       //parcours de different sous dosier de path et remplissage 
        while(!Tokenizer_isOver(tokenizer)){
                           if(sizeStr- stringLength(tokenizer->next)>0){char *newDossier;
                                  newDossier=subString(str,sizeStr- stringLength(tokenizer->next));
                                     const char *Ne=newDossier;
                           if(stringCompare(Ne,getCurrentDirectory(0))){
                                    if(parcours_et_remplissage_autocomplete(prefix,nbItems,extend,results,Ne))
                                            {
                                               printf("le parcours à echoué");
                                               return 3;
                                            }}
                                            free(newDossier);
                                            }
                                            Tokenizer_next(tokenizer);
                                            }
       
        Tokenizer_delete(tokenizer);
       free(str);
        //init iterator
        // 1 extend==result==NULL
        if(*nbItems==0)
        {
            Fifo_delete(*results);
            free(*extend);
            *extend=NULL;
            *results=NULL;
        }else 
        {//2 result NULL et extend !=0
            if(isNotEmpty(*extend)){
                Fifo_delete(*results);
                *results=NULL;
        //3 exetend NULL et result non nul
            } else 
            {
                free(*extend);
                *extend=NULL;
            }
        }
        
        return 0;
}

