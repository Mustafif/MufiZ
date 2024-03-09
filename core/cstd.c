#include "../include/cstd.h"
#include <math.h>

Value array_nf(int argCount, Value *args)
{
    if (argCount == 0)
    {
        ObjArray *a = newArray();
        return OBJ_VAL(a);
    }
    else if (argCount == 1 && IS_FVECTOR(args[0]))
    {
        FloatVector *f = AS_FVECTOR(args[0]);
        ObjArray *a = newArrayWithCap(f->size, true);
        for (int i = 0; i < f->count; i++)
        {
            pushArray(a, DOUBLE_VAL(f->data[i]));
        }
        return OBJ_VAL(a);
    }
    else if (argCount >= 1)
    {
        if (!IS_INT(args[0]))
        {
            runtimeError("First argument must be an integer.");
            return NIL_VAL;
        }

        if (argCount == 2 && !IS_BOOL(args[1]))
        {
            runtimeError("Second argument must be a bool");
            return NIL_VAL;
        }

        ObjArray *a = newArrayWithCap(AS_INT(args[0]), AS_BOOL(args[1]));
        return OBJ_VAL(a);
    }
    else
    {
        runtimeError("array() takes 0 or 1 argument.");
        return NIL_VAL;
    }
}

Value linkedlist_nf(int argCount, Value *args)
{
    ObjLinkedList *l = newLinkedList();
    return OBJ_VAL(l);
}

Value hashtable_nf(int argCount, Value *args)
{
    ObjHashTable *h = newHashTable();
    return OBJ_VAL(h);
}

Value put_nf(int argCount, Value *args)
{
    if (!IS_HASH_TABLE(args[0]))
    {
        runtimeError("First argument must be a hash table.");
        return NIL_VAL;
    }
    if (!IS_STRING(args[1]))
    {
        runtimeError("Second argument must be a string.");
        return NIL_VAL;
    }
    ObjHashTable *h = AS_HASH_TABLE(args[0]);
    ObjString *key = AS_STRING(args[1]);
    return BOOL_VAL(putHashTable(h, key, args[2]));
}

Value get_nf(int argCount, Value *args)
{
    if (!IS_HASH_TABLE(args[0]))
    {
        runtimeError("First argument must be a hash table.");
        return NIL_VAL;
    }
    if (!IS_STRING(args[1]))
    {
        runtimeError("Second argument must be a string.");
        return NIL_VAL;
    }
    ObjHashTable *h = AS_HASH_TABLE(args[0]);
    ObjString *key = AS_STRING(args[1]);
    return getHashTable(h, key);
}

Value remove_nf(int argCount, Value *args)
{
    if (!IS_HASH_TABLE(args[0]))
    {
        runtimeError("First argument must be a hash table.");
        return NIL_VAL;
    }
    if (!IS_STRING(args[1]))
    {
        runtimeError("Second argument must be a string.");
        return NIL_VAL;
    }
    ObjHashTable *h = AS_HASH_TABLE(args[0]);
    ObjString *key = AS_STRING(args[1]);
    return BOOL_VAL(removeHashTable(h, key));
}

Value push_nf(int argCount, Value *args)
{
    if (!IS_ARRAY(args[0]) && !IS_LINKED_LIST(args[0]) && !IS_FVECTOR(args[0]))
    {
        runtimeError("First argument must be an array, linked list or vector.");
        return NIL_VAL;
    }

    if (IS_ARRAY(args[0]))
    {

        ObjArray *a = AS_ARRAY(args[0]);
        for (int i = 1; i < argCount; i++)
        {
            pushArray(a, args[i]);
        }
        return NIL_VAL;
    }
    else if (IS_FVECTOR(args[0]))
    {
        FloatVector *f = AS_FVECTOR(args[0]);
        for (int i = 1; i < argCount; i++)
        {
            if (!IS_DOUBLE(args[i]))
            {
                runtimeError("All elements of the vector must be doubles.");
                return NIL_VAL;
            }
            pushFloatVector(f, AS_DOUBLE(args[i]));
        }
        return NIL_VAL;
    }
    else
    {
        ObjLinkedList *l = AS_LINKED_LIST(args[0]);
        for (int i = 1; i < argCount; i++)
        {
            pushBack(l, args[i]);
        }
        return NIL_VAL;
    }
}

