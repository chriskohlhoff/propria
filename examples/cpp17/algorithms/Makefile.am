AUTOMAKE_OPTIONS = subdir-objects

noinst_PROGRAMS = \
	fill_1 \
	fill_2 \
	fill_3 \
	fill_4 \
	for_each_1 \
	for_each_2 \
	for_each_3

fill_1_SOURCES = fill_1.cpp
fill_2_SOURCES = fill_2.cpp
fill_3_SOURCES = fill_3.cpp
fill_4_SOURCES = fill_4.cpp
for_each_1_SOURCES = for_each_1.cpp
for_each_2_SOURCES = for_each_2.cpp
for_each_3_SOURCES = for_each_3.cpp

AM_CXXFLAGS = -I$(srcdir)/../../../include

MAINTAINERCLEANFILES = \
	$(srcdir)/Makefile.in

EXTRA_DIST = \
	execution.hpp \
	executor_property.hpp \
	extension_property.hpp \
	fill_1.hpp \
	fill_2.hpp \
	fill_3.hpp \
	fill_4.hpp \
	for_each_1.hpp \
	for_each_2.hpp \
	for_each_3.hpp
