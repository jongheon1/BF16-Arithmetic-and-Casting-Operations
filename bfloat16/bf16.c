#include <stdio.h>
#include "bf16.h"

/*
    Do NOT include any C libraries or header files
    except those above
    
    use the defines in bf16.h
    if neccessary, you can add some macros below
*/

bf16 int2bf16 (int input) {
    bf16 result = 0;
    /*
    fill this function
    */
    unsigned int intBits  = (unsigned int)input;
    bf16 sign = (bf16)((intBits >> 31) & 0x1);
    if(sign) intBits = ~intBits + 1;
    bf16 exponent = 0;
    for (int i = 31; i >= 0; i--){
	    if(intBits & (1 << i)) {
		    exponent = i;
		    break;
	    }
    }
    bf16 fraction;
    if (exponent <= 7){
	    intBits ^= (1 << exponent);
	    fraction = (bf16) (intBits << (7 - exponent));
    } else {
	    int G, R, S, up;
	    G = intBits & (1 << (exponent - 7));
	    R = intBits & (1 << (exponent - 8));
	    if(exponent > 8){
		    S = intBits & ((1 << (exponent - 8)) - 1);
		    if (R && (G || S)) up = 1;
		    else up = 0;
	    } else {
		    if (G && R) up = 1;
		    else up = 0;
	    }
	    if (up){
		    intBits += (1 << (exponent - 8));
	    }
	    fraction = (bf16)((intBits >> (exponent - 7)) & 0x7F);

    }
    result = (sign << 15) | (exponent + 127  << 7) | fraction;


    return result;
}

int bf162int (bf16 input) {
    int result = 0;
    /*
    fill this function
    */
    unsigned int sign = input & (1 << 15);
    int exponent = (int)((input >> 7) & 0xFF);
    int fraction = (int)(input & 0x7F);
    if (exponent == 0){
	    result = 0;
    } else if (~exponent == 0){
	    if (fraction == 0 && sign == 0) result = 0x7FFFFFFF;
	    else result = 0x80000000;
    } else {
	    exponent -= 127;
	    if (exponent >= 31){
		    if (sign == 0) result = 0x7FFFFFFF;
		    else result = 0x80000000;
	    } else if (exponent >= 7){
		    exponent -= 7;
		    result = (fraction | 0x80) << exponent;
	    } else {
		    int G, R, S, up;
		    G = fraction & (1 << (7 - exponent));
		    R = fraction & (1 << (6 - exponent));
		    if (exponent <= 5) {
			    S = fraction & ((1 << (6 - exponent)) - 1);
			    if (R && (G || S)) up = 1;
			    else up = 0;
		    } else {
			    if (G && R) up = 1;
			    else up = 0;
		    }
		    if (up){
			    fraction += (1 << (6 - exponent));
		    }
		    exponent = 7 - exponent;
		    result = (fraction + 0x80) >> exponent;
		    if (sign) result = ~result + 1;
	    }
    }
    return result;
}

bf16 float2bf16 (float input) {
    bf16 result = 0;
    /*
    fill this function
    */
    union fu {
	    float f;
	    unsigned u;
    } bits;
    bits.f = input;
    unsigned sign = bits.u >> 31;
    unsigned exponent = (bits.u >> 23) & 0xFF;
    unsigned fraction = (bits.u & 0x7FFFFF);

    int G, R, S, up;
    G = fraction & (1 << 16);
    R = fraction & (1 << 15);
    S = fraction & ((1 << 15) - 1);
    if (R && (G || S)) up = 1;
    else up = 0;
    
    if(up) fraction += (1 << 15);
    result = (sign << 15) | (exponent << 7) | (fraction >> 16);

    return result;
}

float bf162float (bf16 input) {
    float result = 0.0;
    /*
    fill this function
    */
    union fu {
	    float f;
	    unsigned u;
    } bits;
    
    unsigned sign = input >> 15;
    unsigned exponent = (input >> 7) & 0xFF;
    unsigned fraction = input & 0x7F;

    bits.u = (sign << 31) | (exponent << 23) | (fraction << 16);
    result = bits.f;
    return result;
}

