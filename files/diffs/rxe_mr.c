--- files/before/rxe_mr.c	2020-12-03 05:29:02.329756883 +0530
+++ files/after/rxe_mr.c	2020-12-03 05:29:03.249903998 +0530
@@ -59,9 +59,11 @@
 
 	case RXE_MEM_TYPE_MR:
 	case RXE_MEM_TYPE_FMR:
-		return ((iova < mem->iova) ||
-			((iova + length) > (mem->iova + mem->length))) ?
-			-EFAULT : 0;
+		if (iova < mem->iova ||
+		    length > mem->length ||
+		    iova > mem->iova + mem->length - length)
+			return -EFAULT;
+		return 0;
 
 	default:
 		return -EFAULT;
