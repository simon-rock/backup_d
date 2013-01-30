#t
AUTOMAKE_OPTIONS = foreign

bin_PROGRAMS = backupadminctrl

backupadminctrl_SOURCES = main.cpp masterwin.cpp BrickMaster.cpp NASMaster.cpp QueryFrame.cpp db_config.cpp BrowseFolder.cpp 
backupadminctrl_CPPFLAGS = -I ../include/ `pkg-config --cflags gtkmm-2.4`
#backupd_LDADD = ../lib/libgsync.a #/home/yu/workspace/lib/boost_1_42_0/stage/lib/libboost_thread.a
backupadminctrl_LDFLAGS = -L ../systools -L ../backup -L ../db_access -L ../syslayer  #-lsystools -ldb_access -L/usr/lib64/mysql/ -lmysqlclient -lglog -lpthread `pkg-config --libs gtkmm-2.4`

LIBS = -ldb_access -lsystools -L/usr/lib64/mysql/ -lmysqlclient -lglog -lpthread `pkg-config --libs gtkmm-2.4`

