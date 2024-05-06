lib /out:libucrt-removed.lib /remove:d:\os\obj\amd64fre\minkernel\crts\ucrt\src\appcrt\dll\mt\..\..\startup\mt\objfre\amd64\argv_data.obj libucrt.lib
lib /out:libucrt-patched.lib libucrt-removed.lib ../../libucrt-patch/x64/Release/argv_data.obj
lib /out:libucrt-patched.lib libucrt-removed.lib argv_data.obj
