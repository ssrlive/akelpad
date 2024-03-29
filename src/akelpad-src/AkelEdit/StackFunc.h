/*****************************************************************
 *              Stack functions header v3.5                      *
 *                                                               *
 * 2023 Shengalts Aleksander aka Instructor (Shengalts@mail.ru)  *
 *                                                               *
 *                                                               *
 *Linear functions:                                              *
 * StackGetIndexL, StackGetElementL, StackInsertL,               *
 * StackDeleteL, StackSizeL, StackClearL                         *
 *                                                               *
 *Bilinear functions:                                            *
 * StackGetIndex, StackGetElement, StackInsertBefore,            *
 * StackInsertAfter, StackDelete, StackMoveBefore,               *
 * StackMoveAfter, StackExchange, StackJoin, StackSplit,         *
 * StackSize, StackClear                                         *
 *                                                               *
 *Special functions:                                             *
 * StackInsertIndex, StackMoveIndex, StackDeleteRange,           *
 * StackMoveRange, StackReverseRange, StackCopy,                 *
 * StackPushSortA, StackSortA                                    *
 *                                                               *
 *****************************************************************/

#ifndef _HSTACK_STRUCT_
#define _HSTACK_STRUCT_
typedef struct {
  INT_PTR first;
  INT_PTR last;
} HSTACK;
#endif

#ifndef _STACKL_STRUCT_
#define _STACKL_STRUCT_
typedef struct _stackL {
  struct _stackL *next;
} stackL;
#endif

#ifndef _STACK_STRUCT_
#define _STACK_STRUCT_
typedef struct _stack {
  struct _stack *next;
  struct _stack *prev;
} stack;
#endif

#ifndef _STACKS_STRUCT_
#define _STACKS_STRUCT_
typedef struct _stackS {
  struct _stackS *next;
  struct _stackS *prev;
  char string[MAX_PATH];
} stackS;
#endif

#ifndef _STACKFUNC_H_
#define _STACKFUNC_H_

#define ALLSTACKFUNC 1

int StackGetIndexL(stackL *first, stackL *element);
int StackGetElementL(stackL *first, stackL **element, int nIndex);
int StackInsertL(stackL **first, stackL **element, int nIndex, int nBytes);
int StackDeleteL(stackL **first, int nIndex);
int StackSizeL(stackL *first);
void StackClearL(stackL **first);

int StackGetIndex(stack *first, stack *last, stack *element, BOOL bPositive);
int StackGetElement(stack *first, stack *last, stack **element, int nIndex);
int StackInsertBefore(stack **first, stack **last, stack *index, stack **element, int nBytes);
int StackInsertAfter(stack **first, stack **last, stack *index, stack **element, int nBytes);
int StackDelete(stack **first, stack **last, stack *element);
int StackMoveBefore(stack **first, stack **last, stack *src, stack *dst);
int StackMoveAfter(stack **first, stack **last, stack *src, stack *dst);
int StackExchange(stack **first, stack **last, stack *element1, stack *element2);
void StackJoin(stack **first, stack **last, stack *index, stack *joinfirst, stack *joinlast);
void StackSplit(stack **first, stack **last, stack *splitfirst, stack *splitlast);
int StackSize(stack *first, stack *last);
void StackClear(stack **first, stack **last);

int StackInsertIndex(stack **first, stack **last, stack **element, int nIndex, int nBytes);
int StackMoveIndex(stack **first, stack **last, stack *element, int nIndex);
int StackDeleteRange(stack **first, stack **last, int nIndex, int nIndex2);
int StackMoveRange(stack **first, stack **last, int nIndex, int nIndex2, int nIndex3);
void StackReverseRange(stack **first, stack **last, stack *rangemin, stack *rangemax);
int StackCopy(stack *first, stack *last, stack **copyfirst, stack **copylast, int nBytes);
int StackPushSortA(stackS **first, stackS **last, stackS **element, char *pString, int nUpDown, int nBytes);
int StackSortA(stackS **first, stackS **last, int nUpDown);


/********************************************************************
 ********************************************************************
 *                                                                  *
 *                   Linear chain functions (+4)                    *
 *                                                                  *
 ********************************************************************
 ********************************************************************/


/********************************************************************
 *
 *  StackGetIndexL
 *
 *Finds the element index.
 *
 * [in] stackL *first    -Pointer to a pointer that specifies
 *                        the first element in the stack
 * [in] stackL *element  -Pointer to the element
 *
 *Returns: element index, zero if not found
 ********************************************************************/
#if defined StackGetIndexL || defined ALLSTACKFUNC
#define StackGetIndexL_INCLUDED
#undef StackGetIndexL
__inline int StackGetIndexL(stackL *first, stackL *element)
{
  int nCount=1;

  while (first)
  {
    if (first == element)
      return nCount;

    ++nCount;
    first=first->next;
  }
  return 0;
}
#endif

/********************************************************************
 *
 *  StackGetElementL
 *
 *Finds the element by index and returns pointer on it.
 *
 * [in] stackL *first    -Pointer to a pointer that specifies
 *                        the first element in the stack
 *[out] stackL **element -Pointer to a pointer to the element
 * [in] int nIndex       -Index of the element
 *
 *Returns: 0 on success
 *         1 on wrong index
 ********************************************************************/
