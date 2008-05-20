/*****************************************************************
 *              Stack functions header v2.6                      *
 *                                                               *
 * 2008 Shengalts Aleksander aka Instructor (Shengalts@mail.ru)  *
 *                                                               *
 *                                                               *
 *Linear functions (ALLSTACKFUNCL):                              *
 * StackElementL, StackPushFrontL, StackPopFrontL,               *
 * StackPushBackL, StackSizeL, StackClearL                       *
 *                                                               *
 *Bilinear functions (ALLSTACKFUNC):                             *
 * StackElement, StackInsert, StackDelete, StackMove,            *
 * StackExchange, StackDeleteRange, StackMoveRange, StackJoin,   *
 * StackSplit, StackSize, StackClear                             *
 *                                                               *
 *Special functions (ALLSTACKFUNCS):                             *
 * StackReverse, StackCopy, StackPushSortA                       *
 *                                                               *
 *****************************************************************/

#ifndef _HSTACK_STRUCT_
#define _HSTACK_STRUCT_
typedef struct {
  int first;
  int last;
} HSTACK;
#endif

#ifndef _STACKL_STRUCT_
#define _STACKL_STRUCT_
typedef struct _stackL {
  struct _stackL *prev;
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

int StackElementL(stackL *first, stackL *last, stackL **element, int nIndex);
void StackPushFrontL(stackL **first, stackL **last, stackL **element, int nBytes);
int StackPopFrontL(stackL **first, stackL **last);
void StackPushBackL(stackL **first, stackL **last, stackL **element, int nBytes);
int StackSizeL(stackL *first, stackL *last);
void StackClearL(stackL **first, stackL **last);

int StackElement(stack *first, stack *last, stack **element, int nIndex);
int StackInsert(stack **first, stack **last, stack **element, int nIndex, int nBytes);
int StackDelete(stack **first, stack **last, stack *element);
int StackMove(stack **first, stack **last, stack *element, int nIndex);
int StackExchange(stack **first, stack **last, stack *element1, stack *element2);
int StackDeleteRange(stack **first, stack **last, int nIndex, int nIndex2);
int StackMoveRange(stack **first, stack **last, int nIndex, int nIndex2, int nIndex3);
void StackJoin(stack **first, stack **last, stack *joinfirst, stack *joinlast, BOOL bTop);
int StackSplit(stack **first, stack **last, stack **splitfirst, stack **splitlast, int nIndex);
int StackSize(stack *first, stack *last);
void StackClear(stack **first, stack **last);

void StackReverse(stack **first, stack **last);
int StackCopy(stack *first, stack *last, stack **copyfirst, stack **copylast, int nBytes);
int StackPushSortA(stackS **first, stackS **last, stackS **element, char *pString, int nUpDown, int nBytes);

#endif


/********************************************************************
 ********************************************************************
 *                                                                  *
 *                   Linear chain functions (+4)                    *
 *                                                                  *
 ********************************************************************
 ********************************************************************/


/********************************************************************
 *
 *  StackElementL
 *
 *Finds the element by index and returns pointer on it.
 *
 * [in] stackL *first    -Pointer to a pointer that specifies
 *                        the first element in the stack
 * [in] stackL *last     -Pointer to a pointer that specifies
 *                        the top element in the stack
 *[out] stackL **element -Pointer to a pointer to the element
 * [in] int nIndex       -Number of the element if positive search
 *                        from top if negative from beginning
 *
 *Returns: 0 on success
 *         1 on empty stack
 ********************************************************************/
#if defined StackElementL || defined ALLSTACKFUNCL
#define StackElementL_INCLUDED
#undef StackElementL
int StackElementL(stackL *first, stackL *last, stackL **element, int nIndex)
{
  stackL *tmp=last;
  int nCount;

  *element=NULL;

  for (nCount=1; (tmp); ++nCount)
  {
    if (nCount == nIndex)
    {
      *element=tmp;
      return 0;
    }
    tmp=tmp->prev;
  }
  return 1;
}
#endif

/********************************************************************
 *
 *  StackPushFrontL
 *
 *Adds an element to the top of the stack.
 *
 *[in,out] stackL **first   -Pointer to a pointer that specifies
 *                           the first element in the stack
 *[in,out] stackL **last    -Pointer to a pointer that specifies
 *                           the top element in the stack
 *   [out] stackL **element -Pointer to a pointer to the element
 *    [in] int nBytes       -Size of the structure
 ********************************************************************/
#if defined StackPushFrontL || defined ALLSTACKFUNCL
#define StackPushFrontL_INCLUDED
#undef StackPushFrontL
void StackPushFrontL(stackL **first, stackL **last, stackL **element, int nBytes)
{
  if (*element=(stackL *)GlobalAlloc(GPTR, nBytes))
  {
    if (*last)
      (*element)->prev=*last;
    else
      *first=*element;
    *last=*element;
  }
}
#endif

/********************************************************************
 *
 *  StackPopFrontL
 *
 *Removes the element from the top of the stack.
 *
 *[in,out] stackL **first   -Pointer to a pointer that specifies
 *                           the first element in the stack
 *[in,out] stackL **last    -Pointer to a pointer that specifies
 *                           the top element in the stack
 *
 *Returns: 0 on success
 *         1 on empty stack
 ********************************************************************/
#if defined StackPopFrontL || defined ALLSTACKFUNCL
#define StackPopFrontL_INCLUDED
#undef StackPopFrontL
int StackPopFrontL(stackL **first, stackL **last)
{
  stackL *tmp=*last;

  if (!*last) return 1;
  *last=tmp->prev;
  if (!*last) *first=NULL;
  GlobalFree((HGLOBAL)tmp);
  return 0;
}
#endif

/********************************************************************
 *
 *  StackPushBackL
 *
 *Adds an element to the beginning of the stack.
 *
 *[in,out] stackL **first   -Pointer to a pointer that specifies
 *                           the first element in the stack
 *[in,out] stackL **last    -Pointer to a pointer that specifies
 *                           the top element in the stack
 *   [out] stackL **element -Pointer to a pointer to the element
 *    [in] int nBytes       -Size of the structure
 ********************************************************************/
#if defined StackPushBackL || defined ALLSTACKFUNCL
#define StackPushBackL_INCLUDED
#undef StackPushBackL
void StackPushBackL(stackL **first, stackL **last, stackL **element, int nBytes)
{
  if (*element=(stackL *)GlobalAlloc(GPTR, nBytes))
  {
    if (*first)
      (*first)->prev=*element;
    else
      *last=*element;
    *first=*element;
  }
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
 *[in] stackL *last    -Pointer that specifies the top
 *                      element in the stack
 *
 *Returns: the number of elements
 ********************************************************************/
#if defined StackSizeL || defined ALLSTACKFUNCL
#define StackSizeL_INCLUDED
#undef StackSizeL
int StackSizeL(stackL *first, stackL *last)
{
  stackL *tmp=last;
  int nCount;

  for (nCount=0; (tmp); ++nCount)
    tmp=tmp->prev;
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
 *[in,out] stackL **last    -Pointer to a pointer that specifies
 *                           the top element in the stack
 ********************************************************************/
#if defined StackClearL || defined ALLSTACKFUNCL
#define StackClearL_INCLUDED
#undef StackClearL
void StackClearL(stackL **first, stackL **last)
{
  stackL *tmp=*last;
  stackL *tmp2;

  while (tmp)
  {
    tmp2=tmp->prev;
    GlobalFree((HGLOBAL)tmp);
    tmp=tmp2;
  }
  *last=NULL;
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
 *  StackElement
 *
 *Finds the element by index and returns pointer on it.
 *
 * [in] stack *first    -Pointer that specifies the first
 *                       element in the stack
 * [in] stack *last     -Pointer that specifies the top
 *                       element in the stack
 *[out] stack **element -Pointer to a pointer to the element
 * [in] int nIndex      -Number of the element if positive search
 *                       from top if negative from beginning
 *
 *Returns: 0 on success
 *         1 on wrong index
 ********************************************************************/
#if defined StackElement || defined ALLSTACKFUNC
#define StackElement_INCLUDED
#undef StackElement
int StackElement(stack *first, stack *last, stack **element, int nIndex)
{
  stack *tmp;
  int nCount;

  *element=NULL;

  if (nIndex > 0)
  {
    tmp=last;
    nCount=1;
  }
  else if (nIndex < 0)
  {
    tmp=first;
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
      tmp=tmp->prev;
      ++nCount;
    }
    else
    {
      tmp=tmp->next;
      --nCount;
    }
  }
  return 1;
}
#endif

/********************************************************************
 *
 *  StackInsert
 *
 *Finds the element by index and inserts new element in it index.
 *
 *[in,out] stack **first   -Pointer to a pointer that specifies
 *                          the first element in the stack
 *[in,out] stack **last    -Pointer to a pointer that specifies
 *                          the top element in the stack
 *   [out] stack **element -Pointer to a pointer to the element
 *    [in] int nIndex      -Number of the element if positive search
 *                          from top if negative from beginning
 *    [in] int nBytes      -Size of the structure
 *
 *Returns: 0 on success
 *         1 on wrong index
 *         2 on memory allocating error
 ********************************************************************/
#if defined StackInsert || defined ALLSTACKFUNC
#define StackInsert_INCLUDED
#undef StackInsert
int StackInsert(stack **first, stack **last, stack **element, int nIndex, int nBytes)
{
  stack *tmp;
  int nCount;

  *element=NULL;

  if (nIndex > 0)
  {
    tmp=*last;
    nCount=1;
  }
  else if (nIndex < 0)
  {
    tmp=*first;
    nCount=-1;
  }
  else return 1;

  while ((tmp) || (nCount == nIndex))
  {
    if (nCount == nIndex)
    {
      if (*element=(stack *)GlobalAlloc(GPTR, nBytes))
      {
        if (!tmp)
        {
          if (nIndex > 0)
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
        }
        else if (nIndex > 0)
        {
          if (tmp == *last) *last=*element;
          else tmp->next->prev=*element;

          (*element)->prev=tmp;
          (*element)->next=tmp->next;
          tmp->next=*element;
        }
        else
        {
          if (tmp == *first) *first=*element;
          else tmp->prev->next=*element;

          (*element)->next=tmp;
          (*element)->prev=tmp->prev;
          tmp->prev=*element;
        }
      }
      else return 2;

      return 0;
    }
    if (nIndex > 0)
    {
      tmp=tmp->prev;
      ++nCount;
    }
    else
    {
      tmp=tmp->next;
      --nCount;
    }
  }
  return 1;
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
 *                          the top element in the stack
 *    [in] stack *element  -Pointer to the element
 *
 *Returns: 0 on success
 *         1 on empty stack
 ********************************************************************/
#if defined StackDelete || defined ALLSTACKFUNC
#define StackDelete_INCLUDED
#undef StackDelete
int StackDelete(stack **first, stack **last, stack *element)
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
 *  StackMove
 *
 *Finds the element by index and move it to the new index.
 *
 *[in,out] stack **first   -Pointer to a pointer that specifies
 *                          the first element in the stack
 *[in,out] stack **last    -Pointer to a pointer that specifies
 *                          the top element in the stack
 *    [in] stack *element  -Pointer to the element (source)
 *    [in] int nIndex      -Number of the element if positive search
 *                          from top if negative from beginning (destination)
 *
 *Returns: 0 on success
 *         1 on empty stack
 *         2 index pointed to the same element
 ********************************************************************/
#if defined StackMove || defined ALLSTACKFUNC
#define StackMove_INCLUDED
#undef StackMove
int StackMove(stack **first, stack **last, stack *element, int nIndex)
{
  stack *tmp;
  int nCount;

  if (nIndex > 0)
  {
    tmp=*last;
    nCount=1;
  }
  else if (nIndex < 0)
  {
    tmp=*first;
    nCount=-1;
  }
  else return 1;

  while ((tmp) || (nCount == nIndex))
  {
    if (nCount == nIndex)
    {
      if (element == tmp) return 2;

      //Unlink element
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
      element->next=NULL;
      element->prev=NULL;

      //Insert element
      if (!tmp)
      {
        if (nIndex > 0)
        {
          if (*first)
          {
            (*first)->prev=element;
            element->next=*first;
          }
          else
          {
            *last=element;
          }
          *first=element;
        }
        else
        {
          if (*last)
          {
            (*last)->next=element;
            element->prev=*last;
          }
          else
          {
            *first=element;
          }
          *last=element;
        }
      }
      else if (nIndex > 0)
      {
        if (tmp == *last) *last=element;
        else tmp->next->prev=element;

        element->prev=tmp;
        element->next=tmp->next;
        tmp->next=element;
      }
      else
      {
        if (tmp == *first) *first=element;
        else tmp->prev->next=element;

        element->next=tmp;
        element->prev=tmp->prev;
        tmp->prev=element;
      }
      return 0;
    }
    if (nIndex > 0)
    {
      if (element != tmp) ++nCount;
      tmp=tmp->prev;
    }
    else
    {
      if (element != tmp) --nCount;
      tmp=tmp->next;
    }
  }
  return 1;
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
 *                          the top element in the stack
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
int StackExchange(stack **first, stack **last, stack *element1, stack *element2)
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
 *  StackDeleteRange
 *
 *Finds the elements between indexes and removes.
 *
 *[in,out] stack **first   -Pointer to a pointer that specifies
 *                          the first element in the stack
 *[in,out] stack **last    -Pointer to a pointer that specifies
 *                          the top element in the stack
 *    [in] int nIndex      -Number of the element if positive search
 *                          from top if negative from beginning (range limit)
 *    [in] int nIndex2     -Number of the element if positive search
 *                          from top if negative from beginning (range limit)
 *
 *Returns: 0 on success
 *         1 on empty stack
 ********************************************************************/
#if defined StackDeleteRange || defined ALLSTACKFUNC
#define StackDeleteRange_INCLUDED
#undef StackDeleteRange
int StackDeleteRange(stack **first, stack **last, int nIndex, int nIndex2)
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
    tmp=*last;
    nCount=1;
  }
  else if (nIndex < 0)
  {
    tmp=*first;
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
      if ((bMeet == FALSE && nIndex < 0) || (bMeet == TRUE && nIndex > 0))
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
      tmp=tmp->prev;
      ++nCount;
    }
    else
    {
      tmp=tmp->next;
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
 *                          the top element in the stack
 *    [in] int nIndex      -Number of the element if positive search
 *                          from top if negative from beginning (range limit)
 *    [in] int nIndex2     -Number of the element if positive search
 *                          from top if negative from beginning (range limit)
 *    [in] int nIndex3     -Number of the element if positive search
 *                          from top if negative from beginning (destination)
 *
 *Returns: 0 on success
 *         1 on empty stack
 *         2 destination index pointed to the element in the range
 ********************************************************************/
#if defined StackMoveRange || defined ALLSTACKFUNC
#define StackMoveRange_INCLUDED
#undef StackMoveRange
int StackMoveRange(stack **first, stack **last, int nIndex, int nIndex2, int nIndex3)
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
    tmp=*last;
    nCount=1;
  }
  else if (nIndex < 0)
  {
    tmp=*first;
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

      if ((bMeet == FALSE && nIndex2 < 0) || (bMeet == TRUE && nIndex2 > 0))
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

      if ((nMeet == 0 && nIndex < 0) || (nMeet == 3 && nIndex > 0))
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
      tmp=tmp->prev;
      ++nCount;
    }
    else
    {
      tmp=tmp->next;
      --nCount;
    }
  }
  return 1;
}
#endif

/********************************************************************
 *
 *  StackJoin
 *
 *Joins two stacks.
 *
 *[in,out] stack **first     -Pointer to a pointer that specifies
 *                            the first element in the stack
 *[in,out] stack **last      -Pointer to a pointer that specifies
 *                            the top element in the stack
 *    [in] stack *joinfirst  -Pointer to the first element in the stack
 *    [in] stack *joinlast   -Pointer to the top element in the stack
 *    [in] BOOL bTop         -If TRUE second stack will be joined
 *                            to the top of the first stack.
 *                            If FALSE second stack will be joined
 *                            to the beginning of the first stack.
 ********************************************************************/
#if defined StackJoin || defined ALLSTACKFUNC
#define StackJoin_INCLUDED
#undef StackJoin
void StackJoin(stack **first, stack **last, stack *joinfirst, stack *joinlast, BOOL bTop)
{
  if (!*first)
  {
    *first=joinfirst;
    *last=joinlast;
  }
  else if (joinfirst)
  {
    if (bTop)
    {
      (*last)->next=joinfirst;
      joinfirst->prev=*last;
      *last=joinlast;
    }
    else
    {
      (*first)->prev=joinlast;
      joinlast->next=*first;
      *first=joinfirst;
    }
  }
}
#endif

/********************************************************************
 *
 *  StackSplit
 *
 *Splits stack.
 *
 *[in,out] stack **first       -Pointer to a pointer that specifies
 *                              the first element in the stack
 *[in,out] stack **last        -Pointer to a pointer that specifies
 *                              the top element in the stack
 *   [out] stack **splitfirst  -Pointer to a pointer that specifies
 *                              the first element in the stack
 *   [out] stack **splitlast   -Pointer to a pointer that specifies
 *                              the top element in the stack
 *    [in] int nIndex          -Number of the element if positive search
 *                              from top if negative from beginning.
 *                              After spliting this element will be
 *                              the last element of the first stack.
 *
 *Returns: 0 on success
 *         1 on empty stack
 ********************************************************************/
#if defined StackSplit || defined ALLSTACKFUNC
#define StackSplit_INCLUDED
#undef StackSplit
int StackSplit(stack **first, stack **last, stack **splitfirst, stack **splitlast, int nIndex)
{
  stack *tmp;
  int nCount;

  if (nIndex > 0)
  {
    tmp=*last;
    nCount=1;
  }
  else if (nIndex < 0)
  {
    tmp=*first;
    nCount=-1;
  }
  else return 1;

  while (tmp)
  {
    if (nCount == nIndex)
    {
      if (tmp == *last)
      {
        *splitfirst=NULL;
        *splitlast=NULL;
      }
      else
      {
        *splitfirst=tmp->next;
        *splitlast=*last;
        *last=tmp;
        (*splitfirst)->prev=NULL;
        (*last)->next=NULL;
      }
      return 0;
    }
    if (nIndex > 0)
    {
      tmp=tmp->prev;
      ++nCount;
    }
    else
    {
      tmp=tmp->next;
      --nCount;
    }
  }
  return 1;
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
 *[in] stack *last    -Pointer that specifies the top
 *                     element in the stack
 *
 *Returns: the number of elements
 ********************************************************************/
#if defined StackSize || defined ALLSTACKFUNC
#define StackSize_INCLUDED
#undef StackSize
int StackSize(stack *first, stack *last)
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
 *                          the top element in the stack
 ********************************************************************/
#if defined StackClear || defined ALLSTACKFUNC
#define StackClear_INCLUDED
#undef StackClear
void StackClear(stack **first, stack **last)
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
 *  StackReverse
 *
 *Reverse stack.
 *
 *[in,out] stack **first   -Pointer to a pointer that specifies
 *                          the first element in the stack
 *[in,out] stack **last    -Pointer to a pointer that specifies
 *                          the top element in the stack
 *
 *Note:
 *  StackReverse uses StackExchange
 ********************************************************************/
#if (defined StackReverse || defined ALLSTACKFUNCS) && defined StackExchange_INCLUDED
#define StackReverse_INCLUDED
#undef StackReverse
void StackReverse(stack **first, stack **last)
{
  stack *tmp1=*first;
  stack *tmp2=*last;
  stack *tmpNext;
  stack *tmpPrev;
  BOOL bBreak=FALSE;

  while (1)
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
 *                            element in the stack
 *    [in] stack *last       -Pointer that specifies the top
 *                            element in the stack
 *[in,out] stack **copyfirst -Pointer to a pointer that specifies
 *                            the first element in the stack
 *[in,out] stack **copylast  -Pointer to a pointer that specifies
 *                            the top element in the stack
 *    [in] int nBytes        -Size of the structure
 *
 *Returns: 0 on success
 *         1 on wrong index
 *         2 on memory allocating error
 *Note:
 *  StackCopy uses StackInsert
 ********************************************************************/
#if (defined StackCopy || defined ALLSTACKFUNCS) && defined StackInsert_INCLUDED
#define StackCopy_INCLUDED
#undef StackCopy
int StackCopy(stack *first, stack *last, stack **copyfirst, stack **copylast, int nBytes)
{
  stack *tmp1=first;
  stack *tmp2;
  unsigned char *lpData1;
  unsigned char *lpData2;
  int nDataBytes;
  int nResult=0;

  while (tmp1)
  {
    if (!(nResult=StackInsert(copyfirst, copylast, &tmp2, -1, nBytes)))
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
 *                           the top element in the stack
 *   [out] stackS **element -Pointer to a pointer to the element
 *    [in] char *pString    -String
 *    [in] int nUpDown      -Sorts in "1"-ascending, "-1"-descending
 *    [in] int nBytes       -Size of the structure
 *
 *Returns: 0 on success
 *         1 on wrong index
 *         2 on memory allocating error
 *Note:
 *  StackPushSortA uses StackInsert
 ********************************************************************/

#if (defined StackPushSortA || defined ALLSTACKFUNCS) && defined StackInsert_INCLUDED
#define StackPushSortA_INCLUDED
#undef StackPushSortA
int StackPushSortA(stackS **first, stackS **last, stackS **element, char *pString, int nUpDown, int nBytes)
{
  stackS *tmp=*last;
  int nIndex;
  int i;
  int nResult;

  if (nUpDown != 1 && nUpDown != -1) return 1;

  for (nIndex=1; (tmp); ++nIndex)
  {
    i=lstrcmpiA(tmp->string, pString);
    if (i == 0 || i == nUpDown) break;

    tmp=tmp->prev;
  }

  if (!(nResult=StackInsert((stack **)first, (stack **)last, (stack **)element, nIndex, nBytes)))
  {
    lstrcpynA((*element)->string, pString, MAX_PATH);
  }
  return nResult;
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
#define StackInsert
#define StackElement
#define StackDelete
#include "StackFunc.h"

//Structure initialization
typedef struct _HTEXTSTACK {
  struct _HTEXTSTACK *next;
  struct _HTEXTSTACK *prev;
  char szText[MAX_PATH];
  DWORD dwLength;
} HTEXTSTACK;

//Stack initialization
HSTACK hTextStack={0};

void main()
{
  HTEXTSTACK *lpElement;
  int nError;

  if (!StackInsert((stack **)&hTextStack.first, (stack **)&hTextStack.last, (stack **)&lpElement, 1, sizeof(HTEXTSTACK)))
  {
    lstrcpyA(lpElement->szText, "some string");
    lpElement->dwLength=lstrlenA(lpElement->szText);
  }
  if (!(nError=StackElement((stack *)hTextStack.first, (stack *)hTextStack.last, (stack **)&lpElement, 1)))
  {
    printf("element={%s}, error={%d}\n", lpElement->szText, nError);
    StackDelete((stack **)&hTextStack.first, (stack **)&hTextStack.last, (stack *)lpElement);
  }
}

*/