Value push_front_nf(int argCount, Value *args)
{
    if (!IS_LINKED_LIST(args[0]))
    {
        runtimeError("First argument must be a linked list.");
        return NIL_VAL;
    }
    ObjLinkedList *l = AS_LINKED_LIST(args[0]);
    for (int i = 1; i < argCount; i++)
    {
        pushFront(l, args[i]);
    }
    return NIL_VAL;
}

Value pop_nf(int argCount, Value *args)
{
    if (!IS_ARRAY(args[0]) && !IS_LINKED_LIST(args[0]))
    {
        runtimeError("First argument must be an array or linked list.");
        return NIL_VAL;
    }

    if (IS_ARRAY(args[0]))
    {

        ObjArray *a = AS_ARRAY(args[0]);
        return popArray(a);
    }
    else
    {
        ObjLinkedList *l = AS_LINKED_LIST(args[0]);
        return popBack(l);
    }
}

Value pop_front_nf(int argCount, Value *args)
{
    if (!IS_LINKED_LIST(args[0]))
    {
        runtimeError("First argument must be a linked list.");
        return NIL_VAL;
    }
    ObjLinkedList *l = AS_LINKED_LIST(args[0]);
    return popFront(l);
}

Value nth_nf(int argCount, Value *args)
{
    if (!IS_ARRAY(args[0]) && !IS_LINKED_LIST(args[0]) && !IS_MATRIX(args[0]) && !IS_FVECTOR(args[0]))
    {
        runtimeError("First argument must be an array, matrix, linked list or Vector.");
        return NIL_VAL;
    }
    if (!IS_INT(args[1]))
    {
        runtimeError("Second argument must be an integer.");
        return NIL_VAL;
    }

    if (IS_MATRIX(args[0]) && argCount == 3)
    {
        if (!IS_INT(args[2]))
        {
            runtimeError("Third argument must be an integer.");
            return NIL_VAL;
        }

        ObjMatrix *m = AS_MATRIX(args[0]);
        // improve error handling here
        int row = AS_INT(args[1]);
        int col = AS_INT(args[2]);
        return getMatrix(m, row, col);
    }
    else if (IS_FVECTOR(args[0]))
    {
        FloatVector *f = AS_FVECTOR(args[0]);
        int index = AS_INT(args[1]);
        double value = getFloatVector(f, index);
        return DOUBLE_VAL(value);
    }
    else if (IS_ARRAY(args[0]))
    {
        ObjArray *a = AS_ARRAY(args[0]);
        int index = AS_INT(args[1]);
        if (index < 0 || index >= a->count)
        {
            runtimeError("Index out of bounds.");
            return NIL_VAL;
        }
        return a->values[index];
    }
    else
    {
        ObjLinkedList *l = AS_LINKED_LIST(args[0]);
        int index = AS_INT(args[1]);
        if (index < 0 || index >= l->count)
        {
            runtimeError("Index out of bounds.");
            return NIL_VAL;
        }
        struct Node *node = l->head;
        for (int i = 0; i < index; i++)
        {
            node = node->next;
        }

        return node->data;
    }
}

Value is_empty_nf(int argCount, Value *args)
{
    if (!IS_ARRAY(args[0]) && !IS_LINKED_LIST(args[0]) && !IS_HASH_TABLE(args[0]))
    {
        runtimeError("First argument must be an array, linked list, hash table or vector.");
        return NIL_VAL;
    }

    if (IS_ARRAY(args[0]))
    {
        ObjArray *a = AS_ARRAY(args[0]);
        return BOOL_VAL(a->count == 0);
    }
    else if (IS_HASH_TABLE(args[0]))
    {
        ObjHashTable *h = AS_HASH_TABLE(args[0]);
        return BOOL_VAL(h->table.count == 0);
    }
    else if (IS_FVECTOR(args[0]))
    {
        FloatVector *f = AS_FVECTOR(args[0]);
        return BOOL_VAL(f->count == 0);
    }
    else
    {
        ObjLinkedList *l = AS_LINKED_LIST(args[0]);
        return BOOL_VAL(l->count == 0);
    }
}

