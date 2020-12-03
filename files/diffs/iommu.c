--- files/before/iommu.c	2020-12-03 05:27:13.768399799 +0530
+++ files/after/iommu.c	2020-12-03 05:27:14.720552032 +0530
@@ -61,6 +61,14 @@
 	return pfn;
 }
 
+static void kvm_unpin_pages(struct kvm *kvm, pfn_t pfn, unsigned long npages)
+{
+	unsigned long i;
+
+	for (i = 0; i < npages; ++i)
+		kvm_release_pfn_clean(pfn + i);
+}
+
 int kvm_iommu_map_pages(struct kvm *kvm, struct kvm_memory_slot *slot)
 {
 	gfn_t gfn, end_gfn;
@@ -123,6 +131,7 @@
 		if (r) {
 			printk(KERN_ERR "kvm_iommu_map_address:"
 			       "iommu failed to map pfn=%llx\n", pfn);
+			kvm_unpin_pages(kvm, pfn, page_size);
 			goto unmap_pages;
 		}
 
@@ -134,7 +143,7 @@
 	return 0;
 
 unmap_pages:
-	kvm_iommu_put_pages(kvm, slot->base_gfn, gfn);
+	kvm_iommu_put_pages(kvm, slot->base_gfn, gfn - slot->base_gfn);
 	return r;
 }
 
@@ -266,14 +275,6 @@
 	return r;
 }
 
-static void kvm_unpin_pages(struct kvm *kvm, pfn_t pfn, unsigned long npages)
-{
-	unsigned long i;
-
-	for (i = 0; i < npages; ++i)
-		kvm_release_pfn_clean(pfn + i);
-}
-
 static void kvm_iommu_put_pages(struct kvm *kvm,
 				gfn_t base_gfn, unsigned long npages)
 {
