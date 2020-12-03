--- files/before/CCLayerTreeHost.cpp	2020-12-03 05:30:18.521938697 +0530
+++ files/after/CCLayerTreeHost.cpp	2020-12-03 05:30:19.534100523 +0530
@@ -84,8 +84,6 @@
     // Update m_settings based on capabilities that we got back from the renderer.
     m_settings.acceleratePainting = m_proxy->layerRendererCapabilities().usingAcceleratedPainting;
 
-    setNeedsCommitThenRedraw();
-
     m_contentsTextureManager = TextureManager::create(TextureManager::highLimitBytes(), m_proxy->layerRendererCapabilities().maxTextureSize);
     return true;
 }
