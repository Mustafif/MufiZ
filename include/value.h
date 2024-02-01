/* 
 * File:   value.h
 * Author: Mustafif Khan
 * Brief:  Mufi Data Type Values 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef mufi_value_h
#define mufi_value_h

#include "common.h"
#include <string.h>

typedef struct Obj Obj;
typedef struct ObjString ObjString;

#ifdef NAN_BOXING
//> qnan
//> sign-bit
#define SIGN_BIT ((uint64_t)0x8000000000000000)
//< sign-bit
#define QNAN     ((uint64_t)0x7ffc000000000000)
//< qnan
//> tags

#define TAG_NIL   1 // 01.
#define TAG_FALSE 2 // 10.
#define TAG_TRUE  3 // 11.
//< tags

typedef uint64_t Value;
//> is-number

//> is-bool
#define IS_BOOL(value)      (((value) | 1) == TRUE_VAL)
//< is-bool
//> is-nil
#define IS_NIL(value)       ((value) == NIL_VAL)
//< is-nil
#define IS_NUMBER(value)    (((value) & QNAN) != QNAN)
//< is-number
//> is-obj
#define IS_OBJ(value) \
    (((value) & (QNAN | SIGN_BIT)) == (QNAN | SIGN_BIT))
//< is-obj
//> as-number

//> as-bool
#define AS_BOOL(value)      ((value) == TRUE_VAL)
//< as-bool
#define AS_NUMBER(value)    valueToNum(value)
//< as-number
//> as-obj
#define AS_OBJ(value) \
    ((Obj*)(uintptr_t)((value) & ~(SIGN_BIT | QNAN)))
//< as-obj
//> number-val

//> bool-val
#define BOOL_VAL(b)     ((b) ? TRUE_VAL : FALSE_VAL)
//< bool-val
//> false-true-vals
#define FALSE_VAL       ((Value)(uint64_t)(QNAN | TAG_FALSE))
#define TRUE_VAL        ((Value)(uint64_t)(QNAN | TAG_TRUE))
//< false-true-vals
//> nil-val
#define NIL_VAL         ((Value)(uint64_t)(QNAN | TAG_NIL))
//< nil-val
#define NUMBER_VAL(num) numToValue(num)
//< number-val
//> obj-val
#define OBJ_VAL(obj) \
    (Value)(SIGN_BIT | QNAN | (uint64_t)(uintptr_t)(obj))
//< obj-val
//> value-to-num

static inline double valueToNum(Value value) {
  double num;
  memcpy(&num, &value, sizeof(Value));
  return num;
}
//< value-to-num
//> num-to-value

static inline Value numToValue(double num) {
  Value value;
  memcpy(&value, &num, sizeof(double));
  return value;
}
//< num-to-value

#else

typedef enum{
    VAL_BOOL,
    VAL_NIL,
    VAL_INT,
    VAL_DOUBLE,
    VAL_OBJ,
}ValueType;
typedef struct{
    ValueType type;
    union {
        bool boolean;
        double num_double;
        int num_int;
        Obj* obj;
    } as;
}Value;

#define IS_BOOL(value) ((value).type == VAL_BOOL)
#define IS_NIL(value)  ((value).type == VAL_NIL)
#define IS_INT(value)   ((value).type == VAL_INT)
#define IS_DOUBLE(value)  ((value).type == VAL_DOUBLE)
#define IS_OBJ(value) ((value).type == VAL_OBJ)

#define AS_OBJ(value)  ((value).as.obj)
#define AS_BOOL(value) ((value).as.boolean)
#define AS_INT(value) ((value).as.num_int)
#define AS_DOUBLE(value)  ((value).as.num_double)

#define BOOL_VAL(value) ((Value){VAL_BOOL, {.boolean = value}})
#define NIL_VAL         ((Value){VAL_NIL, {.num_int = 0}})
#define INT_VAL(value) ((Value){VAL_INT, {.num_int = value}})
#define DOUBLE_VAL(value) ((Value){VAL_DOUBLE, {.num_double = value}})
#define OBJ_VAL(object)  ((Value){VAL_OBJ, {.obj = (Obj*)object}})
#endif

typedef struct {
    int capacity;
    int count;
    Value* values;
}ValueArray;

//> Evaluates if two values are equal to each other
bool valuesEqual(Value a, Value b);
//> Creates a new empty value array
void initValueArray(ValueArray* array);
//> Appends to the end of a value array
void writeValueArray(ValueArray* array, Value value);
//> Frees the memory of a value array
void freeValueArray(ValueArray* array);
//> Prints a value
void printValue(Value value);
#endif
