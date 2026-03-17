#ifndef THREADED_H

#define THREADED_H

#include "camera.h"
#include "hittable_list.h"
typedef struct {
    int start;
    int end;
    int width;
    unsigned char *output_buffer;
} thread_data;

void *render_thread(thread_data *thread_i, camera *cam, hittable_list *world) {
    thread_data *local_thread =  thread_i;

}


#endif