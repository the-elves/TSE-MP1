--- files/before/sock.c	2020-12-03 05:27:36.320005412 +0530
+++ files/after/sock.c	2020-12-03 05:27:37.276158284 +0530
@@ -577,23 +577,15 @@
 		break;
 	case SO_SNDBUF:
 		/* Don't error on this BSD doesn't and if you think
-		   about it this is right. Otherwise apps have to
-		   play 'guess the biggest size' games. RCVBUF/SNDBUF
-		   are treated in BSD as hints */
-
-		if (val > sysctl_wmem_max)
-			val = sysctl_wmem_max;
+		 * about it this is right. Otherwise apps have to
+		 * play 'guess the biggest size' games. RCVBUF/SNDBUF
+		 * are treated in BSD as hints
+		 */
+		val = min_t(u32, val, sysctl_wmem_max);
 set_sndbuf:
 		sk->sk_userlocks |= SOCK_SNDBUF_LOCK;
-		if ((val * 2) < SOCK_MIN_SNDBUF)
-			sk->sk_sndbuf = SOCK_MIN_SNDBUF;
-		else
-			sk->sk_sndbuf = val * 2;
-
-		/*
-		 *	Wake up sending tasks if we
-		 *	upped the value.
-		 */
+		sk->sk_sndbuf = max_t(u32, val * 2, SOCK_MIN_SNDBUF);
+		/* Wake up sending tasks if we upped the value. */
 		sk->sk_write_space(sk);
 		break;
 
@@ -606,12 +598,11 @@
 
 	case SO_RCVBUF:
 		/* Don't error on this BSD doesn't and if you think
-		   about it this is right. Otherwise apps have to
-		   play 'guess the biggest size' games. RCVBUF/SNDBUF
-		   are treated in BSD as hints */
-
-		if (val > sysctl_rmem_max)
-			val = sysctl_rmem_max;
+		 * about it this is right. Otherwise apps have to
+		 * play 'guess the biggest size' games. RCVBUF/SNDBUF
+		 * are treated in BSD as hints
+		 */
+		val = min_t(u32, val, sysctl_rmem_max);
 set_rcvbuf:
 		sk->sk_userlocks |= SOCK_RCVBUF_LOCK;
 		/*
@@ -629,10 +620,7 @@
 		 * returning the value we actually used in getsockopt
 		 * is the most desirable behavior.
 		 */
-		if ((val * 2) < SOCK_MIN_RCVBUF)
-			sk->sk_rcvbuf = SOCK_MIN_RCVBUF;
-		else
-			sk->sk_rcvbuf = val * 2;
+		sk->sk_rcvbuf = max_t(u32, val * 2, SOCK_MIN_RCVBUF);
 		break;
 
 	case SO_RCVBUFFORCE:
@@ -975,7 +963,7 @@
 		break;
 
 	case SO_PASSCRED:
-		v.val = test_bit(SOCK_PASSCRED, &sock->flags) ? 1 : 0;
+		v.val = !!test_bit(SOCK_PASSCRED, &sock->flags);
 		break;
 
 	case SO_PEERCRED:
@@ -1010,7 +998,7 @@
 		break;
 
 	case SO_PASSSEC:
-		v.val = test_bit(SOCK_PASSSEC, &sock->flags) ? 1 : 0;
+		v.val = !!test_bit(SOCK_PASSSEC, &sock->flags);
 		break;
 
 	case SO_PEERSEC:
