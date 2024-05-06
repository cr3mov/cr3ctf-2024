@echo off
SET PATH=%PATH%;D:\Go\bin\
pushd %~dp0proto
for %%f in (*.proto) do (

	..\protoc --js_out=import_style=commonjs,binary:../ %%f
)
popd
browserify main.js -o bundle.js