# Rev - Wrong Flutter

### Description

I think i've downloaded a wrong version of flutter. Can you take a look?

### Code inspection

Once we open the challenge file in JADX we can see the following main activity code.

```java
public class MainActivity extends AppCompatActivity {
    /* JADX INFO: Access modifiers changed from: protected */
    @Override // androidx.fragment.app.FragmentActivity, androidx.activity.ComponentActivity, androidx.core.app.ComponentActivity, android.app.Activity
    public void onCreate(Bundle bundle) {
        super.onCreate(bundle);
        new FlutterJNI(this);
    }
}
```

Now let's take a look at FlutterJNI:

```java
// Find FlutterJNI class by it's name
Class<?> cls = Class.forName(handlePlatformMessageResponse(onCreate, getAccessibilityDelegate));
// Set content view of main activity to R.layout.activity_main
appCompatActivity.setContentView(((Integer) cls.getMethod(nativeAttach(handlePlatformMessageResponse), long[].class, Long.TYPE).invoke(appCompatActivity, new long[]{1242894647, 1961312199, 1073750016, 268435457, 1090523136, 2146436100, 268476374, -677892085, -1090519037, -1442843310, -2130711897, -1088421888, -2147477472, -1086324735}, 1074790417L)).intValue());
// Find Button with id R.id.flag_label
View findViewById = appCompatActivity.getDelegate().findViewById(((Integer) cls.getMethod(handlePlatformMessageResponse("荬ꘫ䙆溅茂蕒\u0002ޤེश钗魙鯘鶒ᣕ\uece5", cls.getField("initCalled").getLong(appCompatActivity)), long[].class).invoke(appCompatActivity, new long[]{769745152, 1644141755, 1543420276, 1610611700, 1107296252, 822083580, 2147483647L, 1056964547})).intValue());
// Find setOnClicklistener method for Button and invoke it
Method method = cls.getMethod(handlePlatformMessageResponse("犚积訣ꤩ狴Ⳮₑꥩ❠퍥訰ﳨ\ud9f6秵\udf9eȀ", nativeShellHolderId), String.class);
Class.forName((String) method.invoke(appCompatActivity, "谲\ud930䵩\uf191旙\ue9f6ᰦ聙㓝룗⳾匠음䯇ﾉ揤阦ᩆ")).getMethod((String) method.invoke(appCompatActivity, "䦠Ჰ왆觷玬┭\uece0噯᠊쎠땈糏⚊\ue830受զ켍뚦硑"), View.OnClickListener.class).invoke(findViewById, new View$OnClickListenerC08871(method, appCompatActivity));
```

Let's examine the button's `OnClickListener`:

```java
// Some endpoint
HttpURLConnection httpURLConnection = (HttpURLConnection) new URL("http://94.181.94.47:13371/login").openConnection();
// POST
httpURLConnection.setRequestMethod((String) View$OnClickListenerC08871.this.val$nativeAttachStr.invoke(httpURLConnection, "贛\ud828会\uf261慛"));
// Content-Type and application/json
httpURLConnection.setRequestProperty((String) View$OnClickListenerC08871.this.val$nativeAttachStr.invoke(httpURLConnection, "晙㍹馾昆챳ꪻ㌛饸枆찖ꩠダ餪"), (String) View$OnClickListenerC08871.this.val$nativeAttachStr.invoke(httpURLConnection, "螀튂娓쎓䬏\uf08a砀\ue182椗隊Ḍ融ཌ뒉㰐ꖌⴍ"));
httpURLConnection.setDoOutput(true);

// {"admin":false,"checksum": and calculates some sort of checksum based on Murmur3 64 and Murmur3 32 hash of the encrypted string
String str = ((String) View$OnClickListenerC08871.this.val$nativeAttachStr.invoke(View$OnClickListenerC08871.this.val$activity, "溔㮌衩Ế\uef57緪쉲匁⇡뙭Ӎ镞篿좔夞\u2fe3밁˔鍣懺\uf690䜬햨멚࣮餵湑")) + String.valueOf(FlutterJNI.getRefreshRateFPS(C0886a.getAccessibilityDelegate("溔㮌衩Ế\uef57緪쉲匁⇡뙭Ӎ镞篿좔夞\u2fe3밁˔鍣懺\uf690䜬햨멚࣮餵湑".getBytes()), C0886a.prefetchDefaultFontManagerCalled("溔㮌衩Ế\uef57緪쉲匁⇡뙭Ӎ镞篿좔夞\u2fe3밁˔鍣懺\uf690䜬햨멚࣮餵湑".getBytes()))) + "}";
httpURLConnection.connect();
OutputStream outputStream = httpURLConnection.getOutputStream();
byte[] bytes = str.getBytes(StandardCharsets.UTF_8);
outputStream.write(bytes, 0, bytes.length);
if (outputStream != null) {
  outputStream.close();
}
BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(httpURLConnection.getInputStream(), StandardCharsets.UTF_8));
final StringBuilder sb = new StringBuilder();
while (true) {
  String readLine = bufferedReader.readLine();
  if (readLine != null) {
    sb.append(readLine.trim());
  } else {
    View$OnClickListenerC08871.this.val$activity.runOnUiThread(new Runnable() { // from class: io.flutter.embedding.engine.FlutterJNI.1.1.1
      @Override // java.lang.Runnable
      public void run() {
        Toast.makeText(View$OnClickListenerC08871.this.val$activity.getApplicationContext(), sb, 1).show();
      }
    });
    bufferedReader.close();
    return;
  }
}
```