#if defined StackGetElementL || defined ALLSTACKFUNC
#define StackGetElementL_INCLUDED
#undef StackGetElementL
__inline int StackGetElementL(stackL *first, stackL **element, int nIndex)
{
  int nCount=1;

  *element=NULL;

  while (first)
  {
    if (nCount == nIndex)
    {
      *element=first;
      return 0;
    }
    ++nCount;
    first=first->next;
  }
  return 1;
}
#endif

/********************************************************************
 *
 *  StackInsertL
 *
 *Inserts new element at specified index.
 *
 *[in,out] stackL **first   -Pointer to a pointer that specifies
 *                           the first element in the stack
 *   [out] stackL **element -Pointer to a pointer to the element
 *    [in] int nIndex       -Index of the element
 *    [in] int nBytes       -Size of the structure
 *
 *Returns: 0 on success
 *         1 on wrong index
 *         2 on memory allocating error
 ********************************************************************/
#if defined StackInsertL || defined ALLSTACKFUNC
#define StackInsertL_INCLUDED
#undef StackInsertL
__inline int StackInsertL(stackL **first, stackL **element, int nIndex, int nBytes)
{
  stackL *tmp=*first;
  stackL *tmpPrev=NULL;
  int nCount=1;

  *element=NULL;

  while (tmp || nCount == nIndex)
  {
    if (nCount == nIndex)
    {
      if (*element=(stackL *)GlobalAlloc(GPTR, nBytes))
      {
        if (!tmpPrev)
        {
          (*element)->next=*first;
          *first=*element;
        }
        else
        {
          (*element)->next=tmp;
          tmpPrev->next=*element;
        }
      }
      else return 2;

      return 0;
    }
    ++nCount;
    tmpPrev=tmp;
    tmp=tmp->next;
  }
  return 1;
}
#endif

/********************************************************************
 *
 *  StackDeleteL
 *
 *Removes element from the specified index.
 *
 *[in,out] stackL **first   -Pointer to a pointer that specifies
 *                           the first element in the stack
 *    [in] int nIndex       -Index of the element
 *
 *Returns: 0 on success
 *         1 on wrong index
 ********************************************************************/
#if defined StackDeleteL || defined ALLSTACKFUNC
#define StackDeleteL_INCLUDED
#undef StackDeleteL
__inline int StackDeleteL(stackL **first, int nIndex)
{
  stackL *tmp=*first;
  stackL *tmpPrev=NULL;
  int nCount=1;

  while (tmp)
  {
    if (nCount == nIndex)
    {
      if (!tmpPrev)
        *first=(*first)->next;
      else
        tmpPrev->next=tmp->next;
      GlobalFree((HGLOBAL)tmp);
      return 0;
    }
    ++nCount;
    tmpPrev=tmp;
    tmp=tmp->next;
  }
  return 1;
}
#endif

/********************************************************************
 *
 *  StackSizeL
 *
 *Gets the number of elements in the stack.
 *
 *[in] stackL *first   -Pointer that specifies the first
 *                      element in the stack
 *
 *Returns: the number of elements
 ********************************************************************/
#if defined StackSizeL || defined ALLSTACKFUNC
#define StackSizeL_INCLUDED
#undef StackSizeL
__inline int StackSizeL(stackL *first)
{
  int nCount=0;

  while (first)
  {
    ++nCount;
    first=first->next;
  }
  return nCount;
}
#endif

/********************************************************************
 *
 *  StackClearL
 *
 *Clear all stack.
 *
 *[in,out] stackL **first   -Pointer to a pointer that specifies
 *                           the first element in the stack
 ********************************************************************/
#if defined StackClearL || defined ALLSTACKFUNC
#define StackClearL_INCLUDED
#undef StackClearL
__inline void StackClearL(stackL **first)
{
  stackL *tmp=*first;
  stackL *tmp2;

  while (tmp)
  {
    tmp2=tmp->next;
    GlobalFree((HGLOBAL)tmp);
    tmp=tmp2;
  }
  *first=NULL;
}
#endif


/********************************************************************
 ********************************************************************
 *                                                                  *
 *                   Bilinear chain functions (+8)                  *
 *                                                                  *
 ********************************************************************
 ********************************************************************/


/********************************************************************
 *
 *  StackGetIndex
 *
 *Finds the element index.
 *
 * [in] stack *first    -Pointer to a pointer that specifies
 *                       the first element in the stack
 * [in] stack *last     -Pointer to a pointer that specifies
 *                       the last element in the stack
 * [in] stack *element  -Pointer to the element
 * [in] BOOL bPositive  -TRUE  positive index will be returned
 *                       FALSE negative index will be returned
 *
 *Returns: element index, zero if not found
 ********************************************************************/
#if defined StackGetIndex || defined ALLSTACKFUNC
#define StackGetIndex_INCLUDED
#undef StackGetIndex
__inline int StackGetIndex(stack *first, stack *last, stack *element, BOOL bPositive)
{
  stack *tmp;
  int nCount;

  if (bPositive)
  {
    tmp=first;

    for (nCount=1; tmp; ++nCount)
    {
      if (tmp == element)
        return nCount;

      tmp=tmp->next;
    }
  }
  else
  {
    tmp=last;

    for (nCount=-1; tmp; --nCount)
    {
      if (tmp == element)
        return nCount;

      tmp=tmp->prev;
    }
  }
  return 0;
}
#endif

