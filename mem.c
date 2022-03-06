#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <memory.h>
#include "mm.h"

static vm_page_for_families_t *first_vm_page_for_families = NULL;

static size_t SYSTEM_PAGE_SIZE = 0;

void mm_init()
{
    SYSTEM_PAGE_SIZE = getpagesize();
}

static void *mm_get_pages_from_OS(int units)
{
    errno_t error;
    char *vm_page = mmap(
        0,
        units * SYSTEM_PAGE_SIZE,
        PROT_READ | PROT_WRITE,
        MAP_ANON | MAP_PRIVATE,
        0,
        0);

    if (vm_page == MAP_FAILED)
    {
        return NULL;
    }

    memset(vm_page, 0, units * SYSTEM_PAGE_SIZE);

    return (void *)vm_page;
}

void mm_instantiate_new_page_family(char *struct_name, uint32_t struct_size)
{
    vm_page_family_t *vm_page_family_curr = NULL;
    vm_page_for_families_t *new_vm_page_for_families = NULL;

    if(struct_size > SYSTEM_PAGE_SIZE)
    {
	printf("Error: %s() Structure %s size is too big \n", __FUNCTION__, struct_name);
	return;
    }

    if(!first_vm_page_for_families)
    {
	first_vm_page_for_families = 
	    (vm_page_for_families_t *) mm_get_pages_from_OS(1);

	first_vm_page_for_families->next = NULL;
	strncpy(first_vm_page_for_families->vm_page_family[0].struct_name, struct_name, MM_MAX_STRUCT_NAME);
	first_vm_page_for_families->vm_page_family[0].struct_size = struct_size;
	return;
    }

    uint32_t count = 0;

    ITERAGE_PAGE_FAMILIES_BEGIN(first_vm_page_for_families, vm_page_family_curr){
	if(strncmp(vm_page_family_curr->struct_name, struct_name, MM_MAX_STRUCT_NAME) != 0){
	    count++;
	    continue;
	}

	assert(0);  //if by mistake the same page familiy is registered twice

    } ITERATE_PAGE_FAMILIES_END(first_vm_page_for_families, vm_page_family_curr);

    // if the current page is full we request a new one and make it the current one and point next to the previous one
    if( count == MAX_FAMILIES_PER_PAGE )
    {
	new_vm_page_for_families = (vm_page_for_families_t *)mm_get_pages_from_OS(1);
	new_vm_page_for_families->next = first_vm_page_for_families;
	first_vm_page_for_families = new_vm_page_for_families;
	vm_page_family_curr = &first_vm_page_for_families->vm_page_family[0];
    }


    strncpy(vm_page_family_curr->struct_name, struct_name, MM_MAX_STRUCT_NAME);
    
    vm_page_family_curr->struct_size = struct_size;

}

static void mm_return_pages_to_OS(void *vm_page, int units)
{
    if (munmap(vm_page, units * SYSTEM_PAGE_SIZE) != 0)
    {
        printf("Error couldn't munmap memory to kernel\n");
    }
}

int main()
{
    mm_init();
    printf("%d\n", SYSTEM_PAGE_SIZE);
    void *page = mm_get_pages_from_OS(1);
    *((int *)page) = 125;

    printf("%d", *((int *)page));

    return 0;
}