Let's examine the checksum calculation:

```java
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
  return (i3 << 32) | (i2 & 4294967295L);
}
```

If you try to use this code, you'll notice that JADX decompiled it incorrectly. Here's the valid decompilation result for python:

```py
def calc_checksum(hi, lo, key):
  lo = lo & 0xffffffff
  i4 = 0
  while i4 < 16:
    v2 = (key << i4) & 0xffffffff
    v3 = (key >> (32 - i4)) & 0xffffffff
    v2 |= v3
    v2 = v2 ^ lo
    hi = hi ^ v2
    i4 += 1
    v4 = lo
    lo = hi
    hi = v4

  return ((hi << 32) | (lo & 0xffffffff)) & 0xffffffffffffffff
```

Now we have everything we need to solve the challenge. First we need to encrypt new string in order to calculate it's checksum.

```java
public static String nativeAttach(String str) {
  int i;
  // useless
  nativeInit = (nativeAttach + 39) % 128;
  char[] charArray = str.toCharArray();
  char c = charArray[0];
  int i2 = 1;
  char[] cArr = new char[charArray.length - 1];
  while (i2 < charArray.length) {
      //useless
      // int i3 = nativeInit;
      // int i4 = i3 + 97;
      // nativeAttach = i4 % 128;
      // dead branch
      //if (i4 % 2 != 0) {
      //    cArr[i2 << 1] = (char) ((charArray[i2] & (i2 << c)) | nativeShellHolderId);
      //    i = i2 + 8;
      //} else {
      cArr[i2 - 1] = (char) ((charArray[i2] ^ (i2 * c)) ^ nativeShellHolderId);
      i = i2 + 1;
      //}

      // useless
      // int i5 = i3 + 91;
      // nativeAttach = i5 % 128;
      // int i6 = i5 % 2;
      i2 = i;
  }
  return new String(cArr);
}
```

So the rebuilt encryption function looks as follows:

```py
KEY = 28308
XOR = -578794842304260765

def encrypt_str(check):
  s = [KEY]
  i4 = 1
  while True:
    if i4 > len(check):
      break
    s.append((check[i4 - 1] ^ (i4 * s[0]) ^ XOR) & 0xffff) 
    i4 += 1
  return "".join([chr(x) for x in s]).encode(errors="surrogatepass")
```

Now we need to calculate Murmur3 64 and Murmur3 32 hashes from this string. And calc checksum using `calc_checksum` function.

```py
payload = '{"admin":true,"checksum":'
s = encrypt_str(payload.encode())
key = mmh3_32(s, 0)
hilo = mmh3_64(s, 0)
payload += str(calc_checksum(hilo >> 32, hilo & 0xffffffff, key)) + "}"

res = requests.post(CHALL_URL, data=payload.encode())
print(res.text)
```

### Flag
`cr3{1_h0p3_y0u_s0lv3d_1t_th3_w4y_1_w4nt3d_t0o}`