/********************************************************************
 *
 *  StackGetElement
 *
 *Finds the element by index and returns pointer on it.
 *
 * [in] stack *first    -Pointer that specifies the first
 *                       element in the stack
 * [in] stack *last     -Pointer that specifies the last
 *                       element in the stack
 *[out] stack **element -Pointer to a pointer to the element
 * [in] int nIndex      -Number of the element if positive search
 *                       from first element if negative from last element
 *
 *Returns: 0 on success
 *         1 on wrong index
 ********************************************************************/
#if defined StackGetElement || defined ALLSTACKFUNC
#define StackGetElement_INCLUDED
#undef StackGetElement
__inline int StackGetElement(stack *first, stack *last, stack **element, int nIndex)
{
  stack *tmp;
  int nCount;

  *element=NULL;

  if (nIndex > 0)
  {
    tmp=first;
    nCount=1;
  }
  else if (nIndex < 0)
  {
    tmp=last;
    nCount=-1;
  }
  else return 1;

  while (tmp)
  {
    if (nCount == nIndex)
    {
      *element=tmp;
      return 0;
    }
    if (nIndex > 0)
    {
      tmp=tmp->next;
      ++nCount;
    }
    else
    {
      tmp=tmp->prev;
      --nCount;
    }
  }
  return 1;
}
#endif

/********************************************************************
 *
 *  StackInsertBefore
 *
 *Inserts new element before specified element.
 *
 *[in,out] stack **first   -Pointer to a pointer that specifies
 *                          the first element in the stack
 *[in,out] stack **last    -Pointer to a pointer that specifies
 *                          the last element in the stack
 *    [in] stack *element  -Insert before this element,
 *                          if NULL then insert to the end of the stack
 *   [out] stack **element -Pointer to a pointer to the element
 *    [in] int nBytes      -Size of the structure
 *
 *Returns: 0 on success
 *         2 on memory allocating error
 ********************************************************************/
#if defined StackInsertBefore || defined ALLSTACKFUNC
#define StackInsertBefore_INCLUDED
#undef StackInsertBefore
__inline int StackInsertBefore(stack **first, stack **last, stack *index, stack **element, int nBytes)
{
  *element=NULL;

  if (*element=(stack *)GlobalAlloc(GPTR, nBytes))
  {
    if (!index)
    {
      if (*last)
      {
        (*last)->next=*element;
        (*element)->prev=*last;
      }
      else
      {
        *first=*element;
      }
      *last=*element;
    }
    else
    {
      if (index == *first) *first=*element;
      else index->prev->next=*element;

      (*element)->next=index;
      (*element)->prev=index->prev;
      index->prev=*element;
    }
  }
  else return 2;

  return 0;
}
#endif

/********************************************************************
 *
 *  StackInsertAfter
 *
 *Inserts new element after specified element.
 *
 *[in,out] stack **first   -Pointer to a pointer that specifies
 *                          the first element in the stack
 *[in,out] stack **last    -Pointer to a pointer that specifies
 *                          the last element in the stack
 *    [in] stack *element  -Insert after this element,
 *                          if NULL then insert to the top of the stack
 *   [out] stack **element -Pointer to a pointer to the element
 *    [in] int nBytes      -Size of the structure
 *
 *Returns: 0 on success
 *         2 on memory allocating error
 ********************************************************************/
#if defined StackInsertAfter || defined ALLSTACKFUNC
#define StackInsertAfter_INCLUDED
#undef StackInsertAfter
__inline int StackInsertAfter(stack **first, stack **last, stack *index, stack **element, int nBytes)
{
  *element=NULL;

  if (*element=(stack *)GlobalAlloc(GPTR, nBytes))
  {
    if (!index)
    {
      if (*first)
      {
        (*first)->prev=*element;
        (*element)->next=*first;
      }
      else
      {
        *last=*element;
      }
      *first=*element;
    }
    else
    {
      if (index == *last) *last=*element;
      else index->next->prev=*element;

      (*element)->prev=index;
      (*element)->next=index->next;
      index->next=*element;
    }
  }
  else return 2;

  return 0;
}
#endif

/********************************************************************
 *
 *  StackDelete
 *
 *Removes element.
 *
 *[in,out] stack **first   -Pointer to a pointer that specifies
 *                          the first element in the stack
 *[in,out] stack **last    -Pointer to a pointer that specifies
 *                          the last element in the stack
 *    [in] stack *element  -Pointer to the element
 *
 *Returns: 0 on success
 *         1 on empty stack
 ********************************************************************/
#if defined StackDelete || defined ALLSTACKFUNC
#define StackDelete_INCLUDED
#undef StackDelete
__inline int StackDelete(stack **first, stack **last, stack *element)
{
  if (!element) return 1;

  if (element == *first)
  {
    *first=element->next;
    if (*first) (*first)->prev=NULL;
    else *last=NULL;
  }
  else if (element == *last)
  {
    *last=element->prev;
    if (*last) (*last)->next=NULL;
    else *first=NULL;
  }
  else
  {
    element->prev->next=element->next;
    element->next->prev=element->prev;
  }
  GlobalFree((HGLOBAL)element);
  return 0;
}
#endif

