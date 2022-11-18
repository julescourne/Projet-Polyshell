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

#include "system/command.h"
#include "misc/all.h"
#include "misc/ferror.h"
#include "interactive/all.h"
#include <stdio.h>


// #########################################################################
// #########################################################################
// #########################################################################

MAKE_DEL_0(CmdMember)

CmdMember* IMPLEMENT(CmdMember_new)(const char *base)
{
    // MAKE_NEW is not sufficient here because new variables must
    // be registered using the CmdMember_addLivingCmdMember function.
   // return provided_CmdMember_new(base);
    CmdMember *ncmd= (CmdMember*)malloc(sizeof(CmdMember));
    if(ncmd){
        if(CmdMember_init(ncmd,base)){
            free(ncmd);
            return NULL;
        }
    if(CmdMember_addLivingCmdMember(ncmd)){
        CmdMember_delete(ncmd);
        return NULL;
    }
}return ncmd;
}

int IMPLEMENT(CmdMember_init)(CmdMember *mbr, const char *base)
{
    //return provided_CmdMember_init(mbr, base);
    //status
    mbr->status=1;
    //base
    mbr->base= duplicateString(base);
    if(!mbr->base){
        return 1;
    }//redirection
    for(int fd = 0;fd < 3; ++fd){
        mbr->redirections[fd]=NULL;
        mbr->redirectionTypes[fd] = UNDEFINED;
    }
    //chainage
    mbr->next= NULL;
    mbr->prev= NULL;
    //options
    mbr->options= NULL ;
    mbr->nbOptions=0;
    mbr->capacityOption=0;
    //on respecte toute de suite la convention C option[0] = base
    CmdMember_addOption(mbr,base,1);
    if(!mbr->status){
        CmdMember_finalize(mbr);
        return 1;
    }return 0;
}

void IMPLEMENT(CmdMember_finalize)(CmdMember *mbr)
{
   //provided_CmdMember_finalize(mbr);
   //liberer la base alloer par chaque menbre de cmbmenber
   free(mbr->base);
   int i=(int)mbr->nbOptions-1;
   while(i>=0){
    free(mbr->options[i]);
    --i;
   }
   for(int j=0;j<3;++j){
     free(mbr->redirections[j]);
   }
   free(mbr->options);
   //free(mbr->redirections);
}

CmdMember* IMPLEMENT(CmdMember_redirect)(CmdMember *mbr, int fd, const char *filename)
{   //qui met en place une redirection du flot fd vers le fichier filename (<, > ou 2>)
    //return provided_CmdMember_redirect(mbr, fd, filename);
    if(!mbr || fd < 0 || fd > 2 || !filename){
        //erreur on ne fait rien
        fatalError("command : invalid fd or filename");
    }
    if(mbr->status){
        char *abs=prependHomeDir(duplicateString(filename));
        if(!abs){
                  mbr->status=0;
                  fprintf(stderr,"command prependhomedir or duplicate string failed");
        }else{
            //si deux appels redirect pour un même fd 
            if(mbr->redirections[fd]) free(mbr->redirections[fd]);
            mbr->redirections[fd]=abs;
            mbr->redirectionTypes[fd]=NORMAL;
        }
    }return mbr;
}

CmdMember* IMPLEMENT(CmdMember_appendRedirect)(CmdMember *mbr, int fd, const char *filename)
{
    //return provided_CmdMember_appendRedirect(mbr, fd, filename);
     if(!mbr || fd <= 0 || fd > 2 || !filename){
        //erreur on ne fait rien
        fatalError("command : invalid fd or filename");
    }
    if(mbr->status){
        char *abs=prependHomeDir(duplicateString(filename));
        if(!abs){
                  mbr->status=0;
                  fprintf(stderr,"command prependhomedir or duplicate string failed");
        }else{
            //si deux appels redirect pour un même fd 
            if(mbr->redirections[fd]) free(mbr->redirections[fd]);
            mbr->redirections[fd]=abs;
            mbr->redirectionTypes[fd]=APPEND;
        }
    }return mbr;
}


CmdMember* IMPLEMENT(CmdMember_mergeOutputs)(CmdMember *mbr)
{
    return provided_CmdMember_mergeOutputs(mbr);
}

CmdMember* IMPLEMENT(CmdMember_pipe)(CmdMember *m1, CmdMember *m2)
{
    return provided_CmdMember_pipe(m1, m2);
}

