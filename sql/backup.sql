CREATE DATABASE backup;
use backup;
-- NAS 主表 
CREATE TABLE NAS_MASTER (
   NAS_ID char(20) NOT NULL,		-- NAS 标识
 
   PRIMARY KEY  (NAS_ID)
 ) ENGINE=InnoDB ;
 
-- brick 索引表 制约系统中 单个NAS中的brick 数
 CREATE TABLE BRICK_INDEX (
   BRICK_NUM int NOT NULL,
 
   PRIMARY KEY  (BRICK_NUM)
 ) ENGINE=InnoDB ;

-- BRICK 主表
 CREATE TABLE BRICK_MASTER (
   BRICK_ID char(20) NOT NULL,		  -- brick 标识
   PATH char(255) NOT NULL,		  -- brick 网络路径
   MOUNT_PATH char(255) NOT NULL,	  -- brick 本地挂在路径
   BACKUP_PATH char(255) NOT NULL,	  -- brick 备份目标路径
   BACKUP_TIME_BEGIN bigint not NULL,	  -- brick 备份过滤条件开始时间（UTC）
   BACKUP_TIME_INTERVAL bigint not NULL,  -- brick 备份过滤条件距离当前间隔时间（UTC） 默认--条件还有一个备份跨度最小值（默认一小时）
   PRIMARY KEY  (BRICK_ID)
 ) ENGINE=InnoDB ;

-- backup 关系
 CREATE TABLE BACKUP_MASTER (
   NAS_ID char(20) NOT NULL,
   BRICK_NUM int NOT NULL,
   BRICK_ID char(20) NOT NULL unique,
  PRIMARY KEY  (BRICK_ID),
  FOREIGN KEY (NAS_ID) REFERENCES NAS_MASTER (NAS_ID),
  FOREIGN KEY (BRICK_NUM) REFERENCES BRICK_INDEX (BRICK_NUM),
  FOREIGN KEY (BRICK_ID) REFERENCES BRICK_MASTER (BRICK_ID)
 ) ENGINE=InnoDB ;

 -- BRICK 对应 物理盘位置（按硬件情况而定）
 CREATE TABLE BRICK_DISK_POS (
   BRICK_ID char(20) NOT NULL,
   DISK_POS int NOT NULL,

   PRIMARY KEY  (DISK_POS),
   FOREIGN KEY (BRICK_ID) REFERENCES BRICK_MASTER (BRICK_ID)
 ) ENGINE=InnoDB ;

-- 备份日志
-- BRICK 对应的硬盘序列号
 CREATE TABLE BRICK_DISK (
   BRICK_ID char(20) NOT NULL,
   DISK_ID char(100) NOT NULL,

   PRIMARY KEY  (DISK_ID),
   FOREIGN KEY (BRICK_ID) REFERENCES BRICK_MASTER (BRICK_ID)
 ) ENGINE=InnoDB ;

-- 硬盘中备份的文件信息
 CREATE TABLE DISK_FILE (
   DISK_ID char(100) NOT NULL,
   BK_FILE char(100) NOT NULL,

   PRIMARY KEY  (BK_FILE),
   FOREIGN KEY (DISK_ID) REFERENCES BRICK_DISK (DISK_ID)
 ) ENGINE=InnoDB ;

-- JBOD 描述信息
 CREATE TABLE JBOD_MASTER(
   SAS_ADDR char(100) NOT NULL,     -- jbod 地址信息
   START_NUM int NOT NULL,          -- 第一块硬盘位置
   JBOD_ORDER int NOT NULL,			-- 1:ASC 0:DES
   PRIMARY KEY  (SAS_ADDR)
 ) ENGINE=InnoDB ;

-- insert into NAS_MASTER values('NAS_ID_1');
-- insert into BRICK_INDEX values(1);
-- insert into BRICK_MASTER  values('BRICK_ID_1_1', '192.168.1.1');
-- insert into backup.BACKUP_MASTER 
-- 	(NAS_ID, BRICK_NUM, BRICK_ID)
-- 	values
-- 	('NAS_ID_1', 1, 'BRICK_ID_1_1')

select * from NAS_MASTER ;
select * from BRICK_INDEX  ;
select * from BRICK_MASTER;
select * from BACKUP_MASTER  ;