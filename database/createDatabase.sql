ALTER USER 'root'@'localhost' IDENTIFIED WITH mysql_native_password BY 'qwerty123456789';

CREATE DATABASE csvc;
use csvc;

create table gv (
	ID varchar (50)  primary key,
	TenGV varchar (50)
);
delete from gv;
update GV SET ID = '192 111 78 25' where ID = '192 37 245 25';
insert into gv (ID, TenGV) values ("192 37 245 25", "Ngo Quoc Nhu");
insert into gv (ID, TenGV) values ("5 187 141 185", "Le Trong Thuc");

drop table room;
create table room (
	Ind bigint primary key auto_increment,
	RoomID varchar(10) not null,
    MessID varchar(5),
    CardID varchar(10),
    TimeGet datetime
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

insert into mess (MessID, MessInFo) values ('M01', 'Remote máy chiếu');
insert into mess (MessID, MessInFo) values ('M02', 'Micro');
insert into mess (MessID, MessInFo) values ('M03', 'Micro hết pin');
insert into mess (MessID, MessInFo) values ('M04', 'Dây HDMI');
insert into mess (MessID, MessInFo) values ('M05', 'Máy chiếu gặp vấn đề');
insert into mess (MessID, MessInFo) values ('M06', 'Loa gặp vấn đề');
insert into mess (MessID, MessInFo) values ('M07', 'Trục trặc kỹ thuật khác');
insert into mess (MessID, MessInFo) values ('M08', 'Xác nhận tự sửa thành công');

SET SQL_SAFE_UPDATES = 0;
delete from room;
insert into room (RoomID, Mess) values ('R101', '["M01","M02"]');
insert into room (RoomID, Mess) values ('R302', '[]');
insert into room (RoomID, Mess) values ('R201', '["M03", "M04"]');
insert into room (RoomID, Mess) values ('R311', '["M05", "M08"]');

update room set Mess='[]';

select * from mess;

select * from room;

select * from gv;