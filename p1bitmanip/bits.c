/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * 
 * bits.c - Source file with your solutions to the project.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the project by
editing the collection of functions in this source file.

CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. The maximum number of ops for each function is given in the
     header comment for each function.

/*
 * STEP 2: Modify the following functions according to the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the btest checker to verify that your solutions produce
 *      the correct answers.
 */


#endif

/************************************************ 
 * absVal - absolute value of x
 *   Example: absVal(-1) = 1.
 *   You may assume -TMax <= x <= TMax
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 4
 */
int absVal(int x) {
  // for negative sign provide its twos compelemt +1 else just return given
  int is_sign_neg = x>>31;
  return ((is_sign_neg) & (~x+1)) | ( (~is_sign_neg) & x );
}

/************************************************ 
 * addOK - Determine if can compute x+y without overflow
 *   Example: addOK(0x80000000,0x80000000) = 0,
 *            addOK(0x80000000,0x70000000) = 1, 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int addOK(int x, int y) {
  int x_sign = x>>31;
  int y_sign = y>>31;
  int res_sign = (x+y)>>31;
  int sign_dif = x_sign ^ y_sign; //same -> 0000 different -> 1111
  // if signs of xy dif -> overflow cannot happen; if same sign -> happens if the sign of res is dif from input
  return !((~sign_dif)&(x_sign^res_sign))  ;
}

/************************************************ 
 * allEvenBits - return 1 if all even-numbered bits in word set to 1
 *   Examples allEvenBits(0xFFFFFFFE) = 0, allEvenBits(0x55555555) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allEvenBits(int x) {
  //repeatedly apply 0x55, if 0x55 comes out as a result
  int extractor = 0x55;
  int extraction =  x & extractor;
  extraction = (x>>8) & extraction;
  extraction = (x>>16) & extraction;
  extraction = (x>>24) & extraction;
  return !(extraction^extractor);
}

/************************************************
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>4
 *   Max ops: 12
 *   Rating: 4 
 */
int bang(int x) {
  int sign_comp =  (x|(~x+1))>>31; // produces 0000 only if both 0, other options using xor did not work from tmin case cauing circular results
  return ((~sign_comp)&1) | ((sign_comp)&0);
}

/************************************************
 * bitCount - returns count of number of 1's in word
 *   Examples: bitCount(5) = 2, bitCount(7) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 40
 *   Rating: 4
 */
int bitCount(int x) {
  /*
  int extractor = 0xff;
  int byteone = x & extractor;
  int bytetwo = (x>>8) & extractor;
  int bytethree = (x>>16) & extractor;
  int bytefour = (x>>24) & extractor;
  */
  /*
  int mask = 1; //0001
  mask = mask<<4 + mask;
  mask = mask<<4 + mask;
  mask = mask<<4 + mask;
  mask = mask<<4 + mask;
  mask = mask<<4 + mask;
  mask = mask<<4 + mask;
  mask = mask<<4 + mask; // 16 used until now
  */
  // 0101 -> 5
  // 0011 -> 3
  // 00001111 -> 0f
  // 0000 0000 1111 1111 -> 00ff
  // 0000 0000 0000 0000 1111 1111 1111 1111 -> 0000ffff
  int target = x;
  int level;
  int left;
  int right;
  int extractor;
  // level 1
  level =0;
  extractor = 0x55;
  extractor = extractor + (extractor<<8);
  extractor = extractor + (extractor<<16);
  //printf("%x\n", extractor);
  left =  target & extractor;
  right =  (target>>(1)) & extractor;
  target =  left + right;

  // level 2
  level =1;
  extractor = 0x33;
  extractor = extractor + (extractor<<8);
  extractor = extractor + (extractor<<16);
  //printf("%x\n", extractor);
  left =  target & extractor;
  right =  (target>>(1<<level)) & extractor;
  target =  left + right;

  // level 3
  level =2;
  extractor = 0x0f;
  extractor = extractor + (extractor<<8);
  extractor = extractor + (extractor<<16);
  //printf("%x\n", extractor);
  left =  target & extractor;
  right =  (target>>(1<<level)) & extractor;
  target =  left + right;

  // level 4
  level =3;
  extractor = 0xff;
  extractor = extractor + (extractor<<16);
  //printf("%x\n", extractor);
  left =  target & extractor;
  right =  (target>>(1<<level)) & extractor;
  target =  left + right;

  // level 5
  level =4;
  extractor = 0xff;
  extractor = extractor+ (extractor<<8);
  //printf("%x\n", extractor);
  left =  target & extractor;
  right =  (target>>(1<<level)) & extractor;
  target =  left + right;

  return target;
}

