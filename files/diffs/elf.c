--- files/before/elf.c	2020-12-03 05:28:09.833363618 +0530
+++ files/after/elf.c	2020-12-03 05:28:10.849526085 +0530
@@ -705,7 +705,7 @@
 		return false;
 	}
 	link_shdr = &bin->shdr[shdr->sh_link];
-	if (shdr->sh_size < 1) {
+	if (shdr->sh_size < 1 || shdr->sh_size > SIZE_MAX) {
 		return false;
 	}
 	Elf_(Verdef) *defs = calloc (shdr->sh_size, sizeof (char));
@@ -837,7 +837,7 @@
 	if (shdr->sh_link > bin->ehdr.e_shnum) {
 		return NULL;
 	}
-	if (shdr->sh_size < 1) {
+	if (shdr->sh_size < 1 || shdr->sh_size > SIZE_MAX) {
 		return NULL;
 	}
 	sdb = sdb_new0 ();