bf16 bf16_add(bf16 in1, bf16 in2) {
    bf16 result = 0;
    /*
    fill this function
    */
    bf16 sign1 = in1 >> 15;
    bf16 exp1 = (in1 >> 7) & 0xFF;
    bf16 frac1 = in1 & 0x7F;

    bf16 sign2 = in2 >> 15;
    bf16 exp2 = (in2 >> 7) & 0xFF;
    bf16 frac2 = in2 & 0x7F;

    bf16 sign, exp, frac;
    if (exp1 < exp2) {
	    bf16 tsign, texp, tfrac;
	    tsign = sign2, texp = exp2, tfrac = frac2;
	    sign2 = sign1, exp2 = exp1, frac2 = frac1;
	    sign1 = tsign, exp1 = texp, frac1 = tfrac;
    }

    if (~exp1 == 0) {
	    if (frac1 == 0) {
		    if (sign1 == 0) {
			    if ((~exp2 == 0 && sign2 == 0 && frac2 == 0) || ~exp2 != 0) {
				    result = 0x7F80; //+
			    } else  result = 0x7FF0; //Nan
		    } else {
			    if ((~exp2 == 0 && sign2 == 1 && frac2 == 0) || ~exp2 != 0) {
				    result = 0xFF80; //-
			    } else result = 0xFFF0; //NaN
		    }
	    } else result = 0xFFF0; //NaN
    } else {
	    if (~exp2 == 0 && frac2 == 0){
		    if (sign2 == 0) result = 0x7F80;
		    else result = 0xFF80;
	    } else if (~exp2 == 0 && frac2 != 0) result = 0xFFF0;

	    else { // normal value
		    if (exp1 > 0) frac1 |= 0x80;
		    if (exp2 > 0) frac2 |= 0x80;
		    
		    sign = sign1;
		    exp = exp1;
		    int d = exp1 - exp2;
		    if (d >= 9) {
			    result = (sign1 << 15) | (exp1 << 7) | frac1;
		    } else {
			    if (sign1 ^ sign2) frac = ((frac1 << d) - frac2) << 4;
			    else frac = ((frac1 << d) + frac2) << 4;

			    if (frac >= (1 << (8 + d + 4))) {
				    frac = frac >> 1;
				    exp += 1;
			    }
			    while (frac < (1 << (7 + d+ 4)) && exp > 0){
				    frac = frac << 1;
				    exp -= 1;
			    }

			    if(exp1 - exp <= 0) {
				    int G, R, S, up;
				    G = frac & (1 << (d + exp1 - exp + 4));
				    R = frac & (1 << (d - 1 + exp1 - exp + 4));
				    S = frac & ((1 << (d - 1 + exp1 - exp + 4)) - 1);
				    
				    if (R && (G || S)) up = 1;
				    else up = 0;
				   
				    if (up) frac += (1 << (d - 1 + exp1 - exp + 4));
			    }
			    frac = frac >> (d + 4);
			    if (exp == 0xFF) frac = 0;
			    if (exp != 0) frac &= ~0x80;
			    result = (sign << 15) | (exp << 7) | frac;
		    
		    }
	    }
    }
    return result;
}

bf16 bf16_mul(bf16 in1, bf16 in2) {
    bf16 result = 0;
    /*
    fill this function
    */

    bf16 sign1 = in1 >> 15;
    bf16 exp1 = (in1 >> 7) & 0xFF;
    bf16 frac1 = in1 & 0x7F;

    bf16 sign2 = in2 >> 15;
    bf16 exp2 = (in2 >> 7) & 0xFF;
    bf16 frac2 = in2 & 0x7F;
    bf16 sign, exp, frac;

    if ((sign1 ^ sign2) && (~exp1 == 0 || ~exp2 == 0)) result = 0xFF80;
    else if (!(sign1 ^ sign2) && (~exp1 == 0 || ~exp2 == 0))  result = 0x7F80;
    else if ((~exp1 == 0 && in2 == 0) || (~exp2 == 0 && in1 == 0) || (~exp1 == 0 && frac1 != 0) || (~exp2 == 0 && frac2 != 0)) result = 0x7FF0;
    else if (exp1 + exp2 > 509)  result = ((sign1 ^ sign2) << 15) | 0x7F80;
    else {
	    sign = sign1 ^ sign2;
	    exp = exp1 + exp2;
	    if (exp1 > 0) frac1 |= 0x80;
	    if (exp2 > 0) frac2 |= 0x80;
	    frac = (frac1 * frac2);
	    if (frac >= (1 << 8 + 7)) {
		    frac = frac >> 1;
		    exp += 1;
	    }
	    while (frac < (1 << 7)) {
		    frac = frac << 1;
		    exp -= 1;
	    }
	    int G, R, S, up;
	    G = frac & (1 << 7);
	    R = frac & (1 << 6);
	    S = frac & ((1 << 6) - 1);
	    if (R && (G || S)) up = 1;
	    else up = 0;
	    
	    if (up) frac += 1 << 6;
	    frac = frac >> 7;

	    if (exp > 509) result = (sign << 15) | 0x7F80;
	    else {
		    if (exp != 0) frac &= ~0x80;
		    result = (sign << 15) | ((exp - 127) << 7) | frac;
	    }

    }


    return result;

}
