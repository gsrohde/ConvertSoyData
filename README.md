# ConvertSoyData
Demonstrates writing XML

To build all targets, just run
```
make
```
(But see Compilation Notes below.)

ConvertSoyData takes a tab-separated-value input file and converts it to an XML file.  The input file to use is 
https://github.com/ebimodeling/VersionControl_PlantGro/blob/master/CleanUp/ReadData/2002SoyData.  Put this file in the directory with the executable and run the command as
```
./ConvertSoyData 2002SoyData 2002SoyData.xml
```
(Run
```
./ConvertSoyData --help
```
for usage information.)

If you have no command line arguments, input comes from standard input and the output file defaults to "output.xml".

`convert_soy_data.c` is fully commented so that you should be able to glean from it the basics of using the libxml2 API to write out XML.



BarebonesConvertSoyData is a stripped-down version of ConvertSoyData with hard-coded input and output file names ("2002SoyData" and "output.xml", respectively) and _NO_ error checking.  
This is to make the basic libxml2 usage clearer.  Given the same input file, it should produce exactly the same output as ConvertSoyData.

# Compilation Notes

You need to have libxml2 libraries installed to compile this.  Moreover, the makefile assumes the `xml2-config` utility is on your path and that you are running `make` under a shell that can do command substitution whereby `` `command` `` is replaced by the output of `command`.  If you have libxml2 but command substitution doesn't work, tweak the make file by replacing
```
`xml2-config --cflags`
```
with
```
-I<path to include directory for libxml2>
```
and
```
`xml2-config --libs`
```
with
```
-l<libxml2 library name> (probably xml2)
```

You may still be able to run xml2-config on the command line to find out the exact values to use on your machine even if running it from within the makefile doesn't work.
