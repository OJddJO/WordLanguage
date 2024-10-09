#ifndef __TYPE_H__
#define __TYPE_H__

typedef enum _w_type {
    NULL_TYPE,
    INT,
    FLOAT, //double is the same as float
    STRING, //char * is the same as string
    LIST
} W_Type;

#endif