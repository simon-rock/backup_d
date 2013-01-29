AUTOMAKE_OPTIONS = foreign

bin_PROGRAMS = backupd backupctrl

backupd_SOURCES = main.cpp backup_main.cpp brick_group.cpp deamon.cpp backup_status.cpp 
backupd_CPPFLAGS = -I ../include/ `pkg-config --cflags gtkmm-2.4`
#backupd_LDADD = ../backup/libbackup.a #/home/yu/workspace/lib/boost_1_42_0/stage/lib/libboost_thread.a
backupd_LDFLAGS = -L ../systools -L ../backup -L ../db_access -L ../syslayer  #-lbackup -lsystools -ldb_access -lsyslayer -L/usr/lib64/mysql/ -lmysqlclient -lglog -lpthread -ludev `pkg-config --libs gtkmm-2.4`
LIBS = -lbackup -lsystools -ldb_access -lsyslayer -L/usr/lib64/mysql/ -lmysqlclient -lglog -lpthread -ludev `pkg-config --libs gtkmm-2.4`
backupctrl_SOURCES = backupclient.cpp
backupctrl_CPPFLAGS = -I ../include/ `pkg-config --cflags gtkmm-2.4`
#backupctrl_LDADD = ../lib/libgsync.a #/home/yu/workspace/lib/boost_1_42_0/stage/lib/libboost_thread.a
#backupctrl_LDADD = ../backup/libbackup.a #/home/yu/workspace/lib/boost_1_42_0/stage/lib/libboost_thread.a
#backupctrl_LDFLAGS = -L ../systools -L ../backup -L ../db_access -L ../syslayer  -lbackup -lsystools -ldb_access -lsyslayer -L/usr/lib64/mysql/ -lmysqlclient -lglog -lpthread -ludev `pkg-config --libs gtkmm-2.4`
backupctrl_LDFLAGS = -L ../systools -L ../backup -L ../db_access -L ../syslayer  #-lbackup -lsystools -ldb_access -lsyslayer -L/usr/lib64/mysql/ -lmysqlclient -lglog -lpthread -ludev `pkg-config --libs gtkmm-2.4`
