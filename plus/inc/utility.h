
#ifndef _UTILITY_H
#define _UTILITY_H

#include <cstdint>
#include <cstddef>

/// @name Basic Macros
///
//@{

#define LIMIT(a, lowerbound, upperbound) 	(((a) >= (upperbound)) ? a=upperbound : (((a) <= (lowerbound)) ? a=lowerbound : a=a))
											
#ifndef ROLLOVER_INCREMENT
#define ROLLOVER_INCREMENT(a, upperbound) (((((a)+1) >= (upperbound)) ? 0 : (a)+1))
#endif
#ifndef ROLLOVER_DECREMENT
#define ROLLOVER_DECREMENT(a, upperbound) ((((a) <= 0) ? ((upperbound)-1) : (a)-1))
#endif
// Note, only safe where a+b is less than the type max
#ifndef ROLLOVER_ADD
#define ROLLOVER_ADD(a, b, upperbound) (((a)+(b)) >= (upperbound)) ? (((a)+(b))-(upperbound)) : ((a)+(b))
#endif
#ifndef SIGN
#define SIGN(x) ((x) >= 0 ? 1 : -1)
#endif

//@}


/// @name Multi-byte type extraction macros
///
/// These macros assist with extracting multibyte types like short, int, from an array of char
///
//@{

/// Make an unsigned short (16bits) from an array of char
///
/// \param __pCharArray is a pointer to the first byte in the array of char
/// \param __i is the index to start taking data from (least significant byte of the ushort)
#define makeUshort(__pCharArray, __i) ((unsigned short)((__pCharArray[(__i)+1]<<8)&0xFF00) | (__pCharArray[__i]&0x00FF) )

/// Make a short (16bits)from an array of char
///
/// \param __pCharArray is a pointer to the first byte in the array of char
/// \param __i is the index to start taking data from (least significant byte of the short)
#define makeShort(__pCharArray, __i) ( (short)((__pCharArray[(__i)+1]<<8)&0xFF00) | (__pCharArray[__i]&0x00FF) )

/// Make an unsigned int (32bits) from an array of char
///
/// \param __pCharArray is a pointer to the first byte in the array of char
/// \param __i is the index to start taking data from (least significant byte of the uint)
#define makeUint(__pCharArray, __i) ( (unsigned int)((__pCharArray[(__i)+3]<<24)&0xFF000000) | ((__pCharArray[(__i)+2]<<16)&0x00FF0000) \
                                 | ((__pCharArray[(__i)+1]<<8)&0x0000FF00) | ((__pCharArray[__i])&0x000000FF))
//@}

/// @name Bit masks
///
//@{
#define BIT_00 0x01
#define BIT_01 0x02
#define BIT_02 0x04
#define BIT_03 0x08
#define BIT_04 0x10
#define BIT_05 0x20
#define BIT_06 0x40
#define BIT_07 0x80
#define BIT_08 0x100
#define BIT_09 0x200
#define BIT_10 0x400
#define BIT_11 0x800
#define BIT_12 0x1000
#define BIT_13 0x2000
#define BIT_14 0x4000
#define BIT_15 0x8000
#define BIT_16 0x10000
#define BIT_17 0x20000
#define BIT_18 0x40000
#define BIT_19 0x80000
#define BIT_20 0x100000
#define BIT_21 0x200000
#define BIT_22 0x400000
#define BIT_23 0x800000
#define BIT_24 0x1000000
#define BIT_25 0x2000000
#define BIT_26 0x4000000
#define BIT_27 0x8000000
#define BIT_28 0x10000000
#define BIT_29 0x20000000
#define BIT_30 0x40000000
#define BIT_31 0x80000000

// 得到一个field在结构体(struct)中的偏移量
#define FPOS(type,field) ((dword)&((type *)0)->field)
// 得到一个结构体中field所占用的字节数
#define FSIZ(type,field) sizeof(((type *)0)->field)
// 按照LSB格式把两个字节转化为一个Word
#define FLIPW(ray) ((((uint16_t)(ray)[0]) << 8) + (ray)[1])

// 将一个字母转换为大写
#define UPCASE(c) (((c)>='a' && (c) <= 'z') ? ((c) – 0×20) : (c))

// 防止溢出的一个方法
#define INC_SAT(val) (val=((val)+1>(val)) ? (val)+1 : (val))
//@}



/// @name Multi-byte type extraction macros
///
/// These macros assist with extracting multibyte types like short, int, from an array of char
///
//@{

short SwapShort(short a);
unsigned short SwapUshort(unsigned short a);
int SwapInt(int a);
unsigned int SwapUint(unsigned int a);

// Performs an inplace reverse of the byte order of a block of memory.
void Reverse(void *pData, size_t len);

//@}

struct Note{
	double value;
	int    E;
};

#ifdef __cplusplus
struct Note NOTATION(double x,size_t size);
#else
struct Note MK_NOTATION(double x,size_t size);
#endif
#ifdef __cplusplus
double NOTATION(struct Note& p);
#else
double RM_NOTATION(struct Note* p);
#endif



#endif