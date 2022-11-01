s8 IntLut[] = "00010203040506070809"
              "10111213141516171819"
              "20212223242526272829"
              "30313233343536373839"
              "40414243444546474849"
              "50515253545556575859"
              "60616263646566676869"
              "70717273747576777879"
              "80818283848586878889"
              "90919293949596979899";

// InputBuffer must be at least 20 bytes long
u8 U64ToString(s8 *Buffer, u64 Int) {
   s8 Tmp;
   u8 I, J, K, Mod;

   if(Int < 10) {
      Buffer[0] = Int + 0x30;
      return 1;
   }
   
   I = 0;
   J = 0;

   while(Int > 100) {
      Mod = Int % 100 * 2; 

      Buffer[I + 1] = IntLut[Mod];
      Buffer[I] = IntLut[Mod + 1];

      Int /= 100;
      I += 2;
   }
   
   while(Int > 0) {
      Mod = Int % 10; 

      Buffer[I++] = Mod + 0x30;

      Int /= 10;
   }

   K = I;
   --I;

   while(J < I) {
      Tmp = Buffer[I];
      Buffer[I--] = Buffer[J];
      Buffer[J++] = Tmp;
   }

   return K;
}

s8 HexLut[] = "000102030405060708090A0B0C0D0E0F"
              "101112131415161718191A1B1C1D1E1F"
              "202122232425262728292A2B2C2D2E2F"
              "303132333435363738393A3B3C3D3E3F"
              "404142434445464748494A4B4C4D4E4F"
              "505152535455565758595A5B5C5D5E5F"
              "606162636465666768696A6B6C6D6E6F"
              "707172737475767778797A7B7C7D7E7F"
              "808182838485868788898A8B8C8D8E8F"
              "909192939495969798999A9B9C9D9E9F"
              "A0A1A2A3A4A5A6A7A8A9AAABACADAEAF"
              "B0B1B2B3B4B5B6B7B8B9BABBBCBDBEBF"
              "C0C1C2C3C4C5C6C7C8C9CACBCCCDCECF"
              "D0D1D2D3D4D5D6D7D8D9DADBDCDDDEDF"
              "E0E1E2E3E4E5E6E7E8E9EAEBECEDEEEF"
              "F0F1F2F3F4F5F6F7F8F9FAFBFCFDFEFF";

// Buffer has to be at least twice as long as Hex Data
u32 HexToString(s8 *Buffer, u8 *Hex, u32 Len) {
   u32 I;

   for(I = 0; I < Len; ++I) {
      Buffer[I * 2] = HexLut[Hex[I] * 2];
      Buffer[I * 2 + 1] = HexLut[Hex[I] * 2 + 1];
   }
   
   return Len * 2;
}

#ifndef HEX_DUMP_WIDTH
#define HEX_DUMP_WIDTH 20
#endif
// Buffer has to be at least 4 times as long as Hex Data
u32 HexToHexDump(s8 *Buffer, u8 *Hex, u32 Len) {
   u32 I, J, K;

   K = 0;

   for(I = 0; I < Len; I += HEX_DUMP_WIDTH) {
      
      for(J = I; J < min(I + HEX_DUMP_WIDTH, Len); ++J) {
         Buffer[K++] = HexLut[Hex[J] * 2];
         Buffer[K++] = HexLut[Hex[J] * 2 + 1];
         Buffer[K++] = ' ';
      }
      
      Buffer[K++] = '\n';
   }
   
   return K;
}

INLINE u8 Equal(s8 *A, s8 *B) {
   while(*A++ == *B++ && *A && *B);
   
   return !(*A || *B); 
}

INLINE u8 EqualFor(s8 *A, s8 *B, u32 Len) {
   while(*A++ == *B++ && Len--);
   
   return (s32)Len < 1; 
}

INLINE u32 EqualForLengthOf(s8 *A, s8 *B, u32 Stop) {
   u32 Length; 

   Length = 0;

   while(*A++ == *B++ && Length < Stop) ++Length;
   
   return Length; 
}

INLINE s32 FindFor(s8 *Buffer, s8 Character, s32 Length) {
   s32 I;

   for(I = 0; I < Length; ++I) {

      if(Buffer[I] == Character) return I;
   }

   return -1;
}

INLINE u8 EqualUntil(s8 *A, s8 *B, s8 Char) {
   while(*A++ == *B++ && *A != Char && *B);
   
   return !*B && *--A == *--B; 
}

INLINE u0 Fill64(u64 *A, u64 C, u32 Len) {
   while(Len--) *A++ = C;
}

INLINE u0 Copy8(u8 *Dst, u8 *Src, u32 Len) {
   while(Len--) *Dst++ = *Src++;
}

INLINE u0 Copy16(u16 *Dst, u16 *Src, u32 Len) {
   while(Len--) *Dst++ = *Src++;
}

INLINE u0 Copy64(u64 *Dst, u64 *Src, u32 Len) {
   while(Len--) *Dst++ = *Src++;
}

INLINE u0 StringCopy(s8 *Dst, s8 *Src) {
   while(*Src) *Dst++ = *Src++;
}

INLINE u32 StringLength(s8 *String) {
   u32 Size;

   Size = 0;

   while(*String++) Size++;

   return Size + 1;
}
