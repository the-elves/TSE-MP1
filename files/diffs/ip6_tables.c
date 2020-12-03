--- files/before/ip6_tables.c	2020-12-03 05:27:55.151016164 +0530
+++ files/after/ip6_tables.c	2020-12-03 05:27:56.091166478 +0530
@@ -198,11 +198,12 @@
 
 /* All zeroes == unconditional rule. */
 /* Mildly perf critical (only if packet tracing is on) */
-static inline bool unconditional(const struct ip6t_ip6 *ipv6)
+static inline bool unconditional(const struct ip6t_entry *e)
 {
 	static const struct ip6t_ip6 uncond;
 
-	return memcmp(ipv6, &uncond, sizeof(uncond)) == 0;
+	return e->target_offset == sizeof(struct ip6t_entry) &&
+	       memcmp(&e->ipv6, &uncond, sizeof(uncond)) == 0;
 }
 
 static inline const struct xt_entry_target *
@@ -258,11 +259,10 @@
 	} else if (s == e) {
 		(*rulenum)++;
 
-		if (s->target_offset == sizeof(struct ip6t_entry) &&
+		if (unconditional(s) &&
 		    strcmp(t->target.u.kernel.target->name,
 			   XT_STANDARD_TARGET) == 0 &&
-		    t->verdict < 0 &&
-		    unconditional(&s->ipv6)) {
+		    t->verdict < 0) {
 			/* Tail of chains: STANDARD target (return/policy) */
 			*comment = *chainname == hookname
 				? comments[NF_IP6_TRACE_COMMENT_POLICY]
@@ -488,11 +488,10 @@
 			e->comefrom |= ((1 << hook) | (1 << NF_INET_NUMHOOKS));
 
 			/* Unconditional return/END. */
-			if ((e->target_offset == sizeof(struct ip6t_entry) &&
+			if ((unconditional(e) &&
 			     (strcmp(t->target.u.user.name,
 				     XT_STANDARD_TARGET) == 0) &&
-			     t->verdict < 0 &&
-			     unconditional(&e->ipv6)) || visited) {
+			     t->verdict < 0) || visited) {
 				unsigned int oldpos, size;
 
 				if ((strcmp(t->target.u.user.name,
@@ -727,7 +726,7 @@
 	const struct xt_entry_target *t;
 	unsigned int verdict;
 
-	if (!unconditional(&e->ipv6))
+	if (!unconditional(e))
 		return false;
 	t = ip6t_get_target_c(e);
 	if (strcmp(t->u.user.name, XT_STANDARD_TARGET) != 0)
@@ -775,9 +774,9 @@
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