Value sort_nf(int argCount, Value *args)
{
    if (!IS_ARRAY(args[0]) && !IS_LINKED_LIST(args[0]))
    {
        runtimeError("First argument must be an array or linked list.");
        return NIL_VAL;
    }

    if (IS_ARRAY(args[0]))
    {

        ObjArray *a = AS_ARRAY(args[0]);
        sortArray(a);
        return NIL_VAL;
    }
    else
    {
        ObjLinkedList *l = AS_LINKED_LIST(args[0]);
        mergeSort(l);
        return NIL_VAL;
    }
}

Value equal_list_nf(int argCount, Value *args)
{
    if (!IS_ARRAY(args[0]) && !IS_LINKED_LIST(args[0]))
    {
        runtimeError("First argument must be an array, linked list or vector.");
        return NIL_VAL;
    }

    if (IS_ARRAY(args[0]))
    {
        if (!IS_ARRAY(args[1]))
        {
            runtimeError("Second argument must be an array.");
            return NIL_VAL;
        }
        ObjArray *a = AS_ARRAY(args[0]);
        ObjArray *b = AS_ARRAY(args[1]);
        return BOOL_VAL(equalArray(a, b));
    }
    else if (IS_FVECTOR(args[0]))
    {
        if (!IS_FVECTOR(args[1]))
        {
            runtimeError("Second argument must be a vector.");
            return NIL_VAL;
        }
        FloatVector *a = AS_FVECTOR(args[0]);
        FloatVector *b = AS_FVECTOR(args[1]);
        return BOOL_VAL(equalFloatVector(a, b));
    }
    else
    {
        if (!IS_LINKED_LIST(args[1]))
        {
            runtimeError("Second argument must be a linked list.");
            return NIL_VAL;
        }
        ObjLinkedList *a = AS_LINKED_LIST(args[0]);
        ObjLinkedList *b = AS_LINKED_LIST(args[1]);
        return BOOL_VAL(equalLinkedList(a, b));
    }
}

Value contains_nf(int argCount, Value *args)
{
    if (!IS_ARRAY(args[0]) && !IS_LINKED_LIST(args[0]) && !IS_HASH_TABLE(args[0]))
    {
        runtimeError("First argument must be an array, linked list or hash table.");
        return NIL_VAL;
    }

    if (IS_ARRAY(args[0]))
    {
        ObjArray *a = AS_ARRAY(args[0]);
        for (int i = 0; i < a->count; i++)
        {
            if (valuesEqual(a->values[i], args[1]))
            {
                return BOOL_VAL(true);
            }
        }
        return BOOL_VAL(false);
    }
    else if (IS_HASH_TABLE(args[0]))
    {
        ObjHashTable *h = AS_HASH_TABLE(args[0]);
        if (!valuesEqual(getHashTable(h, AS_STRING(args[1])), NIL_VAL))
        {
            return BOOL_VAL(true);
        }
        else
        {
            return BOOL_VAL(false);
        }
    }
    else
    {
        ObjLinkedList *l = AS_LINKED_LIST(args[0]);
        struct Node *current = l->head;
        while (current != NULL)
        {
            if (valuesEqual(current->data, args[1]))
            {
                return BOOL_VAL(true);
            }
            current = current->next;
        }
        return BOOL_VAL(false);
    }
}

