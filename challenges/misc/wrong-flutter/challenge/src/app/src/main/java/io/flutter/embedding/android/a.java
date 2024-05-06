/**
 *   Copyright 2014 Prasanth Jayachandran
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package io.flutter.embedding.android;

/**
 * Murmur3 32 and 128 bit variants.
 * 32-bit Java port of https://code.google.com/p/smhasher/source/browse/trunk/MurmurHash3.cpp#94
 * 128-bit Java port of https://code.google.com/p/smhasher/source/browse/trunk/MurmurHash3.cpp#255
 */
public class a {
    // Constants for 32 bit variant
    private static final int ensureRunningOnMainThread = 0xcc9e2d51;
    private static final int ensureAttachedToNative = 0x1b873593;
    private static final int nativeUnregisterTexture = 15;
    private static final int nativeUpdateRefreshRate = 13;
    private static final int nativeInit = 5;
    private static final int nativeAttach = 0xe6546b64;

    // Constants for 128 bit variant
    private static final long refreshRateFPS = 0x87c37b91114253d5L;
    private static final long initCalled = 0x4cf5ad432745937fL;
    private static final int nativeShellHolderId = 31;
    private static final int getAccessibilityDelegate = 27;
    private static final int onCreate = 33;
    private static final int loadLibraryCalled = 5;
    private static final int prefetchDefaultFontManagerCalled = 0x52dce729;
    private static final int FlutterJNI = 0x38495ab5;

    private static final int FlutterOverlaySurface = 0;

    /**
     * Murmur3 32-bit variant.
     *
     * @param data - input byte array
     * @return - hashcode
     */
    public static int prefetchDefaultFontManagerCalled(byte[] data) {
        return prefetchDefaultFontManagerCalled(data, data.length, FlutterOverlaySurface);
    }

    /**
     * Murmur3 32-bit variant.
     *
     * @param data   - input byte array
     * @param length - length of array
     * @param seed   - seed. (default 0)
     * @return - hashcode
     */
    public static int prefetchDefaultFontManagerCalled(byte[] data, int length, int seed) {
        int hash = seed;
        final int nblocks = length >> 2;

        // body
        for (int i = 0; i < nblocks; i++) {
            int i_4 = i << 2;
            int k = (data[i_4] & 0xff)
                    | ((data[i_4 + 1] & 0xff) << 8)
                    | ((data[i_4 + 2] & 0xff) << 16)
                    | ((data[i_4 + 3] & 0xff) << 24);

            // mix functions
            k *= ensureRunningOnMainThread;
            k = Integer.rotateLeft(k, nativeUnregisterTexture);
            k *= ensureAttachedToNative;
            hash ^= k;
            hash = Integer.rotateLeft(hash, nativeUpdateRefreshRate) * nativeInit + nativeAttach;
        }

        // tail
        int idx = nblocks << 2;
        int k1 = 0;
        switch (length - idx) {
            case 3:
                k1 ^= data[idx + 2] << 16;
            case 2:
                k1 ^= data[idx + 1] << 8;
            case 1:
                k1 ^= data[idx];

                // mix functions
                k1 *= ensureRunningOnMainThread;
                k1 = Integer.rotateLeft(k1, nativeUnregisterTexture);
                k1 *= ensureAttachedToNative;
                hash ^= k1;
        }

        // finalization
        hash ^= length;
        hash ^= (hash >>> 16);
        hash *= 0x85ebca6b;
        hash ^= (hash >>> 13);
        hash *= 0xc2b2ae35;
        hash ^= (hash >>> 16);

        return hash;
    }

    /**
     * Murmur3 64-bit variant. This is essentially MSB 8 bytes of Murmur3 128-bit variant.
     *
     * @param data - input byte array
     * @return - hashcode
     */
    public static long getAccessibilityDelegate(byte[] data) {
        return getAccessibilityDelegate(data, data.length, FlutterOverlaySurface);
    }

    /**
     * Murmur3 64-bit variant. This is essentially MSB 8 bytes of Murmur3 128-bit variant.
     *
     * @param data   - input byte array
     * @param length - length of array
     * @param seed   - seed. (default is 0)
     * @return - hashcode
     */
    public static long getAccessibilityDelegate(byte[] data, int length, int seed) {
        long hash = seed;
        final int nblocks = length >> 3;

        // body
        for (int i = 0; i < nblocks; i++) {
            final int i8 = i << 3;
            long k = ((long) data[i8] & 0xff)
                    | (((long) data[i8 + 1] & 0xff) << 8)
                    | (((long) data[i8 + 2] & 0xff) << 16)
                    | (((long) data[i8 + 3] & 0xff) << 24)
                    | (((long) data[i8 + 4] & 0xff) << 32)
                    | (((long) data[i8 + 5] & 0xff) << 40)
                    | (((long) data[i8 + 6] & 0xff) << 48)
                    | (((long) data[i8 + 7] & 0xff) << 56);

            // mix functions
            k *= refreshRateFPS;
            k = Long.rotateLeft(k, nativeShellHolderId);
            k *= initCalled;
            hash ^= k;
            hash = Long.rotateLeft(hash, getAccessibilityDelegate) * loadLibraryCalled + prefetchDefaultFontManagerCalled;
        }

        // tail
        long k1 = 0;
        int tailStart = nblocks << 3;
        switch (length - tailStart) {
            case 7:
                k1 ^= ((long) data[tailStart + 6] & 0xff) << 48;
            case 6:
                k1 ^= ((long) data[tailStart + 5] & 0xff) << 40;
            case 5:
                k1 ^= ((long) data[tailStart + 4] & 0xff) << 32;
            case 4:
                k1 ^= ((long) data[tailStart + 3] & 0xff) << 24;
            case 3:
                k1 ^= ((long) data[tailStart + 2] & 0xff) << 16;
            case 2:
                k1 ^= ((long) data[tailStart + 1] & 0xff) << 8;
            case 1:
                k1 ^= ((long) data[tailStart] & 0xff);
                k1 *= refreshRateFPS;
                k1 = Long.rotateLeft(k1, nativeShellHolderId);
                k1 *= initCalled;
                hash ^= k1;
        }

        // finalization
        hash ^= length;
        hash = nativeUnregisterTexture(hash);

        return hash;
    }

