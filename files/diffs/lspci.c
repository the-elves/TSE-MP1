--- files/before/lspci.c	2020-12-03 05:29:59.850953528 +0530
+++ files/after/lspci.c	2020-12-03 05:30:00.707090409 +0530
@@ -1,7 +1,8 @@
 /*  -*- c-basic-offset: 8 -*-
    rdesktop: A Remote Desktop Protocol client.
    Support for the Matrox "lspci" channel
-   Copyright (C) 2005 Matrox Graphics Inc. 
+   Copyright (C) 2005 Matrox Graphics Inc.
+   Copyright 2018 Henrik Andersson <hean01@cendio.se> for Cendio AB
 
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
@@ -134,6 +135,12 @@
 	unsigned int pkglen;
 	static char *rest = NULL;
 	char *buf;
+	struct stream packet = *s;
+
+	if (!s_check(s))
+	{
+		rdp_protocol_error("lspci_process(), stream is in unstable state", &packet);
+	}
 
 	pkglen = s->end - s->p;
 	/* str_handle_lines requires null terminated strings */
