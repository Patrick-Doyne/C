/* This is the only file you will be editing.
 * - Copyright of Starter Code: Prof. Kevin Andrea, George Mason University. All
Rights Reserved
 * - Copyright of Student Code: You!
 */

#include <stdio.h>
#include <stdlib.h>
#include "common_structs.h"
#include "common_definitions.h"
#include "common_functions.h"
#include "microfp.h"


//Check for infinite number
int isInfinity(microfp_s val){
	if((((val>>5) & 7) == 7) && ((val & 31) == 0)){
		return 1;
	}
	return 0;
}
//Check for not a number
int isNan(microfp_s val){
	if((((val>>5) & 7) == 7) && ((val&31) != 0)){
		return 1;
	}
	return 0;
}
//Check for negative number
int isNegative(microfp_s val){
	if(((1<<8)&val)!=0){
		return 1;
	}
	return 0;
}
//Check for normalized value
int isNormalized(microfp_s val){
	if(((val>>5) & 7) == 0){
		return 0;
	}
	return 1;
}
//Check for zero value
int isZero(microfp_s val){
	if((((val>>5) & 7) == 0) && ((val & 31) == 0)){
		return 1;
	}
	return 0;
}
//Get exp from micro
int getExp(microfp_s val){
	int exp = ((val>>5) & 7);
	return exp;
}
//Calculate E for micro
int getE(microfp_s val){
	int E = 0;
	if(getExp(val) <= 0){
		E = -2;
	}
	else{
		E = getExp(val) - 3;
	}
	return E;
}
//Shift the number to the left 1 keeping decimal in place
void shiftLeft(microfp_s *whole, microfp_s *frac){
	*whole = (*whole<<1) | ((*frac>>15) & 1);
	*frac = *frac << 1;
}
//Shift the number to the right 1 keeping the decimal in place
void shiftRight(microfp_s *whole, microfp_s *frac){
	*frac = (*frac >> 1) | ((*whole & 1) << 15);
	*whole = *whole >> 1;
}
//All special arithmetic rules for multiplication
microfp_s multSpecialArithmetic(microfp_s micro, microfp_s val1, microfp_s val2){
	//Inf * Inf
	if(isInfinity(val1) && isInfinity(val2)){
		micro = micro | (7<<5);
		return micro;
	}
	//Nan * Anything
	else if(isNan(val1) || isNan(val2)){
		micro = (micro | (7<<5))+1;
		return micro;
	}
	//Inf and Zero
	else if((isInfinity(val1) && isZero(val2)) || (isInfinity(val2) &&
			isZero(val1))){
		micro = (micro | (7<<5))+1;
		return micro;
	}
	//Inf and Anything
	else if(isInfinity(val1) || isInfinity(val2)){
		micro = micro | (7<<5);
		return micro;
	}
	return micro;
}
//All special arithmetic rules for addition and subtraction
microfp_s addSubSpecialArithmetic(microfp_s micro, microfp_s val1, microfp_s val2){
	//Nan +- Anything
	if(isNan(val1) || isNan(val2)){
		micro = (7<<5) + 1;
		return micro;
	}
	//Only one val is Inf
	else if(isInfinity(val1) ^ isInfinity(val2)){
		micro = (7 << 5);
		//-Inf +- X
		if(isInfinity(val1) && isNegative(val1))
			micro = (7 << 5)|(1<<8);
		//X +- -Inf
		else if(isInfinity(val2) && isNegative(val2))
			micro = (7 << 5)|(1<<8);
		return micro;
	}
	//Both values are Inf
	else if(isInfinity(val1) && isInfinity(val2)){
		//Inf + -Inf = Nan
		if(isNegative(val1) ^ isNegative(val2)){
			micro = (7 << 5) + 1;
			return micro;
		}
		//-Inf + -Inf = -Inf
		else if(isNegative(val1) && isNegative(val2)){
			micro = (7 << 5) | (1 << 8);
			return micro;
		}
		//Inf + Inf = Inf
		else{
			micro = (7 << 5);
			return micro;
		}
	}
	//-0 + -0 = -0
	else if((isNegative(val1) && isNegative(val2)) && (isZero(val1) &&
			isZero(val2))){
		micro = 1 << 8;
		return micro;
	}
	return micro;
}
//Convert all variables to proper values to put into a micro
void toMicroAfterMath(int *exp, int *E, microfp_s *product, microfp_s *fraction){
	//Shift to "normalized" whole and frac
	while(*product!=1){
		shiftRight(product, fraction);
		*E+=1;
	}
	//Infinity
	if(*E>3){
		*exp = 7<<5;
		*fraction = 0;
	}
	//Normalized
	else if((*E>=-1 && *E<=3) || (*E == -2 && (*fraction >> 14!= 0 || *fraction
			>> 15 !=0 || *fraction >> 11 == 0))){
		*exp = (*E + 3) << 5;
	}
	//Denormalized
	else{
		while(*E!=-2){
			shiftRight(product,fraction);
			*E+=1;
		}
	}
}
//Set E equal for addition and subtraction
void equalizeE(microfp_s *whole1, int *val1E, microfp_s *whole2, int *val2E){
	//Trash tracker
	microfp_s fraction = 0;
	//While not equal shift largest left
	while(*val1E != *val2E){
		if(*val2E > *val1E){
			shiftLeft(whole2,&fraction);
			*val2E -= 1;
		}
		else{
			shiftLeft(whole1, &fraction);
			*val1E -= 1;
		}
	}
}
//Let exp and frac bits for a micro
microfp_s setBits(int exp, microfp_s frac){
	microfp_s micro = 0;
	micro = micro | frac;
	micro = micro | exp;
	return micro;
}
/* toMicroFP - Converts a Number Struct (whole and fraction parts) into a MicroFP
Value
 * - number is managed by MUAN, DO NOT FREE number.
 * - You may change the contents of number, but do not free it.
 * - Follow the project documentation for this function.
 * Return the MicroFP Value or any legal MicroFP NaN representation if number is
NULL.
 */