/********************************************************************
 *
 *  StackMoveBefore
 *
 *Move source element to the position before destination element.
 *
 *[in,out] stack **first   -Pointer to a pointer that specifies
 *                          the first element in the stack
 *[in,out] stack **last    -Pointer to a pointer that specifies
 *                          the last element in the stack
 *    [in] stack *src      -Pointer to the element (source)
 *    [in] stack *dst      -Pointer to the element (destination),
 *                          if NULL then insert to the end of the stack
 *
 *Returns: 0 on success
 *         2 indexes pointed to the same element
 ********************************************************************/
#if defined StackMoveBefore || defined ALLSTACKFUNC
#define StackMoveBefore_INCLUDED
#undef StackMoveBefore
__inline int StackMoveBefore(stack **first, stack **last, stack *src, stack *dst)
{
  if (src == dst) return 2;

  //Unlink src
  if (src == *first)
  {
    *first=src->next;
    if (*first) (*first)->prev=NULL;
    else *last=NULL;
  }
  else if (src == *last)
  {
    *last=src->prev;
    if (*last) (*last)->next=NULL;
    else *first=NULL;
  }
  else
  {
    src->prev->next=src->next;
    src->next->prev=src->prev;
  }
  src->next=NULL;
  src->prev=NULL;

  //Insert element
  if (!dst)
  {
    if (*last)
    {
      (*last)->next=src;
      src->prev=*last;
    }
    else
    {
      *first=src;
    }
    *last=src;
  }
  else
  {
    if (dst == *first) *first=src;
    else dst->prev->next=src;

    src->next=dst;
    src->prev=dst->prev;
    dst->prev=src;
  }
  return 0;
}
#endif

/********************************************************************
 *
 *  StackMoveAfter
 *
 *Move source element to the position after destination element.
 *
 *[in,out] stack **first   -Pointer to a pointer that specifies
 *                          the first element in the stack
 *[in,out] stack **last    -Pointer to a pointer that specifies
 *                          the last element in the stack
 *    [in] stack *src      -Pointer to the element (source)
 *    [in] stack *dst      -Pointer to the element (destination),
 *                          if NULL then insert to the top of the stack
 *
 *Returns: 0 on success
 *         2 indexes pointed to the same element
 ********************************************************************/
#if defined StackMoveAfter || defined ALLSTACKFUNC
#define StackMoveAfter_INCLUDED
#undef StackMoveAfter
__inline int StackMoveAfter(stack **first, stack **last, stack *src, stack *dst)
{
  if (src == dst) return 2;

  //Unlink src
  if (src == *first)
  {
    *first=src->next;
    if (*first) (*first)->prev=NULL;
    else *last=NULL;
  }
  else if (src == *last)
  {
    *last=src->prev;
    if (*last) (*last)->next=NULL;
    else *first=NULL;
  }
  else
  {
    src->prev->next=src->next;
    src->next->prev=src->prev;
  }
  src->next=NULL;
  src->prev=NULL;

  //Insert element
  if (!dst)
  {
    if (*first)
    {
      (*first)->prev=src;
      src->next=*first;
    }
    else
    {
      *last=src;
    }
    *first=src;
  }
  else
  {
    if (dst == *last) *last=src;
    else dst->next->prev=src;

    src->prev=dst;
    src->next=dst->next;
    dst->next=src;
  }
  return 0;
}
#endif

/********************************************************************
 *
 *  StackExchange
 *
 *Finds the elements by indexes and exchanges them.
 *
 *[in,out] stack **first   -Pointer to a pointer that specifies
 *                          the first element in the stack
 *[in,out] stack **last    -Pointer to a pointer that specifies
 *                          the last element in the stack
 *    [in] stack *element1 -Pointer to the first element
 *    [in] stack *element2 -Pointer to the second element
 *
 *Returns: 0 on success
 *         1 on empty stack
 *         2 indexes pointed to the same element
 ********************************************************************/
#if defined StackExchange || defined ALLSTACKFUNC
#define StackExchange_INCLUDED
#undef StackExchange
__inline int StackExchange(stack **first, stack **last, stack *element1, stack *element2)
{
  stack *tmpNext;
  stack *tmpPrev;

  if (!element1 || !element2) return 1;
  if (element1 == element2) return 2;

  if (element1->next == element2)
  {
    if (element1 == *first) *first=element2;
    else element1->prev->next=element2;

    if (element2 == *last) *last=element1;
    else element2->next->prev=element1;

    element1->next=element2->next;
    element2->next=element1;
    element2->prev=element1->prev;
    element1->prev=element2;
  }
  else if (element1->prev == element2)
  {
    if (element2 == *first) *first=element1;
    else element2->prev->next=element1;

    if (element1 == *last) *last=element2;
    else element1->next->prev=element2;

    element2->next=element1->next;
    element1->next=element2;
    element1->prev=element2->prev;
    element2->prev=element1;
  }
  else
  {
    if (element1 == *first)
    {
      *first=element2;
      element2->prev->next=element1;
    }
    else if (element2 == *first)
    {
      *first=element1;
      element1->prev->next=element2;
    }
    else
    {
      element1->prev->next=element2;
      element2->prev->next=element1;
    }

    if (element1 == *last)
    {
      *last=element2;
      element2->next->prev=element1;
    }
    else if (element2 == *last)
    {
      *last=element1;
      element1->next->prev=element2;
    }
    else
    {
      element1->next->prev=element2;
      element2->next->prev=element1;
    }

    tmpNext=element1->next;
    tmpPrev=element1->prev;

    element1->next=element2->next;
    element1->prev=element2->prev;
    element2->next=tmpNext;
    element2->prev=tmpPrev;
  }
  return 0;
}
#endif