Value insert_nf(int argCount, Value *args)
{
    if (argCount != 3)
    {
        runtimeError("insert() takes 3 arguments.");
        return NIL_VAL;
    }
    if (!IS_ARRAY(args[0]) && !IS_FVECTOR(args[0]))
    {
        runtimeError("First argument must be an array or vector.");
        return NIL_VAL;
    }
    if (!IS_INT(args[1]))
    {
        runtimeError("Second argument must be an integer.");
        return NIL_VAL;
    }
    if (IS_FVECTOR(args[0]))
    {
        FloatVector *f = AS_FVECTOR(args[0]);
        int index = AS_INT(args[1]);
        if (!IS_DOUBLE(args[2]))
        {
            runtimeError("Third argument must be a double.");
            return NIL_VAL;
        }
        insertFloatVector(f, index, AS_DOUBLE(args[2]));
        return NIL_VAL;
    }
    else
    {
        ObjArray *a = AS_ARRAY(args[0]);
        int index = AS_INT(args[1]);
        insertArray(a, index, args[2]);
        return NIL_VAL;
    }
}

Value len_nf(int argCount, Value *args)
{
    if (!IS_ARRAY(args[0]) && !IS_LINKED_LIST(args[0]) && !IS_HASH_TABLE(args[0]) && !IS_FVECTOR(args[0]))
    {
        runtimeError("First argument must be an array, vector, linked list or hash table.");
        return NIL_VAL;
    }

    if (IS_ARRAY(args[0]))
    {
        ObjArray *a = AS_ARRAY(args[0]);
        return INT_VAL(a->count);
    }
    else if (IS_HASH_TABLE(args[0]))
    {
        ObjHashTable *h = AS_HASH_TABLE(args[0]);
        return INT_VAL(h->table.count);
    }
    else if (IS_FVECTOR(args[0]))
    {
        FloatVector *f = AS_FVECTOR(args[0]);
        return INT_VAL(f->count);
    }
    else
    {
        ObjLinkedList *l = AS_LINKED_LIST(args[0]);
        return INT_VAL(l->count);
    }
}

Value range_nf(int argCount, Value *args)
{
    if (!IS_INT(args[0]) || !IS_INT(args[1]))
    {
        runtimeError("Both arguments must be integers.");
        return NIL_VAL;
    }
    int start = AS_INT(args[0]);
    int end = AS_INT(args[1]);
    ObjArray *a = newArray();
    for (int i = start; i < end; i++)
    {
        pushArray(a, INT_VAL(i));
    }
    return OBJ_VAL(a);
}

Value reverse_nf(int argCount, Value *args)
{
    if (!IS_ARRAY(args[0]) && !IS_LINKED_LIST(args[0]))
    {
        runtimeError("First argument must be an array or linked list.");
        return NIL_VAL;
    }

    if (IS_ARRAY(args[0]))
    {
        ObjArray *a = AS_ARRAY(args[0]);
        reverseArray(a);
        return NIL_VAL;
    }
    else
    {
        ObjLinkedList *l = AS_LINKED_LIST(args[0]);
        reverseLinkedList(l);
        return NIL_VAL;
    }
}

Value search_nf(int argCount, Value *args)
{
    if (!IS_ARRAY(args[0]) && !IS_LINKED_LIST(args[0]))
    {
        runtimeError("First argument must be an array or linked list.");
        return NIL_VAL;
    }

    if (IS_ARRAY(args[0]))
    {
        ObjArray *a = AS_ARRAY(args[0]);
        int result = searchArray(a, args[1]);
        if (result == -1)
            return NIL_VAL;
        return INT_VAL(result);
    }
    else
    {
        ObjLinkedList *l = AS_LINKED_LIST(args[0]);
        int result = searchLinkedList(l, args[1]);
        if (result == -1)
            return NIL_VAL;
        return INT_VAL(result);
    }
}

Value matrix_nf(int argCount, Value *args)
{
    if (!IS_INT(args[0]) || !IS_INT(args[1]))
    {
        runtimeError("Both arguments must be integers.");
        return NIL_VAL;
    }
    int rows = AS_INT(args[0]);
    int cols = AS_INT(args[1]);
    ObjMatrix *m = newMatrix(rows, cols);
    return OBJ_VAL(m);
}

