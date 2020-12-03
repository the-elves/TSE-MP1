--- files/before/uio.c	2020-12-03 05:26:46.980116816 +0530
+++ files/after/uio.c	2020-12-03 05:26:47.920267130 +0530
@@ -642,16 +642,29 @@
 {
 	struct uio_device *idev = vma->vm_private_data;
 	int mi = uio_find_mem_index(vma);
+	struct uio_mem *mem;
 	if (mi < 0)
 		return -EINVAL;
+	mem = idev->info->mem + mi;
 
-	vma->vm_ops = &uio_physical_vm_ops;
+	if (vma->vm_end - vma->vm_start > mem->size)
+		return -EINVAL;
 
+	vma->vm_ops = &uio_physical_vm_ops;
 	vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
 
+	/*
+	 * We cannot use the vm_iomap_memory() helper here,
+	 * because vma->vm_pgoff is the map index we looked
+	 * up above in uio_find_mem_index(), rather than an
+	 * actual page offset into the mmap.
+	 *
+	 * So we just do the physical mmap without a page
+	 * offset.
+	 */
 	return remap_pfn_range(vma,
 			       vma->vm_start,
-			       idev->info->mem[mi].addr >> PAGE_SHIFT,
+			       mem->addr >> PAGE_SHIFT,
 			       vma->vm_end - vma->vm_start,
 			       vma->vm_page_prot);
 }
