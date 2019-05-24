ALTER USER 'root'@'localhost' IDENTIFIED WITH mysql_native_password BY 'qwerty123456789';

CREATE DATABASE csvc;
use csvc;

create table gv (
	ID varchar (50)  primary key,
	TenGV varchar (50)
);

drop table room;
create table room (
	Ind bigint primary key auto_increment,
	RoomID varchar(10) not null,
    MessID varchar(5),
    CardID varchar(10),
    TimeGet datetime
);

create table roomlist (
	RoomID varchar(10) primary key,
    RoomName varchar(100)
);

drop table mess;
create table mess (
	MessID varchar(5) primary key,
    MessInFo varchar(200)
);

create table diemdanh (
	Ind bigint primary key auto_increment,
    RoomID varchar(20),
    CardID varchar(10),
    TimeGet datetime
);

select * from mess;

insert into mess (MessID, MessInFo) values ('M01', 'Remote máy chiếu');
insert into mess (MessID, MessInFo) values ('M02', 'Micro');
insert into mess (MessID, MessInFo) values ('M03', 'Dây HDMI');
insert into mess (MessID, MessInFo) values ('M04', 'Micro hết pin');
insert into mess (MessID, MessInFo) values ('M05', 'Báo động mất máy chiếu');
insert into mess (MessID, MessInFo) values ('M06', 'Loa gặp vấn đề');
insert into mess (MessID, MessInFo) values ('M07', 'Trục trặc kỹ thuật khác');
insert into mess (MessID, MessInFo) values ('M08', 'Xác nhận tự sửa thành công');

SET SQL_SAFE_UPDATES = 0;
delete from diemdanh;
delete from room;
insert into room (RoomID, MessID, CardID, TimeGet) values ('R001', 'M01', 'ABABABAB','2019-5-21 11:52:00');
insert into room (RoomID, MessID, CardID, TimeGet) values ('R002', 'M02', 'ABABFBAB','2019-5-21 11:52:00');
insert into room (RoomID, MessID, CardID, TimeGet) values ('R002', 'M01', 'ABABBBAB','2019-5-21 11:52:00');
insert into room (RoomID, MessID, CardID, TimeGet) values ('R102', 'M05', '','2019-5-21 11:52:00');

delete from roomlist;
alter table roomlist add Area varchar(80);
insert into roomlist (RoomID, RoomName, Area) values ('R001','Phòng C101','Khu C');
insert into roomlist (RoomID, RoomName, Area) values ('R002','Phòng C102','Khu C');
insert into roomlist (RoomID, RoomName, Area) values ('R003','Phòng E6.1','Khu E');
insert into roomlist (RoomID, RoomName, Area) values ('R004','Phòng B4.18','Khu B');
insert into roomlist (RoomID, RoomName, Area) values ('R005','Phòng B4.18','Khu B');
insert into roomlist (RoomID, RoomName, Area) values ('R006','Phòng B4.18','Khu B');
insert into roomlist (RoomID, RoomName, Area) values ('R007','Phòng B4.18','Khu B');
insert into roomlist (RoomID, RoomName, Area) values ('R008','Phòng B4.18','Khu B');
insert into roomlist (RoomID, RoomName, Area) values ('R009','Phòng B4.18','Khu B');


select * from mess;

select * from room;

select distinct Area from roomlist;
select * from roomlist, room where room.RoomID = roomlist.RoomID and roomlist.Area = 'Khu C';
select * from roomlist;

select * from diemdanh;