Value set_row_nf(int argCount, Value *args)
{
    if (!IS_MATRIX(args[0]))
    {
        runtimeError("First argument must be a matrix.");
        return NIL_VAL;
    }
    if (!IS_INT(args[1]))
    {
        runtimeError("Second argument must be an integer.");
        return NIL_VAL;
    }
    if (!IS_ARRAY(args[2]))
    {
        runtimeError("Third argument must be an array.");
        return NIL_VAL;
    }

    ObjMatrix *matrix = AS_MATRIX(args[0]);
    int row = AS_INT(args[1]);
    ObjArray *array = AS_ARRAY(args[2]);

    setRow(matrix, row, array);
    return NIL_VAL;
}

Value set_col_nf(int argCount, Value *args)
{
    if (!IS_MATRIX(args[0]))
    {
        runtimeError("First argument must be a matrix.");
        return NIL_VAL;
    }
    if (!IS_INT(args[1]))
    {
        runtimeError("Second argument must be an integer.");
        return NIL_VAL;
    }
    if (!IS_ARRAY(args[2]))
    {
        runtimeError("Third argument must be an array.");
        return NIL_VAL;
    }

    ObjMatrix *matrix = AS_MATRIX(args[0]);
    int col = AS_INT(args[1]);
    ObjArray *array = AS_ARRAY(args[2]);

    setCol(matrix, col, array);
    return NIL_VAL;
}

Value set_nf(int argCount, Value *args)
{
    if (argCount != 4)
    {
        runtimeError("set() takes 4 arguments.");
        return NIL_VAL;
    }

    if (!IS_MATRIX(args[0]))
    {
        runtimeError("First argument must be a matrix.");
        return NIL_VAL;
    }
    if (!IS_INT(args[1]))
    {
        runtimeError("Second argument must be an integer.");
        return NIL_VAL;
    }
    if (!IS_INT(args[2]))
    {
        runtimeError("Third argument must be an integer.");
        return NIL_VAL;
    }

    ObjMatrix *matrix = AS_MATRIX(args[0]);
    int row = AS_INT(args[1]);
    int col = AS_INT(args[2]);

    setMatrix(matrix, row, col, args[3]);
    return NIL_VAL;
}

Value kolasa_nf(int argCount, Value *args)
{
    if (argCount != 0)
    {
        runtimeError("kolasa() takes no arguments.");
        return NIL_VAL;
    }
    ObjMatrix *m = newMatrix(3, 3);
    for (int i = 0; i < m->len; i++)
    {
        m->data->values[i] = DOUBLE_VAL((double)(i + 1));
    }
    return OBJ_VAL(m);
}

Value rref_nf(int argCount, Value *args)
{
    if (!IS_MATRIX(args[0]))
    {
        runtimeError("First argument must be a matrix.");
        return NIL_VAL;
    }
    ObjMatrix *m = AS_MATRIX(args[0]);
    rref(m);
    return NIL_VAL;
}

Value rank_nf(int argCount, Value *args)
{
    if (!IS_MATRIX(args[0]))
    {
        runtimeError("First argument must be a matrix.");
        return NIL_VAL;
    }
    ObjMatrix *m = AS_MATRIX(args[0]);
    return INT_VAL(rank(m));
}

Value transpose_nf(int argCount, Value *args)
{
    if (!IS_MATRIX(args[0]))
    {
        runtimeError("First argument must be a matrix.");
        return NIL_VAL;
    }
    ObjMatrix *m = AS_MATRIX(args[0]);
    ObjMatrix *t = transposeMatrix(m);
    return OBJ_VAL(t);
}

Value determinant_nf(int argCount, Value *args)
{
    if (!IS_MATRIX(args[0]))
    {
        runtimeError("First argument must be a matrix.");
        return NIL_VAL;
    }
    ObjMatrix *m = AS_MATRIX(args[0]);
    return DOUBLE_VAL(determinant(m));
}

