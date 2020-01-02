cmake_minimum_required(VERSION 3.16)


add_library(EasyExif STATIC
	EasyExif_Exif.cpp
	EasyExif_Exif.h
)
target_include_directories (EasyExif PUBLIC .)
