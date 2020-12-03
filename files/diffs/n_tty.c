--- files/before/n_tty.c	2020-12-03 05:27:17.416983145 +0530
+++ files/after/n_tty.c	2020-12-03 05:27:18.357133459 +0530
@@ -2353,8 +2353,12 @@
 			if (tty->ops->flush_chars)
 				tty->ops->flush_chars(tty);
 		} else {
+			struct n_tty_data *ldata = tty->disc_data;
+
 			while (nr > 0) {
+				mutex_lock(&ldata->output_lock);
 				c = tty->ops->write(tty, b, nr);
+				mutex_unlock(&ldata->output_lock);
 				if (c < 0) {
 					retval = c;
 					goto break_out;
