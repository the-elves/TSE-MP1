--- files/before/arp_tables.c	2020-12-03 05:27:50.546279950 +0530
+++ files/after/arp_tables.c	2020-12-03 05:27:52.298560107 +0530
@@ -359,11 +359,12 @@
 }
 
 /* All zeroes == unconditional rule. */
-static inline bool unconditional(const struct arpt_arp *arp)
+static inline bool unconditional(const struct arpt_entry *e)
 {
 	static const struct arpt_arp uncond;
 
-	return memcmp(arp, &uncond, sizeof(uncond)) == 0;
+	return e->target_offset == sizeof(struct arpt_entry) &&
+	       memcmp(&e->arp, &uncond, sizeof(uncond)) == 0;
 }
 
 /* Figures out from what hook each rule can be called: returns 0 if
@@ -402,11 +403,10 @@
 				|= ((1 << hook) | (1 << NF_ARP_NUMHOOKS));
 
 			/* Unconditional return/END. */
-			if ((e->target_offset == sizeof(struct arpt_entry) &&
+			if ((unconditional(e) &&
 			     (strcmp(t->target.u.user.name,
 				     XT_STANDARD_TARGET) == 0) &&
-			     t->verdict < 0 && unconditional(&e->arp)) ||
-			    visited) {
+			     t->verdict < 0) || visited) {
 				unsigned int oldpos, size;
 
 				if ((strcmp(t->target.u.user.name,
@@ -551,7 +551,7 @@
 	const struct xt_entry_target *t;
 	unsigned int verdict;
 
-	if (!unconditional(&e->arp))
+	if (!unconditional(e))
 		return false;
 	t = arpt_get_target_c(e);
 	if (strcmp(t->u.user.name, XT_STANDARD_TARGET) != 0)
@@ -598,9 +598,9 @@
 			newinfo->hook_entry[h] = hook_entries[h];
 		if ((unsigned char *)e - base == underflows[h]) {
 			if (!check_underflow(e)) {
-				pr_err("Underflows must be unconditional and "
-				       "use the STANDARD target with "
-				       "ACCEPT/DROP\n");
+				pr_debug("Underflows must be unconditional and "
+					 "use the STANDARD target with "
+					 "ACCEPT/DROP\n");
 				return -EINVAL;
 			}
 			newinfo->underflow[h] = underflows[h];
