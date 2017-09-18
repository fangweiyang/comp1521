void rightShiftBits(Bits b, int shift, Bits res){
   int i=0;
   unsigned int overflow=0;
   Bits temp= makeBits(64);
   setBitsFromBits(b,temp);
   for(;shift>=32;shift=shift-31){
      for (i=b->nwords-1;i >=0;i--){
         temp->words[i]= b->words[i] >> 31;
         if(i > 0 ){
             overflow=b->words[i-1] << 1;
             temp->words[i]= temp->words[i]+overflow;
         }
     }
   }
   int unshift=32-shift;
   for(i=temp->nwords-1;i >=0;i--){
      res->words[i]=(temp->words[i] >> shift);
      if(i>0) {
         overflow=temp->words[i-1] << unshift;
         res->words[i]= res->words[i]+overflow;
      }
   }
   res->nwords=b->nwords;
}