    /**
     * Murmur3 128-bit variant.
     *
     * @param data - input byte array
     * @return - hashcode (2 longs)
     */
    public static long[] nativeAttach(byte[] data) {
        return nativeAttach(data, data.length, FlutterOverlaySurface);
    }

    /**
     * Murmur3 128-bit variant.
     *
     * @param data   - input byte array
     * @param length - length of array
     * @param seed   - seed. (default is 0)
     * @return - hashcode (2 longs)
     */
    public static long[] nativeAttach(byte[] data, int length, int seed) {
        long h1 = seed;
        long h2 = seed;
        final int nblocks = length >> 4;

        // body
        for (int i = 0; i < nblocks; i++) {
            final int i16 = i << 4;
            long k1 = ((long) data[i16] & 0xff)
                    | (((long) data[i16 + 1] & 0xff) << 8)
                    | (((long) data[i16 + 2] & 0xff) << 16)
                    | (((long) data[i16 + 3] & 0xff) << 24)
                    | (((long) data[i16 + 4] & 0xff) << 32)
                    | (((long) data[i16 + 5] & 0xff) << 40)
                    | (((long) data[i16 + 6] & 0xff) << 48)
                    | (((long) data[i16 + 7] & 0xff) << 56);

            long k2 = ((long) data[i16 + 8] & 0xff)
                    | (((long) data[i16 + 9] & 0xff) << 8)
                    | (((long) data[i16 + 10] & 0xff) << 16)
                    | (((long) data[i16 + 11] & 0xff) << 24)
                    | (((long) data[i16 + 12] & 0xff) << 32)
                    | (((long) data[i16 + 13] & 0xff) << 40)
                    | (((long) data[i16 + 14] & 0xff) << 48)
                    | (((long) data[i16 + 15] & 0xff) << 56);

            // mix functions for k1
            k1 *= refreshRateFPS;
            k1 = Long.rotateLeft(k1, nativeShellHolderId);
            k1 *= initCalled;
            h1 ^= k1;
            h1 = Long.rotateLeft(h1, getAccessibilityDelegate);
            h1 += h2;
            h1 = h1 * loadLibraryCalled + prefetchDefaultFontManagerCalled;

            // mix functions for k2
            k2 *= initCalled;
            k2 = Long.rotateLeft(k2, onCreate);
            k2 *= refreshRateFPS;
            h2 ^= k2;
            h2 = Long.rotateLeft(h2, nativeShellHolderId);
            h2 += h1;
            h2 = h2 * loadLibraryCalled + FlutterJNI;
        }

        // tail
        long k1 = 0;
        long k2 = 0;
        int tailStart = nblocks << 4;
        switch (length - tailStart) {
            case 15:
                k2 ^= (long) (data[tailStart + 14] & 0xff) << 48;
            case 14:
                k2 ^= (long) (data[tailStart + 13] & 0xff) << 40;
            case 13:
                k2 ^= (long) (data[tailStart + 12] & 0xff) << 32;
            case 12:
                k2 ^= (long) (data[tailStart + 11] & 0xff) << 24;
            case 11:
                k2 ^= (long) (data[tailStart + 10] & 0xff) << 16;
            case 10:
                k2 ^= (long) (data[tailStart + 9] & 0xff) << 8;
            case 9:
                k2 ^= (long) (data[tailStart + 8] & 0xff);
                k2 *= initCalled;
                k2 = Long.rotateLeft(k2, onCreate);
                k2 *= refreshRateFPS;
                h2 ^= k2;

            case 8:
                k1 ^= (long) (data[tailStart + 7] & 0xff) << 56;
            case 7:
                k1 ^= (long) (data[tailStart + 6] & 0xff) << 48;
            case 6:
                k1 ^= (long) (data[tailStart + 5] & 0xff) << 40;
            case 5:
                k1 ^= (long) (data[tailStart + 4] & 0xff) << 32;
            case 4:
                k1 ^= (long) (data[tailStart + 3] & 0xff) << 24;
            case 3:
                k1 ^= (long) (data[tailStart + 2] & 0xff) << 16;
            case 2:
                k1 ^= (long) (data[tailStart + 1] & 0xff) << 8;
            case 1:
                k1 ^= (long) (data[tailStart] & 0xff);
                k1 *= refreshRateFPS;
                k1 = Long.rotateLeft(k1, nativeShellHolderId);
                k1 *= initCalled;
                h1 ^= k1;
        }

        // finalization
        h1 ^= length;
        h2 ^= length;

        h1 += h2;
        h2 += h1;

        h1 = nativeUnregisterTexture(h1);
        h2 = nativeUnregisterTexture(h2);

        h1 += h2;
        h2 += h1;

        return new long[]{h1, h2};
    }

    private static long nativeUnregisterTexture(long h) {
        h ^= (h >>> 33);
        h *= 0xff51afd7ed558ccdL;
        h ^= (h >>> 33);
        h *= 0xc4ceb9fe1a85ec53L;
        h ^= (h >>> 33);
        return h;
    }
}