/********************************************************************
 *
 *  StackJoin
 *
 *Joins two stacks.
 *
 *[in,out] stack **first     -Pointer to a pointer that specifies
 *                            the first element in the first stack
 *[in,out] stack **last      -Pointer to a pointer that specifies
 *                            the last element in the first stack
 *    [in] stack *index      -Pointer to element in first stack.
 *                            Second stack will be inserted before this element,
 *                            if NULL then will be inserted to the end of the first stack.
 *    [in] stack *joinfirst  -Pointer to the first element in the second stack
 *    [in] stack *joinlast   -Pointer to the last element in the second stack
 ********************************************************************/
#if defined StackJoin || defined ALLSTACKFUNC
#define StackJoin_INCLUDED
#undef StackJoin
__inline void StackJoin(stack **first, stack **last, stack *index, stack *joinfirst, stack *joinlast)
{
  if (!joinfirst || !joinlast)
    return;

  if (!index)
  {
    if (*last)
    {
      (*last)->next=joinfirst;
      joinfirst->prev=*last;
    }
    else
    {
      *first=joinfirst;
    }
    *last=joinlast;
  }
  else
  {
    if (index == *first) *first=joinfirst;
    else index->prev->next=joinfirst;

    joinlast->next=index;
    joinfirst->prev=index->prev;
    index->prev=joinlast;
  }
}
#endif

/********************************************************************
 *
 *  StackSplit
 *
 *Splits one stack into two stacks.
 *
 *[in,out] stack **first       -Pointer to a pointer that specifies
 *                              the first element in the stack
 *[in,out] stack **last        -Pointer to a pointer that specifies
 *                              the last element in the stack
 *[in] stack *splitfirst       -Pointer to the element that will be
 *                              the first element in the second stack
 *[in] stack *splitlast        -Pointer to the element that will be
 *                              the last element in the second stack
 ********************************************************************/
#if defined StackSplit || defined ALLSTACKFUNC
#define StackSplit_INCLUDED
#undef StackSplit
__inline void StackSplit(stack **first, stack **last, stack *splitfirst, stack *splitlast)
{
  if (!splitfirst || !splitlast)
    return;

  if (splitfirst == *first ||
      splitlast == *last)
  {
    if (splitfirst == *first)
      *first=splitlast->next;
    if (splitlast == *last)
      *last=splitfirst->prev;
  }
  else
  {
    splitfirst->prev->next=splitlast->next;
    splitlast->next->prev=splitfirst->prev;
  }
  if (*first) (*first)->prev=NULL;
  if (*last) (*last)->next=NULL;
  splitfirst->prev=NULL;
  splitlast->next=NULL;
}
#endif

/********************************************************************
 *
 *  StackSize
 *
 *Gets the number of elements in the stack.
 *
 *[in] stack *first   -Pointer that specifies the first
 *                     element in the stack
 *[in] stack *last    -Pointer that specifies the last
 *                     element in the stack
 *
 *Returns: the number of elements
 ********************************************************************/
#if defined StackSize || defined ALLSTACKFUNC
#define StackSize_INCLUDED
#undef StackSize
__inline int StackSize(stack *first, stack *last)
{
  stack *tmp=last;
  int nCount;

  for (nCount=0; (tmp); ++nCount)
    tmp=tmp->prev;
  return nCount;
}
#endif

/********************************************************************
 *
 *  StackClear
 *
 *Clear all stack.
 *
 *[in,out] stack **first   -Pointer to a pointer that specifies
 *                          the first element in the stack
 *[in,out] stack **last    -Pointer to a pointer that specifies
 *                          the last element in the stack
 ********************************************************************/
#if defined StackClear || defined ALLSTACKFUNC
#define StackClear_INCLUDED
#undef StackClear
__inline void StackClear(stack **first, stack **last)
{
  stack *tmp1=*last;
  stack *tmp2;

  while (tmp1)
  {
    tmp2=tmp1->prev;
    GlobalFree((HGLOBAL)tmp1);
    tmp1=tmp2;
  }
  *last=NULL;
  *first=NULL;
}
#endif


/********************************************************************
 ********************************************************************
 *                                                                  *
 *                   Special bilinear functions                     *
 *                                                                  *
 ********************************************************************
 ********************************************************************/


/********************************************************************
 *
 *  StackInsertIndex
 *
 *Inserts new element at specified index.
 *
 *[in,out] stack **first   -Pointer to a pointer that specifies
 *                          the first element in the stack
 *[in,out] stack **last    -Pointer to a pointer that specifies
 *                          the last element in the stack
 *   [out] stack **element -Pointer to a pointer to the element
 *    [in] int nIndex      -Number of the element if positive search
 *                          from top if negative from beginning
 *    [in] int nBytes      -Size of the structure
 *
 *Returns: 0 on success
 *         1 on wrong index
 *         2 on memory allocating error
 *
 *Note:
 *  StackInsertIndex uses StackInsertAfter, StackInsertBefore
 ********************************************************************/