/************************************************
 * bitNor - ~(x|y) using only ~ and & 
 *   Example: bitNor(0x6, 0x5) = 0xFFFFFFF8
 *   Legal ops: ~ &
 *   Max ops: 8
 *   Rating: 1
 */
int bitNor(int x, int y) {
  return (~x) & (~y);
}

/************************************************
 * byteSwap - swaps the nth byte and the mth byte
 *  Examples: byteSwap(0x12345678, 1, 3) = 0x56341278
 *            byteSwap(0xDEADBEEF, 0, 2) = 0xDEEFBEAD
 *  You may assume that 0 <= n <= 3, 0 <= m <= 3
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 25
 *  Rating: 2
 */
int byteSwap(int x, int n, int m) {
  int extractor = 0xff;
  int shift_amount_n = n << 3;
  int shift_amount_m = m << 3;
  int n_copy = x & (extractor << shift_amount_n);
  int n_copy_swap = ((n_copy >> shift_amount_n)&extractor)<<shift_amount_m;
  int m_copy = x & (extractor << shift_amount_m);
  int m_copy_swap = ((m_copy >> shift_amount_m)&extractor)<<shift_amount_n;
  int stamp = n_copy_swap | m_copy_swap;
  int primer =  ~( (extractor << shift_amount_n) | (extractor << shift_amount_m) );
  int res = (x & primer) | stamp;  // prime x, punching 0's into the required placesd and press-stamp the swapped values
  return res;
}

/************************************************ 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
  int trfl = !x + (~1) +1; // forces x into boolean true false value  
  return (y & (trfl)) + (z & (~trfl));
}

/************************************************
 * ezThreeFourths - multiplies by 3/4 rounding toward 0,
 *   Should exactly duplicate effect of C expression (x*3/4),
 *   including overflow behavior.
 *   Examples: ezThreeFourths(11) = 8
 * 
 *             ezThreeFourths(-9) = -6
 *             ezThreeFourths(1073741824) = -268435456 (overflow)
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 3
 */ 
int ezThreeFourths(int x) {
    // pos -> 0000 ; neg --> 1111
  int x_times_3 = (x<<1)+x;
  int x_sign = x_times_3>>31;
  int bias = 3; //= (1<<2)-1
  return ( (~x_sign) & (x_times_3>>2) ) | ((x_sign) & ( (x_times_3+bias) >> 2));
}

/************************************************ 
 * fitsBits - return 1 if x can be represented as an 
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int fitsBits(int x, int n) {
  int sign = (x>>31); //positive 0000, negative 1111
  int n_1 = n + (~1) + 1; //n-1
  int power2n =  (1<<(n_1));
  // see if n is smaller than(leq for negative case as postive must avoid equal case) than power2n
  return !((((~sign) & (power2n+(~x)))|(sign & (power2n+x)))>>31) ;
}

/************************************************
 * getByte - Extract byte n from word x
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: getByte(0x12345678,1) = 0x56
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int getByte(int x, int n) {
  return (x>>(n<<3)) & 0xff;
}

/************************************************ 
 * greatestBitPos - return a mask that marks the position of the
 *               most significant 1 bit. If x == 0, return 0
 *   Example: greatestBitPos(96) = 0x40
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 70
 *   Rating: 4 
 */
int greatestBitPos(int x) {
  int extractor;
  int left;
  int right;
  int target;
  int left_is_zero;
  int mask = 0;
  int final_mask = 1;
  int neg_1 = ~0;
  target =x;
  //level 1 
  extractor = 0xff;
  extractor = extractor+(extractor<<8);
  right = target & extractor;
  left = (target>>16) & extractor;
  left_is_zero = !left + neg_1 ; // if left zero 1111 else 0000 -> left zero --> 1111 else 0000
  target = ((left_is_zero)&left) | ((~left_is_zero)&right);
  mask = (!!left_is_zero); 
  //level 2 
  extractor = 0xff;
  right = target & extractor;
  left = (target>>8) & extractor;
  left_is_zero = !left + neg_1 ; // if left zero 1111 else 0000
  target = ((left_is_zero)&left) | ((~left_is_zero)&right);
  mask = (mask<<1)+(!!left_is_zero); 
  //level 3 
  extractor = 0xf;
  right = target & extractor;
  left = (target>>4) & extractor;
  left_is_zero = !left + neg_1 ; // if left zero 1111 else 0000
  target = ((left_is_zero)&left) | ((~left_is_zero)&right);
  mask = (mask<<1)+(!!left_is_zero); 
  //level 4 
  extractor = 0x3;
  right = target & extractor;
  left = (target>>2) & extractor;
  left_is_zero = !left + neg_1 ; // if left zero 1111 else 0000
  target = ((left_is_zero)&left) | ((~left_is_zero)&right);
  mask = (mask<<1)+(!!left_is_zero); 
  //level 5 
  extractor = 0x1;
  right = target & extractor;
  left = (target>>1) & extractor;
  left_is_zero = !left + neg_1 ; // if left zero 1111 else 0000
  target = ((left_is_zero)&left) | ((~left_is_zero)&right);
  mask = (mask<<1)+(!!left_is_zero);

  //mask = mask+1; // final touch
  final_mask = ((final_mask<<mask)&(!x+neg_1));
  return final_mask;
}

