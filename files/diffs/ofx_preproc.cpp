--- files/before/ofx_preproc.cpp	2020-12-03 05:28:50.367844380 +0530
+++ files/after/ofx_preproc.cpp	2020-12-03 05:28:51.259987019 +0530
@@ -417,7 +417,6 @@
 string sanitize_proprietary_tags(string input_string)
 {
   unsigned int i;
-  size_t input_string_size;
   bool strip = false;
   bool tag_open = false;
   int tag_open_idx = 0; //Are we within < > ?
@@ -438,9 +437,17 @@
     close_tagname[i] = 0;
   }
 
-  input_string_size = input_string.size();
+  size_t input_string_size = input_string.size();
 
-  for (i = 0; i < input_string_size; i++)
+  // Minimum workaround to prevent buffer overflow: Stop iterating
+  // once the (fixed!) size of the output buffers is reached. In
+  // response to
+  // https://www.talosintelligence.com/vulnerability_reports/TALOS-2017-0317
+  //
+  // However, this code is a huge mess anyway and is in no way
+  // anything like up-to-date C++ code. Please, anyone, replace it
+  // with something more modern. Thanks. - cstim, 2017-09-17.
+  for (i = 0; i < std::min(input_string_size, size_t(READ_BUFFER_SIZE)); i++)
   {
     if (input_string.c_str()[i] == '<')
     {