microfp_s toMicroFP(Number_s *num) {
	//Num == NULL
	if(num == NULL) return ((7<<5)+1);
	//Variables
	microfp_s micro = 0;
	int E = 0;
	//Negative
	if(num->is_negative == 1){
		micro = micro | (1<<8);
	}
	//Nan
	if(num->is_nan == 1){
		micro = (micro | (7<<5)) + 1;
		return micro;
	}
	//Infinity
	else if(num->is_infinity){
		micro = (micro | (7<<5));
		return micro;
	}
	//Normalized whole and fraction
	else if(num->whole!=0){
		while(num->whole != 1){
			shiftRight(&num->whole, &num->fraction);
			E++;
		}
	}
	//Normalized fraction
	else if(num->whole == 0 && (((num->fraction >> 14)!=0) || ((num->fraction
			>>15)!= 0))){
		while(num->whole != 1){
			shiftLeft(&num->whole,&num->fraction);
			E--;
		}
	}
	//Denomarlaized
	else{
		E = -999;
	}
	int exp = 0;
	//Infinity or Nan
	if(E > 3){
		exp = 7<<5;
		micro = micro | setBits(exp, 0);
	}
	//Normalized
	else if(E >= -2 && E <=3){
		exp = (E +3)<<5;
		micro = micro | setBits(exp, num->fraction>>11);
	}
	//Denormalized
	else{
		micro = micro | setBits(exp, num->fraction>>9);
	}
	return micro; // Replace this Line with your Code!
}
/* toNumber - Converts a MicroFP Value into a Number Struct (whole and fraction
parts)
 * - number is managed by MUAN, DO NOT FREE or re-Allocate number.
 * - It is already allocated. Do not call malloc/calloc for number.
 * - Follow the project documentation for this function.
 * If the conversion is successful, return 0.
 * - If number is NULL, return -1.
 */
int toNumber(Number_s *number, microfp_s value) {
	//If NULL return -1
	if(number == NULL) return -1;
	//Set everything to 0
	number->is_negative = 0;
	number->is_infinity = 0;
	number->is_nan = 0;
	number->whole = 0;
	number->fraction = 0;
	//Negative
	if(isNegative(value)){
		number->is_negative = 1;
	}
	//Nan
	if(isNan(value)){
		number->is_nan = 1;
		return 0;
	}
	//Infinity
	else if(isInfinity(value)){
		number->is_infinity = 1;
		return 0;
	}
	//Zero
	else if(isZero(value)) return 0;
	//Check for normalized value
	if(isNormalized(value)){
		number->whole = 1;
	}
	//Get fraction and E
	number->fraction = (value & 31)<<11;
	int E = getE(value);
	//Convert to decimal value
	while(E!=0){
		if(E>0){
			shiftLeft(&number->whole,&number->fraction);
			E--;
		}
		else if(E<0 && number->whole == 1){
			shiftRight(&number->whole, &number->fraction);
			E++;
		}
		else{
			number->fraction = number->fraction >> 1;
			E++;
		}
	}
	return 0; // Replace this Line with your Code!
}
/* mulMicroFP - Performs an operation on two MicroFP values
 * - Follow the project documentation for this function.
 * Return the resulting microfp_s value
 */
