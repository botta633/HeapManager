#ifndef __UAPI_H
#define __UAPI_H



#include <stdint.h>

void mm_init();

void mm_insitantiate_page_family(char *struct_name, uint32_t struct_size);



#define MM_REG_STRUCT(struct_name) \
    (mm_insitantiate_page_family(#struct_name, sizeof(struct_name)))





#endif