Value fvector_nf(int argCount, Value *args)
{
    if (argCount != 1)
    {
        runtimeError("fvec() takes 1 argument.");
        return NIL_VAL;
    }
    if (!IS_INT(args[0]) && !IS_ARRAY(args[0]))
    {
        runtimeError("First argument must be an integer or an array.");
        return NIL_VAL;
    }

    if (IS_ARRAY(args[0]))
    {
        ObjArray *a = AS_ARRAY(args[0]);
        FloatVector *f = newFloatVector(a->capacity);
        for (int i = 0; i < a->count; i++)
        {
            if (!IS_DOUBLE(a->values[i]))
            {
                runtimeError("All elements of the vector must be doubles.");
                return NIL_VAL;
            }
            pushFloatVector(f, AS_DOUBLE(a->values[i]));
        }
        return OBJ_VAL(f);
    }
    else
    {
        int n = AS_INT(args[0]);
        FloatVector *f = newFloatVector(n);
        return OBJ_VAL(f);
    }
}

Value merge_nf(int argCount, Value *args)
{
    if (argCount != 2)
    {
        runtimeError("merge() takes 2 arguments.");
        return NIL_VAL;
    }
    if (!IS_ARRAY(args[0]) || !IS_ARRAY(args[1]))
    {
        runtimeError("Both arguments must be arrays.");
        return NIL_VAL;
    }
    ObjArray *a = AS_ARRAY(args[0]);
    ObjArray *b = AS_ARRAY(args[1]);
    ObjArray *c = mergeArrays(a, b);
    return OBJ_VAL(c);
}

Value sum_nf(int argCount, Value *args)
{
    if (!IS_ARRAY(args[0]) && !IS_FVECTOR(args[0]))
    {
        runtimeError("First argument must be an array or vector.");
        return NIL_VAL;
    }

    if (IS_ARRAY(args[0]))
    {
        ObjArray *a = AS_ARRAY(args[0]);
        return sumArray(a);
    }
    else
    {
        FloatVector *f = AS_FVECTOR(args[0]);
        return DOUBLE_VAL(sumFloatVector(f));
    }
}

Value mean_nf(int argCount, Value *args)
{
    if (!IS_ARRAY(args[0]) && !IS_FVECTOR(args[0]))
    {
        runtimeError("First argument must be an array or vector.");
        return NIL_VAL;
    }

    if (IS_ARRAY(args[0]))
    {
        ObjArray *a = AS_ARRAY(args[0]);
        return meanArray(a);
    }
    else
    {
        FloatVector *f = AS_FVECTOR(args[0]);
        return DOUBLE_VAL(meanFloatVector(f));
    }
}

Value std_nf(int argCount, Value *args)
{
    if (!IS_ARRAY(args[0]) && !IS_FVECTOR(args[0]))
    {
        runtimeError("First argument must be an array or vector.");
        return NIL_VAL;
    }

    if (IS_ARRAY(args[0]))
    {
        ObjArray *a = AS_ARRAY(args[0]);
        return stdDevArray(a);
    }
    else
    {
        FloatVector *f = AS_FVECTOR(args[0]);
        return DOUBLE_VAL(stdDevFloatVector(f));
    }
}

Value var_nf(int argCount, Value *args)
{
    if (!IS_ARRAY(args[0]) && !IS_FVECTOR(args[0]))
    {
        runtimeError("First argument must be an array or vector.");
        return NIL_VAL;
    }

    if (IS_ARRAY(args[0]))
    {
        ObjArray *a = AS_ARRAY(args[0]);
        return varianceArray(a);
    }
    else
    {
        FloatVector *f = AS_FVECTOR(args[0]);
        return DOUBLE_VAL(varianceFloatVector(f));
    }
}

