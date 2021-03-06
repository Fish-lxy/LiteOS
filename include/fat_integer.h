#ifndef _FAT_INTEGER_H

/* These types must be 16-bit, 32-bit or larger integer */
typedef signed int		INT;
typedef unsigned int	UINT;

/* These types must be 8-bit integer */
typedef signed char		CHAR;
typedef unsigned char	UCHAR;
typedef unsigned char	BYTE;

/* These types must be 16-bit integer */
typedef signed short	SHORT;
typedef unsigned short	USHORT;
typedef unsigned short	WORD;

/* These types must be 32-bit integer */
typedef signed long		LONG;
typedef unsigned long	ULONG;
typedef unsigned long	DWORD;

/* Boolean type */
typedef int BOOL;

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#define _FAT_INTEGER_H
#endif
