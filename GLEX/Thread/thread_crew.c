//
//  thread_crew.c
//  GLEX
//
//  Created by 李佳 on 15-5-20.
//  Copyright (c) 2015年 李佳. All rights reserved.
//

#include "thread_crew.h"

void *work_routine(void* arg)
{
    worker_t* mine = (worker_t*)arg;
    crew_t* crew = mine->crew;
    work_t* work,*new_work;
    struct stat filestat;
    int status;
    
    struct dirent *entry = (struct dirent*)malloc(sizeof(struct dirent) + name_max);
    if (entry == NULL)
        errno_abort("Allocating dirent");
    status = pthread_mutex_lock(&crew->mutex);
    if (status != 0)
        err_abort(status, "Lock crew mutex");
    
    while (crew->work_count == 0) {
        status = pthread_cond_wait(&crew->go, &crew->mutex);
        if (status != 0)
            err_abort(status, "Unlock mutex");
    }
    
    status = pthread_mutex_unlock(&crew->mutex);
    if (status != 0)
        err_abort(status, "Unlock mutex");
    
    DPRINTF(("Crew %d starting\n",mine->index));
    
    while (1) {
        status = pthread_mutex_lock(&crew->mutex);
        if (status != 0)
            err_abort(status, "Lock crew mutex");
        
        DPRINTF(("Crew %d top : first is %#lx, count is %d\n",mine->index,crew->first,crew->work_count));
        while (crew->first == NULL) {
            status = pthread_cond_wait(&crew->go, &crew->mutex);
            if (status != 0)
                err_abort(status, "Wait for work");
        }
        
        DPRINTF(("Crew %d woke : %#lx, %d\n",mine->index,crew->first,crew->work_count));
        
        /*
         *  Remove and process a work item
         **/
        work = crew->first;
        crew->first = work->next;
        if (crew->first == NULL)
            crew->last = NULL;
        DPRINTF(("Crew %d took %#lx,leaves first %#lx, last %#lx\n",mine->index,work,crew->first,crew->last));
        
        status = pthread_mutex_unlock(&crew->mutex);
        if (status != 0)
            err_abort(status, "Unlock mutex");
        
        /*
         *  We have a work item. Process it, Which may involve
         *  queuing new work items
         */
        status = lstat(work->path, &filestat);
        
        if (S_ISLNK(filestat.st_mode))
            printf("Thread %d : %s is a link, skipping. \n",mine->index,work->path);
        else if (S_ISDIR(filestat.st_mode)) {
            DIR *dir;
            dir = opendir(work->path);
            if (dir == NULL) {
                fprintf(stderr, "Unable to open directory %s: %d (%s)\n",work->path,errno,strerror(errno));
                continue;
            }
            
            struct dirent *result;
            while (1) {
                status = readdir_r(dir, entry, &result);
                if (status != 0) {
                    fprintf(stderr, "Unable to read directory %s: %d(%s)\n",work->path,status,strerror(status));
                }
                
                if (result == NULL) break;
                
                /*
                 *  Ignore "." and ".." entries.
                 */
                
                if (strcmp(entry->d_name, ".") == 0)
                    continue;
                new_work = (work_p)malloc(sizeof(work_t));
                if (new_work == NULL)
                    errno_abort("Unable to allocate space");
                new_work->path = (char*)malloc(sizeof(path_max));
                if (new_work->path == NULL)
                    errno_abort("Unable to allocate path");
                strcpy(new_work->path, work->path);
                strcat(new_work->path, "/");
                strcat(new_work->path, entry->d_name);
                new_work->search_str = work->search_str;
                new_work->next = NULL;
                
                status = pthread_mutex_lock(&crew->mutex);
                if (status != 0)
                    err_abort(status, "Lock mutex");
                if (crew->first == NULL) {
                    crew->first = new_work;
                    crew->last = new_work;
                } else {
                    crew->last->next = new_work;
                    crew->last = new_work;
                }
                crew->work_count++;
                DPRINTF(("Crew %d: add %#lx,first %#lx, last %#lx, %d\n",mine->index,new_work,crew->first,crew->last,crew->work_count));
                status = pthread_cond_signal(&crew->go);
                status = pthread_mutex_unlock(&crew->mutex);
                if (status != 0)
                    err_abort(status, "Unlock mutex");
            }
            closedir(dir);
        } else if (S_ISREG(filestat.st_mode)) {
            FILE *search;
            char buffer[256],*bufptr,*search_ptr;
            
            /*
             *  If this is a file, not a directory, then search it for the string.
             */
            search = fopen(work->path,"r");
            if (search == NULL)
                fprintf(stderr, "Unable to open %s: %d (%s)\n",work->path,errno,strerror(errno));
            else {
                while (1) {
                    bufptr = fgets(buffer, sizeof(buffer), search);
                    if (bufptr == NULL) {
                        if (feof(search)) break;
                        if (ferror(search)) {
                            fprintf(stderr, "Unable to read %s: %d (%s)\n",work->path,errno,strerror(errno));
                            break;
                        }
                    }
                    search_ptr = strstr(buffer, work->search_str);
                    if (search_ptr != NULL) {
                        printf("Thread %d found \"%s\" in %s\n",mine->index,work->search_str,work->path);
                        break;
                    }
                }
                fclose(search);
            }
            
        } else
            fprintf(stderr, "Thread %d: %s is type %o (%s))\n",
                    mine->index,
                    work->path,
                    filestat.st_mode & S_IFMT,
                    (S_ISFIFO(filestat.st_mode)?"FIFO"
                     :(S_ISCHR(filestat.st_mode)?"CHR"
                       :(S_ISBLK(filestat.st_mode)?"BLK"
                         :(S_ISSOCK(filestat.st_mode)?"SOCK"
                           :"unknown")))));
        free(work->path);
        free(work);
        
        status = pthread_mutex_lock(&crew->mutex);
        if (status != 0)
            err_abort(status, "Lock crew mutex");
        
        crew->work_count--;
        DPRINTF(("Crew %d decremented work to %d\n",mine->index,crew->work_count));
        
        if (crew->work_count <= 0) {
            DPRINTF(("Crew thread %d done\n",mine->index));
            status = pthread_cond_broadcast(&crew->done);
            if (status != 0)
                err_abort(status, "Wake waiters");
            status = pthread_mutex_unlock(&crew->mutex);
            if (status != 0)
                err_abort(status, "Unlock mutex");
            break;
        }
        
        status = pthread_mutex_unlock(&crew->mutex);
        if (status != 0)
            err_abort(status, "Unlock mutex");
    }
    
    free(entry);
    
    return NULL;
}




