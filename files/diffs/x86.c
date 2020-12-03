--- files/before/x86.c	2020-12-03 05:27:02.386580044 +0530
+++ files/after/x86.c	2020-12-03 05:27:03.342732915 +0530
@@ -1959,6 +1959,11 @@
 		/* ...but clean it before doing the actual write */
 		vcpu->arch.time_offset = data & ~(PAGE_MASK | 1);
 
+		/* Check that the address is 32-byte aligned. */
+		if (vcpu->arch.time_offset &
+				(sizeof(struct pvclock_vcpu_time_info) - 1))
+			break;
+
 		vcpu->arch.time_page =
 				gfn_to_page(vcpu->kvm, data >> PAGE_SHIFT);
 