Value maxl_nf(int argCount, Value *args)
{
    if (!IS_ARRAY(args[0]) && !IS_FVECTOR(args[0]))
    {
        runtimeError("First argument must be an array or vector.");
        return NIL_VAL;
    }

    if (IS_ARRAY(args[0]))
    {
        ObjArray *a = AS_ARRAY(args[0]);
        return maxArray(a);
    }
    else
    {
        FloatVector *f = AS_FVECTOR(args[0]);
        return DOUBLE_VAL(maxFloatVector(f));
    }
}

Value minl_nf(int argCount, Value *args)
{
    if (!IS_ARRAY(args[0]) && !IS_FVECTOR(args[0]))
    {
        runtimeError("First argument must be an array or vector.");
        return NIL_VAL;
    }

    if (IS_ARRAY(args[0]))
    {
        ObjArray *a = AS_ARRAY(args[0]);
        return minArray(a);
    }
    else
    {
        FloatVector *f = AS_FVECTOR(args[0]);
        return DOUBLE_VAL(minFloatVector(f));
    }
}


Value workspace_nf(int argCount, Value *args)
{
    if (argCount != 0)
    {
        runtimeError("workspace() takes no arguments.");
        return NIL_VAL;
    }
    struct Entry *e = entries_(&vm.globals);
    printf("Workspace:\n");
    for (int i = 0; i < vm.globals.capacity; i++)
    {
        if (e[i].key != NULL && !IS_NATIVE(e[i].value))
        {
            printf("%s: ", e[i].key->chars);
            if (IS_MATRIX(e[i].value))
            {
                printf("\n");
            }
            printValue(e[i].value);
            printf("\n");
        }
    }
    return NIL_VAL;
}

Value lu_nf(int argCount, Value *args)
{
    if (!IS_MATRIX(args[0]))
    {
        runtimeError("First argument must be a matrix.");
        return NIL_VAL;
    }
    ObjMatrix *m = AS_MATRIX(args[0]);
    ObjMatrix *result = lu(m);
    return OBJ_VAL(result);
}

Value linspace_nf(int argCount, Value *args)
{
    if (argCount != 3)
    {
        runtimeError("linspace() takes 3 arguments.");
        return NIL_VAL;
    }
    if (!IS_DOUBLE(args[0]) || !IS_DOUBLE(args[1]) || !IS_INT(args[2]))
    {
        runtimeError("First and second arguments must be doubles and the third argument must be an integer.");
        return NIL_VAL;
    }
    double start = AS_DOUBLE(args[0]);
    double end = AS_DOUBLE(args[1]);
    int n = AS_INT(args[2]);
    FloatVector *a = linspace(start, end, n);
    return OBJ_VAL(a);
}

Value interp1_nf(int argCount, Value *args)
{
    if (argCount != 3)
    {
        runtimeError("interp1() takes 3 arguments.");
        return NIL_VAL;
    }
    if (!IS_FVECTOR(args[0]) || !IS_FVECTOR(args[1]) || !IS_DOUBLE(args[2]))
    {
        runtimeError("First and second arguments must be vectors and the third argument must be a double.");
        return NIL_VAL;
    }

    FloatVector *x = AS_FVECTOR(args[0]);
    FloatVector *y = AS_FVECTOR(args[1]);
    double x0 = AS_DOUBLE(args[2]);
    double result = interp1(x, y, x0);
    return DOUBLE_VAL(result);
}

// Cumulative distribution function for the standard normal distribution
static double cdf(double x)
{
    return 0.5 * (1 + erf(x / sqrt(2)));
}

void tridiagonalSolve(double *A, double *B, double *C, double *D, int size, double *result)
{
    double *alpha = malloc(size * sizeof(double));
    double *beta = malloc(size * sizeof(double));

    alpha[0] = B[0];
    beta[0] = D[0] / alpha[0];

    for (int i = 1; i < size; i++)
    {
        alpha[i] = B[i] - A[i] * C[i - 1] / alpha[i - 1];
        beta[i] = (D[i] - A[i] * beta[i - 1]) / alpha[i];
    }

    result[size - 1] = beta[size - 1];

    for (int i = size - 2; i >= 0; i--)
    {
        result[i] = beta[i] - C[i] * result[i + 1] / alpha[i];
    }

    free(alpha);
    free(beta);
}

