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

#include "interactive/bucket.h"
#include <assert.h>

// #########################################################################
// #########################################################################
// #########################################################################

// MAKE_NEW_0(Bucket)
// MAKE_DEL_0(Bucket)

Bucket* IMPLEMENT(Bucket_new)(void)
{
    //return provided_Bucket_new();
    Bucket *nbucket=(Bucket*)malloc(sizeof(Bucket));
    if (nbucket!=NULL)
        {
           if (Bucket_init(nbucket))
           {
               free(nbucket);
               nbucket=NULL;
           }
        }
        return nbucket;
}

int IMPLEMENT(Bucket_init)(Bucket *bucket)
{
    //return provided_Bucket_init(bucket);
    bucket->top=-1;
    return 0;
}

void IMPLEMENT(Bucket_delete)(Bucket *bucket)
{
    //provided_Bucket_delete(bucket);
    free(bucket);
    Bucket_finalize(bucket);
    
}

void IMPLEMENT(Bucket_finalize)(Bucket *bucket)
{
    //provided_Bucket_finalize(bucket);
    // ça ne fait rien mais ça évite le warning
    (void)bucket;
}

size_t IMPLEMENT(Bucket_size)(const Bucket *bucket)
{
    //return provided_Bucket_size(bucket);
        return (size_t)bucket->top+1;
}

void IMPLEMENT(Bucket_remove)(Bucket *bucket, int position)
{
    //provided_Bucket_remove(bucket, position);
    assert(position >=    0       );
    assert(position <= bucket->top);
    for (int i=position;i<bucket->top;++i)
    {
      bucket->content[i]=bucket->content[i+1];
    }
    --bucket->top;
}

void IMPLEMENT(Bucket_insert)(Bucket *bucket, int position, char c)
{
    //provided_Bucket_insert(bucket, position, c);
    assert (position>=0                    );
    for (int i=bucket->top+1;i>position;--i)
    {
        bucket->content[i]=bucket->content[i-1];
    }
    bucket->content[position]=c;
    ++bucket->top;
}

void IMPLEMENT(Bucket_move)(Bucket *from, int position, Bucket *to)
{
    //provided_Bucket_move(from, position, to);
    //qui dePlace la pOsition iemE caracere et ceUX qui suivent de from À To
    assert (position>=0                    );
    int j=0;
    int n=from->top+1-position;
    for(int i=position ;i<from->top+1;++i)
        {
            Bucket_insert(to,j,from->content[i]);
            ++j;
        } 
        for(int i=0;i<n;++i)
        {
          Bucket_remove(from,position);  
        }


}

int IMPLEMENT(Bucket_empty)(const Bucket *bucket)
{
    //return provided_Bucket_empty(bucket);
    if (bucket->top!=-1)
    {
        return 0;
    }
    return 1;
}

int IMPLEMENT(Bucket_full)(const Bucket *bucket)
{
    //return provided_Bucket_full(bucket);
    if (Bucket_size(bucket)==sizeof(bucket->content))
    {
        return 1;
    }
    return 0;
}
    

