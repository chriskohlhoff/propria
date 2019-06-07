AUTOMAKE_OPTIONS = subdir-objects

if HAVE_CXX17
if HAVE_CONCEPTS_TS

noinst_PROGRAMS = \
	cpp17/trivial_executors/inline_1 \
	cpp17/trivial_executors/perf \
	cpp17/executors_with_sub_executors/inline_1 \
	cpp17/executors_with_sub_executors/perf \
	cpp17/executors_with_sub_executors/thread_pool \
	cpp17/sender_executors/inline_1 \
	cpp17/sender_executors/lazy \
	cpp17/sender_executors/perf \
	cpp17/sender_executors/thread_pool

cpp17_trivial_executors_inline_1_SOURCES = cpp17/trivial_executors/inline_1.cpp
cpp17_trivial_executors_perf_SOURCES = cpp17/trivial_executors/perf.cpp
cpp17_executors_with_sub_executors_inline_1_SOURCES = cpp17/executors_with_sub_executors/inline_1.cpp
cpp17_executors_with_sub_executors_perf_SOURCES = cpp17/executors_with_sub_executors/perf.cpp
cpp17_executors_with_sub_executors_thread_pool_SOURCES = cpp17/executors_with_sub_executors/thread_pool.cpp
cpp17_sender_executors_inline_1_SOURCES = cpp17/sender_executors/inline_1.cpp
cpp17_sender_executors_lazy_SOURCES = cpp17/sender_executors/lazy.cpp
cpp17_sender_executors_perf_SOURCES = cpp17/sender_executors/perf.cpp
cpp17_sender_executors_thread_pool_SOURCES = cpp17/sender_executors/thread_pool.cpp

endif
endif

AM_CXXFLAGS = -I$(srcdir)/../include

EXTRADIST = \
	cpp17/trivial_executors/execution.hpp \
	cpp17/executors_with_sub_executors/execution.hpp \
	cpp17/sender_executors/execution.hpp

MAINTAINERCLEANFILES = \
	$(srcdir)/Makefile.in
