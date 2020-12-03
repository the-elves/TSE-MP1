--- files/before/ImageBitmapFactories.cpp	2020-12-03 05:31:48.436314451 +0530
+++ files/after/ImageBitmapFactories.cpp	2020-12-03 05:31:49.496483953 +0530
@@ -67,7 +67,12 @@
 {
     RefPtr<ScriptPromiseResolver> resolver = ScriptPromiseResolver::create(context);
     ScriptPromise promise = resolver->promise();
-    resolver->resolve(imageBitmap);
+    if (imageBitmap) {
+        resolver->resolve(imageBitmap);
+    } else {
+        v8::Isolate* isolate = ScriptState::current()->isolate();
+        resolver->reject(ScriptValue(v8::Null(isolate), isolate));
+    }
     return promise;
 }
 
@@ -185,8 +190,9 @@
         exceptionState.throwDOMException(IndexSizeError, String::format("The source %s provided is 0.", sw ? "height" : "width"));
         return ScriptPromise();
     }
+
     // FIXME: make ImageBitmap creation asynchronous crbug.com/258082
-    return fulfillImageBitmap(eventTarget.executionContext(), ImageBitmap::create(canvas, IntRect(sx, sy, sw, sh)));
+    return fulfillImageBitmap(eventTarget.executionContext(), canvas->buffer() ? ImageBitmap::create(canvas, IntRect(sx, sy, sw, sh)) : nullptr);
 }
 
 ScriptPromise ImageBitmapFactories::createImageBitmap(EventTarget& eventTarget, Blob* blob, ExceptionState& exceptionState)