/************************************************
 * implication - return x -> y in propositional logic - 0 for false, 1
 * for true
 *   Example: implication(1,1) = 1
 *            implication(1,0) = 0
 *   Legal ops: ! ~ ^ |
 *   Max ops: 5
 *   Rating: 2
 */
int implication(int x, int y) {
  // 11:1 10:0 01:1 00:1
  return (!x)|y;
}

/************************************************ 
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15 
 *   Rating: 3
 */
int isAsciiDigit(int x) {
  int lowbound = 0x30;
  int highbound = 0x39;
  return !(((x+(~lowbound+1))>>31)|((highbound+(~x+1))>>31));
}

/************************************************ 
 * isEqual - return 1 if x == y, and 0 otherwise 
 *   Examples: isEqual(5,5) = 1, isEqual(4,5) = 0
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int isEqual(int x, int y) {
  return !(x^y);
}

/************************************************
 * isLess - if x < y  then return 1, else return 0 
 *   Example: isLess(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLess(int x, int y) {
  // 0 < y-x figure this out 
  // -x -> ~x+1
  // the most significat bit is reserved for sign; 0 -> positive, 1 -> negative. if its 1, then the computer does ~x+1 to
  // find the negative value. Thus 0xff ff ff ff is -1, and the biggest possible positive number would be using 31bits
  // thus do y-x and use shift and shit to check if the most significant(most left) bit is 0 or 1
  int dif = y + (~x);
  int is_sign_dif = ((x>>31)^(y>>31)); // if different -> gives out 1 not different gives out 0
  return  ((~is_sign_dif) & !(dif>>31))|((is_sign_dif) & (!!(x>>31)));
}


/************************************************
 * isNonNegative - return 1 if x >= 0, return 0 otherwise 
 *   Example: isNonNegative(-1) = 0.  isNonNegative(0) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 3
 */
int isNonNegative(int x) {
  // check most significant bit
  return !((x & (1 << 31)));
}

/************************************************
 * isPower2 - returns 1 if x is a power of 2, and 0 otherwise
 *   Examples: isPower2(5) = 0, isPower2(8) = 1, isPower2(0) = 0
 *   Note that no negative number is a power of 2.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 4
 */
/*
 -x = (~x)+1 -Tmax 
 -Tmin def doesnt work

 Tmin 1000
 tmax 0111
 ~tmin + 1 = 1110

- tmax would work 
- tmin wouldnt work
*/

int isPower2(int x) {
  //x & (x-1) ==0 iff x ==0 or 1bit
  // this works because only when you have one bit, do you have a total mismatch between 0 and 1 when doing & so that there is complete 0
  // when its 0, as the sign changes, everything is going to be 1111, resulting in a 0 
  // power of 2 is just finding if there is just one 1 among the 32 bits. 
  return (!(x & (x+(~0)))) & !((x & 1 << 31)|(!x));
}

/************************************************
 * isTmin - returns 1 if x is the minimum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmin(int x) {
  return (!!x) &(!((~x+1)^x)); 
}

/************************************************
 * minusOne - return a value of -1 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 2
 *   Rating: 1
 */
int minusOne(void) {
  return ~0;
}

/************************************************
 * rotateLeft - Rotate x to the left by n
 *   Can assume that 0 <= n <= 31
 *   Examples: rotateLeft(0x87654321,4) = 0x76543218 // by 4 bits
 *   Legal ops: ~ & ^ | + << >> !
 *   Max ops: 25
 *   Rating: 3 
 */
int rotateLeft(int x, int n) {
  // shift it to the left by n 
  // shift it to the right by 32-n 
  // put them together --> use |
  int neg_n = ~n+1;
  return ((x<<n)| (~(~(~0<<(32+neg_n))<<n)&(x>>(32+neg_n))));
}



/************************************************
 * satMul2 - multiplies by 2, saturating to Tmin or Tmax if overflow
 *   Examples: satMul2(0x30000000) = 0x60000000
 *             satMul2(0x40000000) = 0x7FFFFFFF (saturate to TMax)
 *             satMul2(0x60000000) = 0x80000000 (saturate to TMin)
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int satMul2(int x) {
  return 2;
}

