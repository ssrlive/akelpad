#include "XMemStrFunc.h"

// Special form of memcpy implementation to avoid
// compiler from replace this code with memcpy call.
void x_mem_cpy(void *pDest, const void *pSrc, UINT_PTR nBytes)
{
    if ( pDest != pSrc )
    {
        unsigned int* pDestUint = (unsigned int *) pDest;
        const unsigned int* pSrcUint = (const unsigned int *) pSrc;

        if ( nBytes >= sizeof(unsigned int) )
        {
            for ( ; ; )
            {
                *pDestUint = *pSrcUint;
                nBytes -= sizeof(unsigned int);
                if ( nBytes < sizeof(unsigned int) )
                {
                    pDest = NULL;
                    break;
                }
                ++pDestUint;
                ++pSrcUint;
            }
        }

        if ( nBytes != 0 )
        {
            unsigned char* pDestByte;
            const unsigned char* pSrcByte;

            if ( pDest == NULL )
            {
                ++pDestUint;
                ++pSrcUint;
            }
            pDestByte = (unsigned char *) pDestUint;
            pSrcByte = (const unsigned char *) pSrcUint;

            for ( ; ; )
            {
                *pDestByte = *pSrcByte;
                if ( --nBytes == 0 )
                    break;
                ++pDestByte;
                ++pSrcByte;
            }
        }
    }
}

// Special form of memset implementation to avoid
// compiler from replace this code with memset call.
void x_mem_set(void *pDest, unsigned int c, UINT_PTR nBytes)
{
    if ( nBytes != 0 )
    {
        unsigned char *pDestByte = (unsigned char *) pDest;

        for ( ; ; )
        {
            *pDestByte = (unsigned char) c;
            if ( --nBytes == 0 )
                break;
            ++pDestByte;
        }
    }
}

// Special form of ZeroMemory implementation to avoid
// compiler from replace this code with memset call
void x_zero_mem(void* pDest, UINT_PTR nBytes)
{
    unsigned int* pDestUint = (unsigned int *) pDest;

    if ( nBytes >= sizeof(unsigned int) )
    {
        for ( ; ; )
        {
            *pDestUint = 0;
            nBytes -= sizeof(unsigned int);
            if ( nBytes < sizeof(unsigned int) )
            {
                pDest = NULL;
                break;
            }
            ++pDestUint;
        }
    }

    if ( nBytes != 0 )
    {
        unsigned char *pDestByte;

        if ( pDest == NULL )
        {
            ++pDestUint;
        }
        pDestByte = (unsigned char *) pDestUint;

        for ( ; ; )
        {
            *pDestByte = 0;
            if ( --nBytes == 0 )
                break;
            ++pDestByte;
        }
    }
}

void* x_mem_alloc(unsigned int nSizeInBytes)
{
    return SysMemAlloc(nSizeInBytes);
}

void  x_mem_free(void* ptr)
{
    SysMemFree(ptr);
}

BOOL x_wstr_startswith(const WCHAR* str, const WCHAR* substr)
{
    for ( ; ; )
    {
        if ( *substr == 0 )
            break; // end of substr - OK

        if ( *substr != *str )
            return FALSE; // diff

        ++substr;
        ++str;
    }

    return TRUE;
}

BOOL x_wstr_endswith(const WCHAR* str, int nStrLen, const WCHAR* substr, int nSubstrLen)
{
    if ( nSubstrLen > nStrLen )
        return FALSE;
    
    str += (nStrLen - nSubstrLen);
    for ( ; ; )
    {
        if ( *str != *substr )
            return FALSE; // diff

        if ( *str == 0 )
            break; // end of both strings - OK

        ++str;
        ++substr;
    }

    return TRUE;
}

int x_wstr_findch(const WCHAR* str, WCHAR ch, int nOffset)
{
    const WCHAR* p;
    WCHAR cc;

    for ( p = str + nOffset; (cc = *p) != 0; ++p )
    {
        if ( cc == ch )
            return (int) (p - str);
    }

    return -1; // not found
}

#if !defined(xatoiW_INCLUDED)
INT_PTR xatoiW(const wchar_t *wpStr, const wchar_t **wpNext)
{
    INT_PTR nNumber = 0;
    BOOL bMinus = FALSE;

    while (*wpStr == ' ' || *wpStr == '\t')
        ++wpStr;
    if (*wpStr == '+')
        ++wpStr;
    else if (*wpStr == '-')
    {
        bMinus = TRUE;
        ++wpStr;
    }

    while (*wpStr >= '0' && *wpStr <= '9')
    {
        nNumber = (nNumber * 10) + (*wpStr - '0');
        ++wpStr;
    }
    if (bMinus == TRUE) nNumber = 0 - nNumber;
    if (wpNext) *wpNext = wpStr;
    return nNumber;
}
#endif // !defined(xatoiW_INCLUDED)

