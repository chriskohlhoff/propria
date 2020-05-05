AUTOMAKE_OPTIONS = subdir-objects

noinst_PROGRAMS = \
	allocators/query/main \
	allocators/query_with_fallback/main \
	allocators/require/main \
	allocators/prefer/main \
	allocators/require_concept/main

allocators_query_main_SOURCES = allocators/query/main.cpp
allocators_query_with_fallback_main_SOURCES = allocators/query_with_fallback/main.cpp
allocators_require_main_SOURCES = allocators/require/main.cpp
allocators_prefer_main_SOURCES = allocators/prefer/main.cpp
allocators_require_concept_main_SOURCES = allocators/require_concept/main.cpp

EXTRA_DIST = \
	allocators/query/alignment.hpp \
	allocators/query/global_allocator.hpp \
	allocators/query/allocator.hpp \
	allocators/query/monotonic_allocator.hpp \
	allocators/query/algorithm.hpp \
	allocators/query_with_fallback/alignment.hpp \
	allocators/query_with_fallback/global_allocator.hpp \
	allocators/query_with_fallback/allocator.hpp \
	allocators/query_with_fallback/monotonic_allocator.hpp \
	allocators/query_with_fallback/algorithm.hpp \
	allocators/require/alignment.hpp \
	allocators/require/global_allocator.hpp \
	allocators/require/allocator.hpp \
	allocators/require/monotonic_allocator.hpp \
	allocators/require/algorithm.hpp
	allocators/prefer/alignment.hpp \
	allocators/prefer/global_allocator.hpp \
	allocators/prefer/allocator.hpp \
	allocators/prefer/monotonic_allocator.hpp \
	allocators/prefer/algorithm.hpp \
	allocators/require_concept/global_allocator.hpp \
	allocators/require_concept/allocator.hpp \
	allocators/require_concept/monotonic_allocator.hpp \
	allocators/require_concept/algorithm.hpp

AM_CXXFLAGS = -I$(srcdir)/../../include

MAINTAINERCLEANFILES = \
	$(srcdir)/Makefile.in
