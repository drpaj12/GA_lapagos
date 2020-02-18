/*
Copyright (c) 2019 Peter Andrew Jamieson (jamieson.peter@gmail.com)

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/ 
#include <pthread.h>

#include "types.h"
#include "globals.h"
#include "thread_utils.h"

/*---------------------------------------------------------------------------------------------
 * (function: barrier)
 *-------------------------------------------------------------------------------------------*/
void *barrier(thread_t *thread_struct)
{
    int i;

    pthread_mutex_lock(thread_struct->lock);
    //printf("Thread %d -- waiting for barrier as %d one\n", thread_struct->id, *thread_struct->ndone);

    *thread_struct->ndone = *thread_struct->ndone + 1;

    if (*thread_struct->ndone < configuration.num_threads) 
    {
        pthread_cond_wait(thread_struct->cv, thread_struct->lock);
    } 
    else 
    {
        //printf("Thread %d -- signalling\n", thread_struct->id);
        for (i = 1; i < configuration.num_threads; i++) 
            pthread_cond_signal(thread_struct->cv);
        *thread_struct->ndone = 0;
    }
    pthread_mutex_unlock(thread_struct->lock);

    //printf("Thread %d -- after barrier\n", thread_struct->id);
}