#if defined StackInsertIndex || defined ALLSTACKFUNC
#define StackInsertIndex_INCLUDED
#undef StackInsertIndex
__inline int StackInsertIndex(stack **first, stack **last, stack **element, int nIndex, int nBytes)
{
  stack *tmp;
  int nCount;

  *element=NULL;

  if (nIndex > 0)
  {
    tmp=*first;
    nCount=1;
  }
  else if (nIndex < 0)
  {
    tmp=*last;
    nCount=-1;
  }
  else return 1;

  while (tmp || nCount == nIndex)
  {
    if (nCount == nIndex)
    {
      if (nIndex > 0)
        return StackInsertBefore(first, last, tmp, element, nBytes);
      else
        return StackInsertAfter(first, last, tmp, element, nBytes);
    }
    if (nIndex > 0)
    {
      tmp=tmp->next;
      ++nCount;
    }
    else
    {
      tmp=tmp->prev;
      --nCount;
    }
  }
  return 1;
}
#endif

/********************************************************************
 *
 *  StackMoveIndex
 *
 *Move element to the new index.
 *
 *[in,out] stack **first   -Pointer to a pointer that specifies
 *                          the first element in the stack
 *[in,out] stack **last    -Pointer to a pointer that specifies
 *                          the last element in the stack
 *    [in] stack *element  -Pointer to the element (source)
 *    [in] int nIndex      -Number of the element if positive search
 *                          from top if negative from beginning (destination)
 *
 *Returns: 0 on success
 *         1 on wrong index
 *         2 source and destination indexes pointed to the same element
 *
 *Note:
 *  StackMoveIndex uses StackMoveAfter, StackMoveBefore
 ********************************************************************/
#if defined StackMoveIndex || defined ALLSTACKFUNC
#define StackMoveIndex_INCLUDED
#undef StackMoveIndex
__inline int StackMoveIndex(stack **first, stack **last, stack *element, int nIndex)
{
  stack *tmp;
  int nCount;

  if (nIndex > 0)
  {
    tmp=*first;
    nCount=1;
  }
  else if (nIndex < 0)
  {
    tmp=*last;
    nCount=-1;
  }
  else return 1;

  while (tmp || nCount == nIndex)
  {
    if (nCount == nIndex)
    {
      if (nIndex > 0)
        return StackMoveBefore(first, last, element, tmp);
      else
        return StackMoveAfter(first, last, element, tmp);
    }
    if (nIndex > 0)
    {
      if (element != tmp) ++nCount;
      tmp=tmp->next;
    }
    else
    {
      if (element != tmp) --nCount;
      tmp=tmp->prev;
    }
  }
  return 1;
}
#endif

/********************************************************************
 *
 *  StackDeleteRange
 *
 *Finds the elements between indexes and removes.
 *
 *[in,out] stack **first   -Pointer to a pointer that specifies
 *                          the first element in the stack
 *[in,out] stack **last    -Pointer to a pointer that specifies
 *                          the last element in the stack
 *    [in] int nIndex      -Number of the element if positive search
 *                          from first element if negative from last element (range limit)
 *    [in] int nIndex2     -Number of the element if positive search
 *                          from first element if negative from last element (range limit)
 *
 *Returns: 0 on success
 *         1 on empty stack
 ********************************************************************/
#if defined StackDeleteRange || defined ALLSTACKFUNC
#define StackDeleteRange_INCLUDED
#undef StackDeleteRange
__inline int StackDeleteRange(stack **first, stack **last, int nIndex, int nIndex2)
{
  stack *tmp;
  stack *tmp2=NULL;
  stack *tmp3=NULL;
  stack *tmp4;
  int nCount;
  BOOL bMeet=FALSE;
  BOOL bExit=FALSE;

  loop:

  if (nIndex > 0)
  {
    tmp=*first;
    nCount=1;
  }
  else if (nIndex < 0)
  {
    tmp=*last;
    nCount=-1;
  }
  else return 1;

  while (tmp)
  {
    if (tmp == tmp2) bMeet=TRUE;

    if (nCount == nIndex)
    {
      if (!tmp2)
      {
        tmp2=tmp;
        nIndex=nIndex2;
        goto loop;
      }
      if ((bMeet == FALSE && nIndex > 0) || (bMeet == TRUE && nIndex < 0))
      {
        tmp3=tmp;
        tmp4=tmp2;
        tmp=tmp4;
        tmp2=tmp3;
      }

      if ((tmp2 == *first) && (tmp == *last))
      {
        *first=NULL;
        *last=NULL;
      }
      else if (tmp2 == *first)
      {
        *first=tmp->next;
        (*first)->prev=NULL;
      }
      else if (tmp == *last)
      {
        *last=tmp2->prev;
        (*last)->next=NULL;
      }
      else
      {
        tmp2->prev->next=tmp->next;
        tmp->next->prev=tmp2->prev;
      }

      for (; bExit != TRUE; tmp=tmp3)
      {
        if (tmp2 == tmp) bExit=TRUE;
        else tmp3=tmp->prev;
        GlobalFree((HGLOBAL)tmp);
      }
      return 0;
    }
    if (nIndex > 0)
    {
      tmp=tmp->next;
      ++nCount;
    }
    else
    {
      tmp=tmp->prev;
      --nCount;
    }
  }
  return 1;
}
#endif

