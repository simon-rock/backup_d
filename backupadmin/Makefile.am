#t
AUTOMAKE_OPTIONS = foreign

bin_PROGRAMS = backupadminctrl

backupadminctrl_SOURCES = main.cpp masterwin.cpp BrickMaster.cpp NASMaster.cpp QueryFrame.cpp db_config.cpp BrowseFolder.cpp 
backupadminctrl_CPPFLAGS = -I ../include/ `pkg-config --cflags gtkmm-2.4`
backupadminctrl_LDFLAGS = -L ../systools -L ../backup -L ../db_access -L ../syslayer

LIBS = -ldb_access -lsystools -L/usr/lib64/mysql/ -lmysqlclient -lglog -lpthread `pkg-config --libs gtkmm-2.4`

