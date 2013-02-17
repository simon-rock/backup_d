#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT(backupd, 1.0, )
AC_CONFIG_SRCDIR([backup_ui/main.cpp])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])
# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_RANLIB
#AC_PROG_LIBTOOL
# Checks for libraries.
# FIXME: Replace `main' with a function in `-lglog':
AC_CHECK_LIB([glog], [InitGoogleLogging])
# FIXME: Replace `main' with a function in `-lpthread':
AC_CHECK_LIB([pthread], [pthread_rwlock_init])

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h inttypes.h limits.h malloc.h memory.h netinet/in.h stddef.h stdint.h stdlib.h string.h sys/mount.h sys/param.h sys/socket.h sys/time.h sys/vfs.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_MODE_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_CHECK_MEMBERS([struct stat.st_blksize])
AC_STRUCT_ST_BLOCKS
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_FUNC_STRERROR_R
AC_CHECK_FUNCS([gettimeofday localtime_r memmove memset mkdir mkfifo select strerror])

AC_CONFIG_FILES([Makefile
				systools/Makefile
                db_access/Makefile
				syslayer/Makefile
				backup/Makefile
				backup_ui/Makefile
				backupadmin/Makefile])
AC_OUTPUT