/********************************************************************
 *
 *  StackMoveRange
 *
 *Finds the elements by indexes and move them to the new index.
 *
 *[in,out] stack **first   -Pointer to a pointer that specifies
 *                          the first element in the stack
 *[in,out] stack **last    -Pointer to a pointer that specifies
 *                          the last element in the stack
 *    [in] int nIndex      -Number of the element if positive search
 *                          from first element if negative from last element (range limit)
 *    [in] int nIndex2     -Number of the element if positive search
 *                          from first element if negative from last element (range limit)
 *    [in] int nIndex3     -Number of the element if positive search
 *                          from first element if negative from last element (destination)
 *
 *Returns: 0 on success
 *         1 on empty stack
 *         2 destination index pointed to the element in the range
 ********************************************************************/
#if defined StackMoveRange || defined ALLSTACKFUNC
#define StackMoveRange_INCLUDED
#undef StackMoveRange
__inline int StackMoveRange(stack **first, stack **last, int nIndex, int nIndex2, int nIndex3)
{
  stack *tmp;
  stack *tmp2=NULL;
  stack *tmp3=NULL;
  stack *tmp4;
  stack *tmp5;
  int nCount;
  int nMeet=0;
  BOOL bMeet=FALSE;

  loop:

  if (nIndex > 0)
  {
    tmp=*first;
    nCount=1;
  }
  else if (nIndex < 0)
  {
    tmp=*last;
    nCount=-1;
  }
  else return 1;

  while (tmp)
  {
    if (tmp3)
    {
      if (tmp == tmp2) ++nMeet;
      if (tmp == tmp3) ++nMeet;
      else if (nMeet == 2) nMeet=3;
    }
    else if (tmp == tmp2) bMeet=TRUE;

    if (nCount == nIndex)
    {
      if (!tmp2)
      {
        tmp2=tmp;
        nIndex=nIndex2;
        goto loop;
      }
      if (!tmp3)
      {
        tmp3=tmp;
        nIndex=nIndex3;
        goto loop;
      }

      if (nMeet == 1 || nMeet == 2) return 2;

      if ((bMeet == FALSE && nIndex2 > 0) || (bMeet == TRUE && nIndex2 < 0))
      {
        tmp4=tmp2;
        tmp5=tmp3;
        tmp2=tmp5;
        tmp3=tmp4;
      }

      if (tmp2 == *first && tmp3 == *last) return 2;

      if (tmp2 == *first)
      {
        *first=tmp3->next;
        tmp3->next->prev=NULL;
      }
      else if (tmp3 == *last)
      {
        *last=tmp2->prev;
        tmp2->prev->next=NULL;
      }
      else
      {
        tmp3->next->prev=tmp2->prev;
        tmp2->prev->next=tmp3->next;
      }

      if ((nMeet == 0 && nIndex > 0) || (nMeet == 3 && nIndex < 0))
      {
        if (tmp == *first) *first=tmp2;
        else tmp->prev->next=tmp2;

        tmp3->next=tmp;
        tmp2->prev=tmp->prev;
        tmp->prev=tmp3;
      }
      else
      {
        if (tmp == *last) *last=tmp3;
        else tmp->next->prev=tmp3;

        tmp2->prev=tmp;
        tmp3->next=tmp->next;
        tmp->next=tmp2;
      }
      return 0;
    }
    if (nIndex > 0)
    {
      tmp=tmp->next;
      ++nCount;
    }
    else
    {
      tmp=tmp->prev;
      --nCount;
    }
  }
  return 1;
}
#endif

/********************************************************************
 *
 *  StackReverseRange
 *
 *Reverse stack.
 *
 *[in,out] stack **first     -Pointer to a pointer that specifies
 *                            the first element in the stack
 *[in,out] stack **last      -Pointer to a pointer that specifies
 *                            the last element in the stack
 *    [in] stack *rangemin   -Pointer to a first element in range
 *    [in] stack *rangemax   -Pointer to a last element in range
 *
 *Note:
 *  StackReverseRange uses StackExchange
 ********************************************************************/
#if defined StackReverseRange || defined ALLSTACKFUNC
#define StackReverseRange_INCLUDED
#undef StackReverseRange
__inline void StackReverseRange(stack **first, stack **last, stack *rangemin, stack *rangemax)
{
  stack *tmp1=rangemin;
  stack *tmp2=rangemax;
  stack *tmpNext;
  stack *tmpPrev;
  BOOL bBreak=FALSE;

  for (;;)
  {
    tmpNext=tmp1->next;
    tmpPrev=tmp2->prev;

    if (tmp1->next == tmp2) bBreak=TRUE;
    if (StackExchange(first, last, tmp1, tmp2)) break;
    if (bBreak) break;

    tmp1=tmpNext;
    tmp2=tmpPrev;
  }
}
#endif

/********************************************************************
 *
 *  StackCopy
 *
 *Copy stack.
 *
 *    [in] stack *first      -Pointer that specifies the first
 *                            element in the stack (source)
 *    [in] stack *last       -Pointer that specifies the last
 *                            element in the stack (source)
 *[in,out] stack **copyfirst -Pointer to a pointer that specifies
 *                            the first element in the stack (destination)
 *[in,out] stack **copylast  -Pointer to a pointer that specifies
 *                            the last element in the stack (destination)
 *    [in] int nBytes        -Size of the structure
 *
 *Returns: 0 on success
 *         1 on wrong index
 *         2 on memory allocating error
 *Note:
 *  StackCopy uses StackInsertAfter
 *  After copylast new elements from source will be inserted
 ********************************************************************/
