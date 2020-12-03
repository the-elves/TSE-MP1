--- files/before/stb_vorbis.c	2020-12-03 05:29:04.194054952 +0530
+++ files/after/stb_vorbis.c	2020-12-03 05:29:05.566274349 +0530
@@ -3,9 +3,9 @@
 //
 // Original version written by Sean Barrett in 2007.
 //
-// Originally sponsored by RAD Game Tools. Seeking sponsored
-// by Phillip Bennefall, Marc Andersen, Aaron Baker, Elias Software,
-// Aras Pranckevicius, and Sean Barrett.
+// Originally sponsored by RAD Game Tools. Seeking implementation
+// sponsored by Phillip Bennefall, Marc Andersen, Aaron Baker,
+// Elias Software, Aras Pranckevicius, and Sean Barrett.
 //
 // LICENSE
 //
@@ -32,6 +32,7 @@
 //    manxorist@github   saga musix          github:infatum
 //
 // Partial history:
+//    1.12    - 2017/11/21 - limit residue begin/end to blocksize/2 to avoid large temp allocs in bad/corrupt files
 //    1.11    - 2017/07/23 - fix MinGW compilation 
 //    1.10    - 2017/03/03 - more robust seeking; fix negative ilog(); clear error in open_memory
 //    1.09    - 2016/04/04 - back out 'truncation of last frame' fix from previous version
@@ -2042,6 +2043,8 @@
    return TRUE;
 }
 
+// n is 1/2 of the blocksize --
+// specification: "Correct per-vector decode length is [n]/2"
 static void decode_residue(vorb *f, float *residue_buffers[], int ch, int n, int rn, uint8 *do_not_decode)
 {
    int i,j,pass;
@@ -2049,7 +2052,10 @@
    int rtype = f->residue_types[rn];
    int c = r->classbook;
    int classwords = f->codebooks[c].dimensions;
-   int n_read = r->end - r->begin;
+   unsigned int actual_size = rtype == 2 ? n*2 : n;
+   unsigned int limit_r_begin = (r->begin < actual_size ? r->begin : actual_size);
+   unsigned int limit_r_end   = (r->end   < actual_size ? r->end   : actual_size);
+   int n_read = limit_r_end - limit_r_begin;
    int part_read = n_read / r->part_size;
    int temp_alloc_point = temp_alloc_save(f);
    #ifndef STB_VORBIS_DIVIDES_IN_RESIDUE
@@ -4077,7 +4083,10 @@
       int i,max_part_read=0;
       for (i=0; i < f->residue_count; ++i) {
          Residue *r = f->residue_config + i;
-         int n_read = r->end - r->begin;
+         unsigned int actual_size = f->blocksize_1 / 2;
+         unsigned int limit_r_begin = r->begin < actual_size ? r->begin : actual_size;
+         unsigned int limit_r_end   = r->end   < actual_size ? r->end   : actual_size;
+         int n_read = limit_r_end - limit_r_begin;
          int part_read = n_read / r->part_size;
          if (part_read > max_part_read)
             max_part_read = part_read;
@@ -4088,6 +4097,8 @@
       classify_mem = f->channels * (sizeof(void*) + max_part_read * sizeof(int *));
       #endif
 
+      // maximum reasonable partition size is f->blocksize_1
+
       f->temp_memory_required = classify_mem;
       if (imdct_mem > f->temp_memory_required)
          f->temp_memory_required = imdct_mem;
@@ -5351,6 +5362,8 @@
 #endif // STB_VORBIS_NO_PULLDATA_API
 
 /* Version history
+    1.12    - 2017/11/21 - limit residue begin/end to blocksize/2 to avoid large temp allocs in bad/corrupt files
+    1.11    - 2017/07/23 - fix MinGW compilation 
     1.10    - 2017/03/03 - more robust seeking; fix negative ilog(); clear error in open_memory
     1.09    - 2016/04/04 - back out 'avoid discarding last frame' fix from previous version
     1.08    - 2016/04/02 - fixed multiple warnings; fix setup memory leaks;
