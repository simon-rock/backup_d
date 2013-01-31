AUTOMAKE_OPTIONS = foreign

bin_PROGRAMS = backupd backupctrl

backupd_SOURCES = main.cpp backup_main.cpp brick_group.cpp deamon.cpp backup_status.cpp 
backupd_CPPFLAGS = -I ../include/ `pkg-config --cflags gtkmm-2.4`
backupd_LDFLAGS = -L ../systools -L ../backup -L ../db_access -L ../syslayer  
LIBS = -lbackup -lsystools -ldb_access -lsyslayer -L/usr/lib64/mysql/ -lmysqlclient -lglog -lpthread -ludev `pkg-config --libs gtkmm-2.4`
backupctrl_SOURCES = backupclient.cpp
backupctrl_CPPFLAGS = -I ../include/ `pkg-config --cflags gtkmm-2.4`
backupctrl_LDFLAGS = -L ../systools -L ../backup -L ../db_access -L ../syslayer  