#if defined StackCopy || defined ALLSTACKFUNC
#define StackCopy_INCLUDED
#undef StackCopy
__inline int StackCopy(stack *first, stack *last, stack **copyfirst, stack **copylast, int nBytes)
{
  stack *tmp1=first;
  stack *tmp2;
  unsigned char *lpData1;
  unsigned char *lpData2;
  int nDataBytes;
  int nResult=0;

  while (tmp1)
  {
    if (!(nResult=StackInsertAfter(copyfirst, copylast, *copylast, &tmp2, nBytes)))
    {
      lpData1=(unsigned char *)tmp1 + sizeof(stack);
      lpData2=(unsigned char *)tmp2 + sizeof(stack);
      nDataBytes=nBytes - sizeof(stack);

      while (nDataBytes-- > 0)
      {
        *lpData2++=*lpData1++;
      }
    }
    else break;

    if (tmp1 == last) break;
    tmp1=tmp1->next;
  }
  return nResult;
}
#endif

/********************************************************************
 *
 *  StackPushSortA
 *
 *Pushs element to the stack and sorts alphabetically in ascending or descending.
 *
 *[in,out] stackS **first   -Pointer to a pointer that specifies
 *                           the first element in the stack
 *[in,out] stackS **last    -Pointer to a pointer that specifies
 *                           the last element in the stack
 *   [out] stackS **element -Pointer to a pointer to the element
 *    [in] char *pString    -String
 *    [in] int nUpDown      -Sorts in "1"-ascending, "-1"-descending
 *    [in] int nBytes       -Size of the structure
 *
 *Returns: 0 on success
 *         1 on wrong index
 *         2 on memory allocating error
 *Note:
 *  StackPushSortA uses StackInsertBefore
 ********************************************************************/
#if defined StackPushSortA || defined ALLSTACKFUNC
#define StackPushSortA_INCLUDED
#undef StackPushSortA
__inline int StackPushSortA(stackS **first, stackS **last, stackS **element, char *pString, int nUpDown, int nBytes)
{
  stackS *tmp;
  int i;

  if (nUpDown != 1 && nUpDown != -1) return 1;

  for (tmp=*first; tmp; tmp=tmp->next)
  {
    i=lstrcmpiA(tmp->string, pString);

    if (i == 0 || i == nUpDown)
      break;
  }
  return StackInsertBefore((stack **)first, (stack **)last, (stack *)tmp, (stack **)element, nBytes);
}
#endif

/********************************************************************
 *
 *  StackSortA
 *
 *Sorts stack alphabetically in ascending or descending.
 *
 *[in,out] stackS **first   -Pointer to a pointer that specifies
 *                           the first element in the stack
 *[in,out] stackS **last    -Pointer to a pointer that specifies
 *                           the last element in the stack
 *    [in] int nUpDown      -Sorts in "1"-ascending, "-1"-descending
 *
 *Returns: 0 on success
 *         1 on wrong index
 *Note:
 *  StackSortA uses StackMoveBefore
 ********************************************************************/
#if defined StackSortA || defined ALLSTACKFUNC
#define StackSortA_INCLUDED
#undef StackSortA
__inline int StackSortA(stackS **first, stackS **last, int nUpDown)
{
  stackS *tmp1;
  stackS *tmp2;
  stackS *tmpNext;
  int i;

  if (nUpDown != 1 && nUpDown != -1) return 1;

  for (tmp1=*first; tmp1; tmp1=tmpNext)
  {
    tmpNext=tmp1->next;

    for (tmp2=*first; tmp2 != tmp1; tmp2=tmp2->next)
    {
      i=lstrcmpiA(tmp2->string, tmp1->string);

      if (i == 0 || i == nUpDown)
      {
        StackMoveBefore((stack **)first, (stack **)last, (stack *)tmp1, (stack *)tmp2);
        break;
      }
    }
  }
  return 0;
}
#endif


/********************************************************************
 *                                                                  *
 *                           Example                                *
 *                                                                  *
 ********************************************************************

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include "StackFunc.h"

//Include stack functions
#define StackInsertAfter
#define StackGetElement
#define StackDelete
#include "StackFunc.h"

//Structure initialization
typedef struct _TEXTITEM {
  struct _TEXTITEM *next;
  struct _TEXTITEM *prev;
  char szText[MAX_PATH];
  int nTextLen;
} TEXTITEM;

//Stack initialization
HSTACK hTextStack={0};

void main()
{
  TEXTITEM *lpElement;
  int nError;

  if (!StackInsertAfter((stack **)&hTextStack.first, (stack **)&hTextStack.last, (stack *)hTextStack.last, (stack **)&lpElement, sizeof(TEXTITEM)))
  {
    lstrcpyA(lpElement->szText, "some string");
    lpElement->nTextLen=lstrlenA(lpElement->szText);
  }
  if (!(nError=StackGetElement((stack *)hTextStack.first, (stack *)hTextStack.last, (stack **)&lpElement, -1)))
  {
    printf("element={%s}, error={%d}\n", lpElement->szText, nError);
    StackDelete((stack **)&hTextStack.first, (stack **)&hTextStack.last, (stack *)lpElement);
  }
}

*/


#endif  // _STACKFUNC_H_
