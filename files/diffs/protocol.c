--- files/before/protocol.c	2020-12-03 05:27:22.981872876 +0530
+++ files/after/protocol.c	2020-12-03 05:27:23.882016793 +0530
@@ -1186,7 +1186,7 @@
 	unregister_inetaddr_notifier(&sctp_inetaddr_notifier);
 }
 
-static int __net_init sctp_net_init(struct net *net)
+static int __net_init sctp_defaults_init(struct net *net)
 {
 	int status;
 
@@ -1279,12 +1279,6 @@
 
 	sctp_dbg_objcnt_init(net);
 
-	/* Initialize the control inode/socket for handling OOTB packets.  */
-	if ((status = sctp_ctl_sock_init(net))) {
-		pr_err("Failed to initialize the SCTP control sock\n");
-		goto err_ctl_sock_init;
-	}
-
 	/* Initialize the local address list. */
 	INIT_LIST_HEAD(&net->sctp.local_addr_list);
 	spin_lock_init(&net->sctp.local_addr_lock);
@@ -1300,9 +1294,6 @@
 
 	return 0;
 
-err_ctl_sock_init:
-	sctp_dbg_objcnt_exit(net);
-	sctp_proc_exit(net);
 err_init_proc:
 	cleanup_sctp_mibs(net);
 err_init_mibs:
@@ -1311,15 +1302,12 @@
 	return status;
 }
 
-static void __net_exit sctp_net_exit(struct net *net)
+static void __net_exit sctp_defaults_exit(struct net *net)
 {
 	/* Free the local address list */
 	sctp_free_addr_wq(net);
 	sctp_free_local_addr_list(net);
 
-	/* Free the control endpoint.  */
-	inet_ctl_sock_destroy(net->sctp.ctl_sock);
-
 	sctp_dbg_objcnt_exit(net);
 
 	sctp_proc_exit(net);
@@ -1327,9 +1315,32 @@
 	sctp_sysctl_net_unregister(net);
 }
 
-static struct pernet_operations sctp_net_ops = {
-	.init = sctp_net_init,
-	.exit = sctp_net_exit,
+static struct pernet_operations sctp_defaults_ops = {
+	.init = sctp_defaults_init,
+	.exit = sctp_defaults_exit,
+};
+
+static int __net_init sctp_ctrlsock_init(struct net *net)
+{
+	int status;
+
+	/* Initialize the control inode/socket for handling OOTB packets.  */
+	status = sctp_ctl_sock_init(net);
+	if (status)
+		pr_err("Failed to initialize the SCTP control sock\n");
+
+	return status;
+}
+
+static void __net_init sctp_ctrlsock_exit(struct net *net)
+{
+	/* Free the control endpoint.  */
+	inet_ctl_sock_destroy(net->sctp.ctl_sock);
+}
+
+static struct pernet_operations sctp_ctrlsock_ops = {
+	.init = sctp_ctrlsock_init,
+	.exit = sctp_ctrlsock_exit,
 };
 
 /* Initialize the universe into something sensible.  */
@@ -1462,8 +1473,11 @@
 	sctp_v4_pf_init();
 	sctp_v6_pf_init();
 
-	status = sctp_v4_protosw_init();
+	status = register_pernet_subsys(&sctp_defaults_ops);
+	if (status)
+		goto err_register_defaults;
 
+	status = sctp_v4_protosw_init();
 	if (status)
 		goto err_protosw_init;
 
@@ -1471,9 +1485,9 @@
 	if (status)
 		goto err_v6_protosw_init;
 
-	status = register_pernet_subsys(&sctp_net_ops);
+	status = register_pernet_subsys(&sctp_ctrlsock_ops);
 	if (status)
-		goto err_register_pernet_subsys;
+		goto err_register_ctrlsock;
 
 	status = sctp_v4_add_protocol();
 	if (status)
@@ -1489,12 +1503,14 @@
 err_v6_add_protocol:
 	sctp_v4_del_protocol();
 err_add_protocol:
-	unregister_pernet_subsys(&sctp_net_ops);
-err_register_pernet_subsys:
+	unregister_pernet_subsys(&sctp_ctrlsock_ops);
+err_register_ctrlsock:
 	sctp_v6_protosw_exit();
 err_v6_protosw_init:
 	sctp_v4_protosw_exit();
 err_protosw_init:
+	unregister_pernet_subsys(&sctp_defaults_ops);
+err_register_defaults:
 	sctp_v4_pf_exit();
 	sctp_v6_pf_exit();
 	sctp_sysctl_unregister();
@@ -1527,12 +1543,14 @@
 	sctp_v6_del_protocol();
 	sctp_v4_del_protocol();
 
-	unregister_pernet_subsys(&sctp_net_ops);
+	unregister_pernet_subsys(&sctp_ctrlsock_ops);
 
 	/* Free protosw registrations */
 	sctp_v6_protosw_exit();
 	sctp_v4_protosw_exit();
 
+	unregister_pernet_subsys(&sctp_defaults_ops);
+
 	/* Unregister with socket layer. */
 	sctp_v6_pf_exit();
 	sctp_v4_pf_exit();
