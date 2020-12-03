--- files/before/macos_main.c	2020-12-03 05:33:07.732992619 +0530
+++ files/after/macos_main.c	2020-12-03 05:33:08.697146770 +0530
@@ -1 +1 @@
-#include <GUSICommandLine.h>
+#include <GUSICommandLine.h>#include <stdlib.h>#undef mainDECLARE_MAIN(test)REGISTER_MAIN_STARTREGISTER_MAIN(test)REGISTER_MAIN_ENDint main(){	(void) exec_commands();		return 0;}
\ No newline at end of file
