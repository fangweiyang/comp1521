// ADT for Bit-strings
// COMP1521 17s2 Week02 Lab Exercise
// Written by John Shepherd, July 2017
// Modified by ...

#include <stdlib.h>
#include<math.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "Bits.h"

// assumes that an unsigned int is 32 bits
#define BITS_PER_WORD 32

// A bit-string is an array of unsigned ints (each a 32-bit Word)
// The number of bits (hence Words) is determined at creation time
// Words are indexed from right-to-left
// words[0] contains the most significant bits
// words[nwords-1] contains the least significant bits
// Within each Word, bits are indexed right-to-left
// Bit position 0 in the Word is the least significant bit
// Bit position 31 in the Word is the most significant bit

typedef unsigned int Word;

struct BitsRep {
   int nwords;   // # of Words
   Word *words;  // array of Words
};
void printBit(int i);
int power(int exponent);
long decimalToBinary(int n);
char *BitList(int i);
void leftShiftBits_case_1(Bits b, int shift, Bits res);
void leftShiftBits_case_2(Bits b, int shift, Bits res)
// make a new empty Bits with space for at least nbits
// rounds up to nearest multiple of BITS_PER_WORD
Bits makeBits(int nbits)
{
   Bits new;
   new = malloc(sizeof(struct BitsRep));
   assert(new != NULL);
   int  nwords;
   if (nbits%BITS_PER_WORD == 0)
      nwords = nbits/BITS_PER_WORD;
   else
      nwords = 1+nbits/BITS_PER_WORD;
   new->nwords = nwords;
   // calloc sets to all 0's
   new->words = calloc(nwords, sizeof(Word));
   assert(new->words != NULL);
   return new;
}

// release space used by Bits
void  freeBits(Bits b)
{
   assert(b != NULL && b->words != NULL);
   free(b->words);
   free(b);
}

// form bit-wise AND of two Bits a,b
// store result in res Bits
void andBits(Bits a, Bits b, Bits res){
  int i,max=0;
  max = (a->nwords >= b->nwords)? a->nwords : b->nwords;
  //less = (a->nwords >= b->nwords)? b->nwords : a->nwords;
  for(i = 0;i < max; i++){
     res ->words[i]= a->words[i] & b->words[i];
  }
  res->nwords=max;
}

// form bit-wise OR of two Bits a,b
// store result in res Bits
void orBits(Bits a, Bits b, Bits res){
  int i,max=0;
  max = (a->nwords >= b->nwords)? a->nwords : b->nwords;
  //less = (a->nwords >= b->nwords)? b->nwords : a->nwords;
  for(i = 0;i < max; i++){
     res->words[i] = a->words[i]|b->words[i];
  }
  res->nwords=max;
}

// form bit-wise negation of Bits a,b
// store result in res Bits
void invertBits(Bits a, Bits res){
   int count=0;
   for(count=0;count<a->nwords;count++){
      res->words[count]= ~a->words[count];
    }
   res->nwords=a->nwords;
}

// left shift Bits
void leftShiftBits(Bits b, int shift, Bits res){
  if(b->nwords==1){
    if(shift>32) shift=32;
    leftShiftBits_case_1(b,shift,res);
    return;
  }elseif(shift> b->nwords* 32){
    shift=32;
    leftShiftBits_case_1(b,shift,res);
    return;
  }else{
    leftShiftBits_case_2(b,shift,res);
    return;
  }
}

// right shift Bits
void rightShiftBits(Bits b, int shift, Bits res){
  int i=0;
  for (i=0;i < b->nwords;i++){
    res->words[i]=b->words[i] >> shift;
  }
  res->nwords=b->nwords;
}

// copy value from one Bits object to another
void setBitsFromBits(Bits from, Bits to){
   int count=0;
   unsigned int temp=0;
   while(count<from->nwords){
        temp=from->words[count];
        to->words[count]=from->words[count];
        count++;
   }
   from->nwords=to->nwords;
}

// assign a bit-string (sequence of 0's and 1's) to Bits
// if the bit-string is longer than the size of Bits, truncate higher-order bits
void setBitsFromString(Bits b, char *bitseq){
 int i,j=0;
   unsigned int result=0;
   for(j=0;j < b->nwords;j++){
      result=0;
      for(i=32-strlen(bitseq);i<32;i++,bitseq++){
         if(*bitseq=='1')
             result=result+power(i);
      }
      b->words[j]=result;
   }
}

// display a Bits value as sequence of 0's and 1's
void showBits(Bits b){
   int i;
   for(i=b->nwords-1;i >=0;i--){
      printBit(b->words[i]);
   }
}


int power(int exponent){
  unsigned int result=0;
  result = pow(2, exponent);
  return  result;
}

long decimalToBinary(int n) {
    int remainder;
    long binary = 0, i = 1;
    while(n != 0) {
        remainder = n%2;
        n = n/2;
        binary= binary + (remainder*i);
        i = i*10;
    }
    return binary;
}


void printBit(int i){
   int list[32];
   int j = 0;
   int mask = 1;
   for(j = 0;j < 32; j++){
      if(i & mask){
         list[j]=1;
      }else{
         list[j]=0;
      }
      i=i>>1;
   }
   for(j = 0;j<32;j++){
       printf("%d",list[j]);
   }
}

char *BitList(int i){
  char *bit_list= malloc(sizeof(char) *BITS_PER_WORD );;
  int j = 0;
  int mask = 1;
  for(j = 0;j < 32; j++){
     if(i & mask){
        bit_list[j]=1;
     }else{
        bit_list[j]=0;
     }
     i=i>>1;
  }
  return bit_list;
}



void leftShiftBits_case_1(Bits b, int shift, Bits res){
   int i=0;
   for (i=0;i < b->nwords;i++){
     res->words[i]=b->words[i] << shift;
   }
   res->nwords=b->nwords;
}

void leftShiftBits_case_2(Bits b, int shift, Bits res){
  int i=0;
  unsigned int overflow=0;
  for (i=0;i < b->nwords;i++){
    res->words[i]=b->words[i] << shift;
    if(i>1){
        overflow==b->words[i-1] >> 32-shift;
        res->words[i]=res->words[i]+overflow;
    }
  }
  res->nwords=b->nwords;
  }