// Currently has around 8% relative error compared to Matlab's blsprice
static double blsprice(double s, double x, double r, double t, double v)
{
    int N = 100; // Placeholder value for N

    double d1 = (log(s / x) + (r + v * v / 2) * t) / (v * sqrt(t));
    double d2 = d1 - v * sqrt(t);
    double h = 1e-8; // Updated step size for central differences
    double f1 = s * exp(-r * t) * cdf(d1) - x * exp(-r * t) * cdf(d2);
    double f2 = s * exp(-r * t) * cdf(d1 + h) - x * exp(-r * t) * cdf(d2 + h);
    double f3 = s * exp(-r * t) * cdf(d1 - h) - x * exp(-r * t) * cdf(d2 - h);
    double derivative = (f2 - f3) / (2 * h);
    double f = f1 - derivative * v;

    // Calculate call and put prices using implicit method and Crank-Nicolson scheme
    double dt = t / N;        // Time step
    double dx = v * sqrt(dt); // Price step
    double alpha = 0.25 * v * v * dt;
    double beta = r * dt / 2;

    double u[N + 1];     // Solution vector at time t
    double u_new[N + 1]; // Solution vector at time t+dt

    // Initialize boundary conditions
    u[0] = 0;     // Put option payoff at S = 0
    u[N] = s - x; // Call option payoff at S = N*dx

    // Solve the PDE using implicit method and Crank-Nicolson scheme
    for (int i = 1; i < N; i++)
    {
        double a = -alpha * i * i + beta * i;
        double b = 1 + 2 * alpha * i * i + r * dt;
        double c = -alpha * i * i - beta * i;

        // Construct the tridiagonal matrix
        double A[N - 1], B[N - 1], C[N - 1], D[N - 1];
        for (int j = 0; j < N - 1; j++)
        {
            A[j] = -a;
            B[j] = b;
            C[j] = -c;
            D[j] = -u[j + 1] - beta * (u[j + 2] - u[j]);
        }

        // Solve the tridiagonal system using the tridiagonalSolve function
        tridiagonalSolve(A, B, C, D, N - 1, u_new);

        // Update the solution vector
        for (int j = 0; j < N - 1; j++)
        {
            u[j + 1] = u_new[j];
        }
    }

    // Calculate the call and put prices
    double callPrice = u[N];
    double putPrice = u[0];

    return f;
}

Value blsprice_nf(int argCount, Value *args)
{
    if (argCount != 5)
    {
        runtimeError("blsprice() takes 5 arguments.");
        return NIL_VAL;
    }
    if (!IS_DOUBLE(args[0]) || !IS_DOUBLE(args[1]) || !IS_DOUBLE(args[2]) || !IS_DOUBLE(args[3]) || !IS_DOUBLE(args[4]))
    {
        runtimeError("All arguments must be doubles.");
        return NIL_VAL;
    }
    double s = AS_DOUBLE(args[0]);
    double x = AS_DOUBLE(args[1]);
    double r = AS_DOUBLE(args[2]);
    double t = AS_DOUBLE(args[3]);
    double v = AS_DOUBLE(args[4]);
    double result = blsprice(s, x, r, t, v);
    return DOUBLE_VAL(result);
}

Value solve_nf(int argCount, Value *args)
{
    if (argCount != 2)
    {
        runtimeError("solve() takes 2 arguments.");
        return NIL_VAL;
    }
    if (!IS_MATRIX(args[0]) || !IS_ARRAY(args[1]))
    {
        runtimeError("First argument must be a matrix and the second argument must be an array.");
        return NIL_VAL;
    }
    ObjMatrix *a = AS_MATRIX(args[0]);
    ObjMatrix *b = AS_ARRAY(args[1]);
    ObjMatrix *result = solveMatrix(a, b);
    return OBJ_VAL(result);
}