#if !defined(xitoaW_INCLUDED)
int xitoaW(INT_PTR nNumber, wchar_t *wszStr)
{
    wchar_t wszReverse[128];
    int a;
    int b = 0;

    if (nNumber == 0)
    {
        if (wszStr) wszStr[b] = L'0';
        ++b;
    }
    else if (nNumber < 0)
    {
        if (wszStr) wszStr[b] = L'-';
        ++b;
        nNumber = 0 - nNumber;
    }
    for (a = 0; nNumber != 0; ++a)
    {
        wszReverse[a] = (wchar_t) (nNumber % 10) + L'0';
        nNumber = nNumber / 10;
    }
    if (!wszStr) return a + b + 1;

    while (--a >= 0) wszStr[b++] = wszReverse[a];
    wszStr[b] = L'\0';
    return b;
}
#endif // !defined(xitoaW_INCLUDED)

// tDynamicBuffer
void tDynamicBuffer_Init(tDynamicBuffer* pBuf)
{
    x_zero_mem( pBuf, sizeof(tDynamicBuffer) );
}

void tDynamicBuffer_Free(tDynamicBuffer* pBuf)
{
    if ( pBuf->ptr )
    {
        SysMemFree( pBuf->ptr );
        x_zero_mem( pBuf, sizeof(tDynamicBuffer) );
    }
}

BOOL tDynamicBuffer_Allocate(tDynamicBuffer* pBuf, UINT_PTR nBytesToAllocate)
{
    if ( pBuf->nBytesAllocated < nBytesToAllocate )
    {
        // allocate new memory
        tDynamicBuffer newBuf;

        tDynamicBuffer_Init(&newBuf);
        newBuf.ptr = SysMemAlloc( nBytesToAllocate );
        if ( !newBuf.ptr )
            return FALSE; // failed to allocate the memory

        newBuf.nBytesAllocated = nBytesToAllocate;
        
        // free the previously allocated memory
        if ( pBuf->ptr )
            SysMemFree( pBuf->ptr );

        // use the newly allocated memory
        x_mem_cpy( pBuf, &newBuf, sizeof(tDynamicBuffer) );
    }

    return TRUE; // OK
}

UINT_PTR tDynamicBuffer_Append(tDynamicBuffer* pBuf, const void* pData, UINT_PTR nBytes)
{
    UINT_PTR nBytesToStore;

    if ( nBytes == 0 )
        return pBuf->nBytesStored;

    nBytesToStore = pBuf->nBytesStored + nBytes;
    if ( nBytesToStore > pBuf->nBytesAllocated )
    {
        // allocate new memory
        tDynamicBuffer newBuf;
        UINT_PTR nBytesToAllocate;

        nBytesToAllocate = pBuf->nBytesAllocated;
        if ( nBytesToAllocate == 0 )
        {
            nBytesToAllocate = 64;
        }
        else if ( (nBytesToAllocate % 64) != 0 )
        {
            nBytesToAllocate = (1 + nBytesToAllocate/64)*64;
        }
        nBytesToAllocate = (1 + nBytesToStore/nBytesToAllocate)*nBytesToAllocate;
        tDynamicBuffer_Init(&newBuf);
        if ( tDynamicBuffer_Allocate(&newBuf, nBytesToAllocate) != 0 )
        {
            unsigned char* p = (unsigned char *) newBuf.ptr;
            // copy the previous data
            if ( pBuf->nBytesStored != 0 )
            {
                x_mem_cpy( p, pBuf->ptr, pBuf->nBytesStored );
                p += pBuf->nBytesStored;
            }
            // append the new data
            x_mem_cpy(p, pData, nBytes);

            // free the previously allocated memory
            if ( pBuf->ptr )
                SysMemFree( pBuf->ptr );

            // use the newly allocated memory
            x_mem_cpy( pBuf, &newBuf, sizeof(tDynamicBuffer) );
        }
        else
            return 0; // failed to allocate the memory
    }
    else
    {
        // use the existing memory
        unsigned char* p = (unsigned char *) pBuf->ptr;
        p += pBuf->nBytesStored;
        // append the new data
        x_mem_cpy(p, pData, nBytes);
    }

    pBuf->nBytesStored = nBytesToStore;
    return nBytesToStore; // OK
}

void tDynamicBuffer_Clear(tDynamicBuffer* pBuf)
{
    pBuf->nBytesStored = 0;
}
