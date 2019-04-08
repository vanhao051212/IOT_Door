ALTER USER 'root'@'localhost' IDENTIFIED WITH mysql_native_password BY 'qwerty123456789';

CREATE DATABASE csvc;
use csvc;

create table gv (
	ID varchar (50)  primary key,
	TenGV varchar (50)
);
delete from gv;
insert into gv (ID, TenGV) values ("192 37 245 25", "Ngo Quoc Nhu");

create table room (
	RoomID varchar(20) primary key,
    Mess varchar(500)
);

create table mess (
	MessID varchar(50) primary key,
    MessInFo varchar(200)
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