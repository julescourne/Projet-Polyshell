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

#include "misc/string.h"
#include "misc/ferror.h"
#include <stdio.h>


// #########################################################################
// #########################################################################
// #########################################################################

char IMPLEMENT(toLowerCase)(char c)
{
    //return provided_toLowerCase(c);
    if(c>='A' && c<='Z')
        c=(char)(c-'A'+'a');
    return c;
}

char IMPLEMENT(toUpperCase)(char c)
{
    //return provided_toUpperCase(c);
    if(c>='a' && c<='z')
        c=(char)(c+'A'-'a');
    return c;
}

size_t IMPLEMENT(stringLength)(const char *str)
{
    //return provided_stringLength(str);
    size_t i =0;
    while (*str !='\0')
    {
        ++str;
        ++i;
    }
    return i;
}

char* IMPLEMENT(duplicateString)(const char *str)
{
    //return provided_duplicateString(str);
    size_t size=stringLength(str)+1;
    char *newstr=malloc(size);
    if (newstr!=NULL)
    {
        copyStringWithLength(newstr,str,size);
    }
    
    return newstr;
}

const char* IMPLEMENT(findFirst)(const char *str, const char *separators)
{
    //return provided_findFirst(str, separators);
    int i =0;
    while(str[i]!='\0')
    {
        int j=0;
        while(separators[j]!='\0')
               {
                   if (str[i]==separators[j])
                       return &str[i];
               ++j;
               }
        
        ++i;
    }
    return NULL;
}

char* IMPLEMENT(findLast)(char *str, char c)
{
    //return provided_findLast(str, c);
    int i=stringLength(str);
    while (i>=0)
    {
        if(str[i]==c)
        {
            return &str[i];
        }
        i--;
    }
    return NULL ;
}

int IMPLEMENT(stringCompare)(const char *str1, const char *str2)
{
    //return provided_stringCompare(str1, str2);
    size_t i=0;
    while (str1[i]!=0&& str2[i]==str1[i])
        ++i;
    return str1[i]-str2[i];
}

const char* IMPLEMENT(indexOfString)(const char *foin, const char *aiguille, int csensitive)
{
     //return provided_indexOfString(foin, aiguille, csensitive);
    size_t i =0;
    while(foin[i]!='\0')
    {
        size_t  j=0;
        while((aiguille[j]!='\0'&&(csensitive !=0 )&& (foin[i]==aiguille[j]))||((aiguille[j]!='\0' && csensitive==0)&&(toUpperCase(foin[i])==toUpperCase(aiguille[j]))))
        {
            ++i;
            ++j;
        }
        if (stringLength(aiguille)==j)
        {
            return &foin[i-j];
        }
        ++i;
    }
    return NULL;
}

char* IMPLEMENT(concatenateStrings)(const char *str1, const char *str2, size_t minDestSize)
{
    //return provided_concatenateStrings(str1, str2, minDestSize);
    if(minDestSize<(stringLength(str1)+stringLength(str2)+1))
    {
        minDestSize=(stringLength(str1)+stringLength(str2)+1);
    }
    char *newconcaStr=(char*)malloc(minDestSize);
    if (newconcaStr!=NULL)
          {
              copyStringWithLength(newconcaStr,str1,stringLength(str1)+1);
           copyStringWithLength(&newconcaStr[stringLength(str1)],str2,stringLength(str2)+1);
           }
    return newconcaStr;
}

void IMPLEMENT(copyStringWithLength)(char *dest, const char * src, size_t destSize)
{
    //provided_copyStringWithLength(dest, src, destSize);
    if (destSize==0)
    {
        fatalError("destsize==0");
    }
    size_t i;
    for(i=0;src[i] !='\0' && i<destSize-1;++i)
    {
        dest[i]=src[i];
    }
    dest[i]='\0';
    
}

char* IMPLEMENT(mkReverse)(char *str)
{
    //return provided_mkReverse(str);
    size_t size=stringLength(str);
    char *temp=malloc((size+1)*sizeof(char));// crÉation d une chaine provisoire pour stoker str
    if(temp!=NULL)
    {
      size_t i;
      for(i=0;i<size;++i)
      {
          temp[i]=str[size-i-1];
      }
      temp[i]='\0';
      for(i=0;i<size;++i)
      {
          str[i]=temp[i];
      }
      free(temp);
    }
    return str;
}

const char* IMPLEMENT(startWith)(const char *str, const char *prefix, int csensitive)
{
    //return provided_startWith(str, prefix, csensitive);
    while(*prefix)
    {
        if ((csensitive !=0 && *prefix != *str)||(csensitive==0 && toLowerCase(*prefix)!=toLowerCase(*str)))
        {
            return NULL;
        }
        ++str;
        ++prefix;
    }
    return str;
}

int IMPLEMENT(belongs)(char c, const char *str)
{
    //return provided_belongs(c, str);
    while(*str)
    {
        if(*str==c)
        {
            return 1;
        }
        ++str;
    }
    return 0 ;
}

