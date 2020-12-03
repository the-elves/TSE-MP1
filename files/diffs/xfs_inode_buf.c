--- files/before/xfs_inode_buf.c	2020-12-03 05:29:12.359360444 +0530
+++ files/after/xfs_inode_buf.c	2020-12-03 05:29:13.279507559 +0530
@@ -374,6 +374,47 @@
 	}
 }
 
+static xfs_failaddr_t
+xfs_dinode_verify_fork(
+	struct xfs_dinode	*dip,
+	struct xfs_mount	*mp,
+	int			whichfork)
+{
+	uint32_t		di_nextents = XFS_DFORK_NEXTENTS(dip, whichfork);
+
+	switch (XFS_DFORK_FORMAT(dip, whichfork)) {
+	case XFS_DINODE_FMT_LOCAL:
+		/*
+		 * no local regular files yet
+		 */
+		if (whichfork == XFS_DATA_FORK) {
+			if (S_ISREG(be16_to_cpu(dip->di_mode)))
+				return __this_address;
+			if (be64_to_cpu(dip->di_size) >
+					XFS_DFORK_SIZE(dip, mp, whichfork))
+				return __this_address;
+		}
+		if (di_nextents)
+			return __this_address;
+		break;
+	case XFS_DINODE_FMT_EXTENTS:
+		if (di_nextents > XFS_DFORK_MAXEXT(dip, mp, whichfork))
+			return __this_address;
+		break;
+	case XFS_DINODE_FMT_BTREE:
+		if (whichfork == XFS_ATTR_FORK) {
+			if (di_nextents > MAXAEXTNUM)
+				return __this_address;
+		} else if (di_nextents > MAXEXTNUM) {
+			return __this_address;
+		}
+		break;
+	default:
+		return __this_address;
+	}
+	return NULL;
+}
+
 xfs_failaddr_t
 xfs_dinode_verify(
 	struct xfs_mount	*mp,
@@ -441,24 +482,9 @@
 	case S_IFREG:
 	case S_IFLNK:
 	case S_IFDIR:
-		switch (dip->di_format) {
-		case XFS_DINODE_FMT_LOCAL:
-			/*
-			 * no local regular files yet
-			 */
-			if (S_ISREG(mode))
-				return __this_address;
-			if (di_size > XFS_DFORK_DSIZE(dip, mp))
-				return __this_address;
-			if (dip->di_nextents)
-				return __this_address;
-			/* fall through */
-		case XFS_DINODE_FMT_EXTENTS:
-		case XFS_DINODE_FMT_BTREE:
-			break;
-		default:
-			return __this_address;
-		}
+		fa = xfs_dinode_verify_fork(dip, mp, XFS_DATA_FORK);
+		if (fa)
+			return fa;
 		break;
 	case 0:
 		/* Uninitialized inode ok. */
@@ -468,17 +494,9 @@
 	}
 
 	if (XFS_DFORK_Q(dip)) {
-		switch (dip->di_aformat) {
-		case XFS_DINODE_FMT_LOCAL:
-			if (dip->di_anextents)
-				return __this_address;
-		/* fall through */
-		case XFS_DINODE_FMT_EXTENTS:
-		case XFS_DINODE_FMT_BTREE:
-			break;
-		default:
-			return __this_address;
-		}
+		fa = xfs_dinode_verify_fork(dip, mp, XFS_ATTR_FORK);
+		if (fa)
+			return fa;
 	} else {
 		/*
 		 * If there is no fork offset, this may be a freshly-made inode
