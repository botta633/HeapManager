#ifndef MM_H
#define MM_H
#include <stdint.h>
#include <assert.h>


#define MM_MAX_STRUCT_NAME 32

typedef struct vm_page_family
{
    char struct_name[MM_MAX_STRUCT_NAME];
    uint32_t struct_size;
} vm_page_family_t;

typedef struct vm_page_for_families
{
    struct vm_page_for_families *next;
    vm_page_family_t vm_page_family[0];
} vm_page_for_families_t;

#define MAX_FAMILIES_PER_PAGE       \
    (SYSTEM_PAGE_SIZE - sizeof(vm_page_for_families_t *) /\
     sizeof(vm_page_family_t))


#define ITERAGE_PAGE_FAMILIES_BEGIN(vm_page_for_families_ptr, curr) \
{ \
    uint32_t count = 0; 					    \
    for(curr = (vm_page_family_t *) & vm_page_for_families_ptr -> vm_page_family[0]; \
	    curr -> struct_size && count < MAX_FAMILIES_PER_PAGE; \
	    curr++, count++) {




#define ITERATE_PAGE_FAMILIES_END(vm_page_for_families_ptr, curr) } }




#endif 



