--- files/before/update.c	2020-12-03 05:29:25.713495538 +0530
+++ files/after/update.c	2020-12-03 05:29:26.657646492 +0530
@@ -208,11 +208,9 @@
 
 	if (bitmapUpdate->number > bitmapUpdate->count)
 	{
-		UINT16 count;
-		BITMAP_DATA* newdata;
-		count = bitmapUpdate->number * 2;
-		newdata = (BITMAP_DATA*) realloc(bitmapUpdate->rectangles,
-		                                 sizeof(BITMAP_DATA) * count);
+		UINT32 count = bitmapUpdate->number * 2;
+		BITMAP_DATA* newdata = (BITMAP_DATA*) realloc(bitmapUpdate->rectangles,
+		                       sizeof(BITMAP_DATA) * count);
 
 		if (!newdata)
 			goto fail;