CmdMember* IMPLEMENT(CmdMember_addOption)(CmdMember *mbr, const char *option, int expend)
{
    //return provided_CmdMember_addOption(mbr, option, expend);
    if(!mbr ||  !mbr->status ){
        return mbr;
    }
    //ajout simple  sans prise en charge  des extention de type *pour commencer
    
    if(mbr->nbOptions==0 && !mbr->options)
    {   mbr->capacityOption=1;
    { mbr->options=malloc((mbr->capacityOption)*sizeof(char*));
    if(!mbr->options){
        mbr->status=0;
        printf("erreur d'allocation");
        return NULL;
     }}}
   
    if(expend==0 ){
                   if(isNotEmpty(option)){
                   mbr->options[mbr->nbOptions]=duplicateString(option);
                   ++mbr->nbOptions;
                                        }
                    else{ if(!option){
                             mbr->options[mbr->nbOptions]=NULL;
                             ++mbr->nbOptions;
    }}}
           else { 
              if(isNotEmpty(option)){
               mbr->options[mbr->nbOptions]=duplicateString(option); 
              ++mbr->nbOptions;
                                   }
             
              
              else{ if(!option){
                  mbr->options[mbr->nbOptions]=NULL;  
              ++mbr->nbOptions;
                              }
        }}
        if(mbr->capacityOption==mbr->nbOptions){
            mbr->capacityOption=(mbr->capacityOption*2);
            mbr->options=realloc(mbr->options,(mbr->capacityOption)*sizeof(char*));
        }
    // Capacité du tableau options.
    
    return mbr;
}

size_t IMPLEMENT(Command_getNbMember)(const Command *cmd)
{
    return provided_Command_getNbMember(cmd);
}
#include <unistd.h>
#include <errno.h>
#include <wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
static void __make_redirect__(const char* path,         int flags, int fd){
                     //on ouvre le fichier et on recupère le descripteur de fichier permettant
                     //dde lire ou écrire dans path
                     const int fdFichier= open(path ,flags, a64l);
                    if(fdFichier==-1){
                        fprintf(stderr,"command_execute: cannot open %s\n",path);
                        exit(1);
                    }
                    //on dupplique ce descriptor de fichier et on remplace le descripteur de fichier fd 
                    //par exemple 0=entrée standart par celle copie
                   if( dup2(fdFichier,fd)==-1){
                       fprintf(stderr,"command_execute: dup2 has failed");
                   exit(1);}
                   //on ferme le descriptor de fichier 
                    if (close(fdFichier)){
                        fprintf(stderr,"command_execute: close has failed");
                        exit(1);
                    }
}

 

int IMPLEMENT(Command_execute)(Command *cmd)
{
    return provided_Command_execute(cmd);
    if(!stringCompare(cmd->base,"exit")){
        exit(0);
    }else{if(stringCompare(cmd->base,"cd")){
        fprintf(stderr,"cmd execute : cd :not implemented");
    }
    pid_t pid = fork();
    switch(pid){
        case -1 :
                perror("fork");
                break;
        case 0 :
        //entrée standart
        if(cmd->redirectionTypes[0]==NORMAL){
            __make_redirect__(cmd->redirections[0],O_RDONLY,0);
        }
        //2 sortie standart
                if(cmd->redirectionTypes[1]==NORMAL){
            __make_redirect__(cmd->redirections[1],O_CREAT |O_TRUNC |O_WRONLY,1);
        }
        else{
            if(cmd->redirectionTypes[1]==APPEND){
                 __make_redirect__(cmd->redirections[1],O_CREAT |O_APPEND |O_WRONLY,1);
             }
             if(cmd->redirectionTypes[2]==NORMAL){
                 __make_redirect__(cmd->redirections[1],O_CREAT |O_TRUNC |O_WRONLY,2);
             }  
             else{
                 if(cmd->redirectionTypes[2]==APPEND){
                    __make_redirect__(cmd->redirections[1],O_CREAT |O_APPEND|O_WRONLY,2);
                   }
                  else{
                 if(cmd->redirectionTypes[1]==FUSION){
                     if(dup2(1,2)==-1){
                         fprintf(stderr,"command_execute: dup2 has failed\n");
                         exit(1);
                 }}
              }
             }
        }
                CmdMember_addOption(cmd,NULL,0);
                execvp(cmd->base, cmd->options );
                perror("execvp");
               exit(1);
        default:
                   wait(NULL);
                   break;
    }}
    return 0;
}
