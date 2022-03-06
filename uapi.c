#include "uapi.h"

typedef struct emp_ {
    char name[32];
    uint32_t emp_id;
} emp_t;


typedef struct student_ {
    char name[32];
    uint32_t rollno;
    uint32_t marks_phys;
    uint32_t marks_maths;
    uint32_t marks_chem;

    struct student_ *next
} student_t;



int main(){
    mm_init();
    MEM_REG_STRUCT(emp_t);
    MEM_REG_STRUCT(student_t);
    return 0;
}


