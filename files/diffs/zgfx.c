--- files/before/zgfx.c	2020-12-03 05:29:27.525785292 +0530
+++ files/after/zgfx.c	2020-12-03 05:29:28.433930489 +0530
@@ -137,6 +137,7 @@
 	_zgfx->cBitsCurrent -= _nbits;
 	_zgfx->bits = _zgfx->BitsCurrent >> _zgfx->cBitsCurrent;
 	_zgfx->BitsCurrent &= ((1 << _zgfx->cBitsCurrent) - 1);
+	return TRUE;
 }
 
 static void zgfx_history_buffer_ring_write(ZGFX_CONTEXT* zgfx, const BYTE* src, size_t count)
@@ -229,9 +230,15 @@
 	UINT32 count;
 	UINT32 distance;
 	BYTE* pbSegment;
-	size_t cbSegment = segmentSize - 1;
+	size_t cbSegment;
 
-	if ((Stream_GetRemainingLength(stream) < segmentSize) || (segmentSize < 1))
+	if (!zgfx || !stream)
+		return FALSE;
+
+	cbSegment = segmentSize - 1;
+
+	if ((Stream_GetRemainingLength(stream) < segmentSize) || (segmentSize < 1) ||
+	    (segmentSize > UINT32_MAX))
 		return FALSE;
 
 	Stream_Read_UINT8(stream, flags); /* header (1 byte) */
@@ -242,6 +249,10 @@
 	if (!(flags & PACKET_COMPRESSED))
 	{
 		zgfx_history_buffer_ring_write(zgfx, pbSegment, cbSegment);
+
+		if (cbSegment > sizeof(zgfx->OutputBuffer))
+			return FALSE;
+
 		CopyMemory(zgfx->OutputBuffer, pbSegment, cbSegment);
 		zgfx->OutputCount = cbSegment;
 		return TRUE;
@@ -280,6 +291,9 @@
 					if (++zgfx->HistoryIndex == zgfx->HistoryBufferSize)
 						zgfx->HistoryIndex = 0;
 
+					if (zgfx->OutputCount >= sizeof(zgfx->OutputBuffer))
+						return FALSE;
+
 					zgfx->OutputBuffer[zgfx->OutputCount++] = c;
 				}
 				else
@@ -313,6 +327,9 @@
 							count += zgfx->bits;
 						}
 
+						if (count > sizeof(zgfx->OutputBuffer) - zgfx->OutputCount)
+							return FALSE;
+
 						zgfx_history_buffer_ring_read(zgfx, distance, &(zgfx->OutputBuffer[zgfx->OutputCount]), count);
 						zgfx_history_buffer_ring_write(zgfx, &(zgfx->OutputBuffer[zgfx->OutputCount]), count);
 						zgfx->OutputCount += count;
@@ -325,6 +342,10 @@
 						zgfx->cBitsRemaining -= zgfx->cBitsCurrent;
 						zgfx->cBitsCurrent = 0;
 						zgfx->BitsCurrent = 0;
+
+						if (count > sizeof(zgfx->OutputBuffer) - zgfx->OutputCount)
+							return FALSE;
+
 						CopyMemory(&(zgfx->OutputBuffer[zgfx->OutputCount]), zgfx->pbInputCurrent, count);
 						zgfx_history_buffer_ring_write(zgfx, zgfx->pbInputCurrent, count);
 						zgfx->pbInputCurrent += count;