char* IMPLEMENT(subString)(const char *start, size_t length)
{
    //return provided_subString(start, length);
    char *newStart= malloc(length+1);
    if (newStart!=NULL)
    {
        size_t i;
        for( i=0;i<length && start[i]!=0;++i)
        {
            newStart[i]=start[i];
        }
        newStart[i]='\0';
    }
    return newStart;
}

void IMPLEMENT(mkCommon)(char *result, const char *str)
{
    //provided_mkCommon(result, str);
    int i =0;
    while(result[i]!='\0' && result[i]==str[i])
    {
        ++i;
    }
    result[i]='\0';
}

int IMPLEMENT(isNotEmpty)(const char *str)
{
    //return provided_isNotEmpty(str);
    while (*str)
    {
        if (*str==' ')
        {
            ++str;
        }
        else 
        {
            return 1;
        }

    }
    return 0;
}

char* IMPLEMENT(getProtString)(const char *str, char c)
{
    //return provided_getProtString(str, c);
    char* newstr=malloc((2*stringLength(str)+1));
    if (newstr!=NULL)
    {
        size_t i=0,j=0;
        while(str[i]!='\0')
        {
           newstr[j]=str[i];
           if(str[i]==c)
           {
               ++j;
               newstr[j]=str[i];
           }
           ++i;
           ++j;
        }
         newstr[j]='\0';
    }
    return newstr;
}

char* IMPLEMENT(getRealString)(const char *str, char c, char **firstNotEscaped)
{
    //return provided_getRealString(str, c, firstNotEscaped);
    if(!str                                               
    || !firstNotEscaped  )
            {
                return NULL;
            }
    size_t i=0;
    size_t j=0;
    *firstNotEscaped=NULL;
    char* newStr=malloc(stringLength(str)+2);
    if(newStr!=NULL)
    {
        for(i=0;str[i];i++){
            newStr[j]=str[i];
            if(str[i]==c&&str[i+1]!=c && !(*firstNotEscaped))
             {
                *firstNotEscaped=&newStr[j];
                }
           if(str[i]==c && str[i+1]==c){
                ++i;
            }
           ++j;
        }
        
        newStr[j]='\0';
    }
    return newStr;
}

MAKE_NEW_2(Tokenizer, const char*, const char*)
MAKE_DEL_0(Tokenizer)

int IMPLEMENT(Tokenizer_init)(Tokenizer *tokenizer, const char *str, const char *separators)
{
    //return provided_Tokenizer_init(tokenizer, str, separators);
    if(   !str 
          || !separators
          || !tokenizer) 
          {
              return 1;
          }
          // position dans la chaine de depart du prochain mots à retourner
          if(!isNotEmpty(str)){
              tokenizer->str=NULL;
          }
          else{tokenizer->str=str;
           //la liste des separators selons la quelles on decoupe str
          tokenizer->separators =separators;
          // position dans la chaine de depart du prochain separators
    tokenizer->next=findFirst(tokenizer->str,tokenizer->separators); 
    if(tokenizer->next==tokenizer->str)
    {
        Tokenizer_next(tokenizer);
          }}
    return 0 ;
}

void IMPLEMENT(Tokenizer_finalize)(Tokenizer *tokenizer)
{
  //provided_Tokenizer_finalize(tokenizer);
   tokenizer->separators=NULL;
   tokenizer->next=NULL;
   tokenizer->str=NULL;
}

int IMPLEMENT(Tokenizer_isOver)(const Tokenizer *tokenizer)
{
    //return provided_Tokenizer_isOver(tokenizer);
    return tokenizer->str==NULL;
}

char* IMPLEMENT(Tokenizer_get)(const Tokenizer *tokenizer)
{
    //return provided_Tokenizer_get(tokenizer);
    size_t size;
    if(tokenizer->next!=NULL)
    { size=stringLength(tokenizer->str)-stringLength(tokenizer->next);}
    else{
        size=stringLength(tokenizer->str);
    }
    return subString(tokenizer->str,size);
    
}

void IMPLEMENT(Tokenizer_next)(Tokenizer *tokenizer)
{
//provided_Tokenizer_next(tokenizer);
int i=0;

if(tokenizer->next && tokenizer->str){
                if(tokenizer->str[i]!=tokenizer->next[i]&& tokenizer->next[1]!='\0')
                   {   if(tokenizer->next[1+1]!='\0')
                       {tokenizer->str=&tokenizer->next[i+1];
                        tokenizer->next=findFirst(tokenizer->str,tokenizer->separators);
                        }else{
                        tokenizer->str=NULL;
                        tokenizer->next=NULL;
                        }
}else{ if(tokenizer->next[1]!='\0'){
                if(tokenizer->next[1+1]!='\0'){
                           tokenizer->str=&tokenizer->next[i+1];
                           tokenizer->next=findFirst(tokenizer->str,tokenizer->separators);
                           if(tokenizer->str==tokenizer->next)
                                                    {
                                                     Tokenizer_next(tokenizer);
}}
               else{
                    tokenizer->str=NULL;
                    tokenizer->next=NULL;
}
}else{
                        tokenizer->str=NULL;
                    tokenizer->next=NULL;
}
}}else{
    tokenizer->str=tokenizer->next;}}
    


