#define ERR(fmt, ...) fprintf(stderr, "ERROR: %ls!\n", fmt, __VA_ARGS__); getchar();

INLINE u0 *Allocate(u64 Bytes) {
   return VirtualAlloc(0, Bytes, MEM_COMMIT, PAGE_READWRITE);
}

INLINE u0 Free(u0 *Memory) {
   VirtualFree(Memory, 0, MEM_RELEASE);
}

s8 *ReadWholeFile(const s8 *FileName, u32 *Size) {
   s8 *Buffer;
   u32 *FileHandle, FileSize, ReadBytes;

   FileHandle = CreateFile(FileName, GENERIC_READ, 0, 0, OPEN_ALWAYS, 
                           FILE_ATTRIBUTE_NORMAL, 0);

   FileSize = GetFileSize(FileHandle, 0);

   Buffer = (s8 *)Allocate(FileSize);

   ReadFile(FileHandle, Buffer, FileSize, &ReadBytes, 0);

   *Size = ReadBytes;
   
   CloseHandle(FileHandle);

   return Buffer;
}

u0 WriteWholeFile(const s8 *FileName, u8 *Buffer, u32 Size) {
   u32 *FileHandle, BytesWritten;

   FileHandle = CreateFile(FileName, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 
                           FILE_ATTRIBUTE_NORMAL, 0);

   WriteFile(FileHandle, Buffer, Size, &BytesWritten, 0);

   CloseHandle(FileHandle);
}

double secpercount = 0.0;

f64 Time() {
   u64 starttime = 0;
   u64 countspersec = 0;

   // Get current time, and determine how fast the clock ticks
    QueryPerformanceCounter((LARGE_INTEGER*)&starttime);
    QueryPerformanceFrequency((LARGE_INTEGER*)&countspersec);
    return starttime/(f64)countspersec;
}