microfp_s mulMicroFP(microfp_s val1, microfp_s val2) {
	//Initilize final answer to proper sign bit
	microfp_s micro = (isNegative(val1) ^ isNegative(val2))<<8;
	//Special arithmetics
	micro = multSpecialArithmetic(micro,val1,val2);
	if((micro & ~(1<<8))!=0) return micro;
	if(isZero(val1) || isZero(val2)) return micro;
	//Computational components
	int exp = 0;
	int E = -10+getE(val1)+getE(val2);
	microfp_s whole1 = (val1 & 31);
	microfp_s whole2 = (val2 & 31);
	microfp_s fraction = 0;
	microfp_s product = 0;
	//Turn on index 5 bit for Normalized values
	if(isNormalized(val1)) whole1= whole1 | (1<<5);
	if(isNormalized(val2)) whole2 = whole2 | (1<<5);
	//Multiply
	product = whole1 * whole2;
	//Shift product back to Binary Scientific and track computational components
	toMicroAfterMath(&exp, &E, &product, &fraction);
	//Maintain bit 8, set exp bits (5-7), set fraction bits(0-4)
	micro = micro | setBits(exp,fraction >> 11);
	return micro; // Replace this Line with your Code!
}
/* addMicroFP - Performs an operation on two MicroFP values
 * - Follow the project documentation for this function.
 * Return the resulting microfp_s value
 */
microfp_s addMicroFP(microfp_s val1, microfp_s val2) {
	//Computational components
	int val1E = getE(val1)-5;
	int val2E = getE(val2)-5;
	microfp_s whole1 = val1 & 31;
	microfp_s whole2 = val2 & 31;
	microfp_s sum = 0;
	microfp_s fraction = 0;
	int exp = 0;
	//Initialize final answer to 0
	microfp_s micro = 0;
	//Special addition and subtraction arithmetic rules
	micro = addSubSpecialArithmetic(micro, val1, val2);
	if(micro!=0)return micro;
	//Turn on index 5 bit for Normalized values
	if(isNormalized(val1)) whole1 = whole1 | (1<<5);
	if(isNormalized(val2)) whole2 = whole2 | (1<<5);
	//Shift val1 or val2 until they have equal E
	equalizeE(&whole1, &val1E, &whole2, &val2E);
	//Perform addition
	if(isNegative(val1) ^ isNegative(val2)){
		if(whole1>=whole2){
			sum = whole1 - whole2;
			if(whole1>whole2)
				micro = micro | isNegative(val1) << 8;
		}
		else{
			sum = whole2 - whole1;
			micro = micro | isNegative(val2) << 8;
		}
	}
	else{
		sum = whole1 + whole2;
		micro = micro | isNegative(val2) << 8;
	}
	//If sum is 0 we are done
	if(sum == 0) return micro;
	//E after arithmetic
	int E = val1E;
	//Shift product back to Binary Scientific and track computational components
	toMicroAfterMath(&exp, &E, &sum, &fraction);
	//Maintain bit 8, set exp bits (5-7), set fraction bits(0-4)
	micro = micro | setBits(exp,fraction>>11);
	return micro;
}
/* opMicroFP - Performs an operation on two MicroFP values
 * - Follow the project documentation for this function.
 * Return the resulting microfp_s value
 */
microfp_s subMicroFP(microfp_s val1, microfp_s val2) {
	//Use negMicroFP to add a negative
	return addMicroFP(val1, negMicroFP(val2));
}
/* negMicroFP - Negates a MicroFP Value.
 * - Follow the project documentation for this function.
 * Return the resulting MicroFP Value
 */
microfp_s negMicroFP(microfp_s value) {
	//Negative->Positive
	if(isNegative(value)){
		value = value & ~(1<<8);
	}
	//Positive->Negative
	else{
		value = value | 1<<8;
	}
	return value;
}