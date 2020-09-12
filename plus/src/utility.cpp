#include "../inc/utility.h"

#include <iostream>
using namespace std;



// int main(int argc, char const *argv[])
// {
// 	int a = 23400;

// 	LIMIT(a,0,4096);

// 	cout<< a << endl;
// 	return 0;
// }


class Interval{
public:

private:

};

/// Reverse the byte order for a 16 bit signed integer
short SwapShort(short a)
{
    return ((a>>8)&0xFF) | ((a&0xFF)<<8);
}


/// Reverse the byte order for a 16 bit unsigned integer
unsigned short SwapUshort(unsigned short a)
{
    return ((a>>8)&0xFF) | ((a&0xFF)<<8);
}


/// Reverse the byte order for a 32 bit signed integer
int SwapInt(int a)
{
    return ((a>>24)&0xFF) | ((a>>8)&0xFF00) | ((a<<8)&0xFF0000) | ((a<<24)&0xFF000000);
}


/// Reverse the byte order for a 32 bit unsigned integer
unsigned int SwapUint(unsigned int a)
{
    return ((a>>24)&0xFF) | ((a>>8)&0xFF00) | ((a<<8)&0xFF0000) | ((a<<24)&0xFF000000);
}

#ifdef __cplusplus
struct Note NOTATION(double x,size_t size)
#else
struct Note MK_NOTATION(double x,size_t size)
#endif
{
	struct Note res;
	int temp = 1;

	res.value = 0.0;
	res.E     = 0;
    
    if(x == 0.0)
        return res;

	while(--size)
		temp *= 10;

	if(x < temp){
		while(x < temp){
			x *= 10;
			res.E--;
		}
	}else{
		while(x > temp){
			x /= 10;
			res.E++;
		}
	}
    res.value = x;
	return res;
}


#ifdef __cplusplus
double NOTATION(struct Note& p)
#else
double RM_NOTATION(struct Note* p)
#endif
{

#ifdef __cplusplus
    int    E     = p.E;
    double value = p.value;
#else
    int    E     = p->E;
    double value = p->value;
#endif

    if(E > 0)
        while(E--)
            value*=10;
    else
        while(E++)
            value/=10;
    return value;
}

    
    





