package io.flutter.embedding.engine;

import android.view.Surface;
/* loaded from: classes.dex */
public class FlutterOverlaySurface {

    /* renamed from: id */
    private final int f5245id;
    private final Surface surface;

    public FlutterOverlaySurface(int i, Surface surface) {
        this.f5245id = i;
        this.surface = surface;
    }

    public int getId() {
        return this.f5245id;
    }

    public Surface getSurface() {
        return this.surface;
    }
}
