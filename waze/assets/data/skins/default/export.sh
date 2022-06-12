rmdir tmp
mkdir tmp
cp *.png tmp/
rename -f 's/.png$/.bin/' tmp/*.png
adb push tmp /sdcard/waze/skins/default
