ALTER SESSION SET CONTAINER = FREEPDB1;
ALTER PLUGGABLE DATABASE FREEPDB1 OPEN;

// 1
create table T_RANGE(
    id number,
    data varchar2(100)
)
partition by range(id)(
    partition p1 values less than (100),
    partition p2 values less than (200),
    partition p3 values less than (300),
    partition pmax values less than (maxvalue)
);
    
// 2
create table T_INTERVAL(
    id number,
    data varchar2(100),
    date_created date
)
partition by range (date_created)
interval (numtoyminterval(1, 'MONTH'))(
    partition p0 values less than (date '2000-01-01')
);

// 3
create table T_HASH(
    id number,
    name varchar2(50),
    data varchar2(100)
)
partition by hash (name)
partitions 4;

// 4
create table T_LIST(
    id number,
    region char(5),
    data varchar2(100)
)
partition by list(region)(
    partition p_eu values ('EN', 'BR', 'FR', 'RU'),
    partition p_asia values ('CH', 'JP', 'IN'),
    partition p_amer values ('USA', 'KAN'),
    partition p_others values(default)
);

// 5
insert into t_range values (10, 'range < 100');
insert into t_range values (150, 'range 100-199');
insert into t_range values (250, 'range < 200-299');
insert into t_range values (350, 'range >= 300');

select * from t_range;

insert into t_interval values(1, 'interval december', date '1999-12-25');
insert into t_interval values(2, 'interval january', date '2000-01-25');
insert into t_interval values(3, 'interval febrary', date '2000-02-25');
insert into t_interval values(4, 'interval march', date '2000-03-25');

select * from t_interval;

insert into t_hash values (1, 'Vanya', 'hash test 1');
insert into t_hash values (2, 'Lida', 'hash test 2');
insert into t_hash values (3, 'James', 'hash test 3');
insert into t_hash values (4, 'Angela', 'hash test 4');

select * from t_hash;

insert into t_list values (1, 'BR', 'List Europa');
insert into t_list values (2, 'JP', 'List Asia');
insert into t_list values (3, 'USA', 'List America');
insert into t_list values (4, 'XXX', 'List Others');

select * from t_list partition(p_eu);

commit;

// 6
alter table t_range enable row movement;
update t_range set id = 99 where id = 150;

select * from t_range partition(p1);
select * from t_range partition(p2);


alter table t_interval enable row movement;
update t_interval set date_created = date '2000-03-05' where id = 3;

select * from user_tab_partitions where table_name = 'T_INTERVAL';
select * from t_interval partition(SYS_P296);
select * from t_interval partition(SYS_P286);
select * from t_interval partition(SYS_P287);


alter table t_hash enable row movement;
update t_hash set name = 'Ale' where id = 1;

select * from user_tab_partitions where table_name = 'T_HASH';
select * from t_hash partition(SYS_P292);
select * from t_hash partition(SYS_P2024);
select * from t_hash partition(SYS_P2025);
select * from t_hash partition(SYS_P2023);


alter table t_list enable row movement;
update t_list set region = 'IN' where id = 1;

select * from t_list partition(p_eu);
select * from t_list partition(p_asia);


drop table t_range purge;
drop table t_interval purge;
drop table t_hash purge;
drop table t_list purge;

// 7
alter table t_range merge partitions p1, p2 into partition p12;
select * from t_range partition(p1);
select * from t_range partition(p2);
select * from t_range partition(p12);

// 8
alter table t_range split partition pmax
at (500) into (
    partition p4,
    partition p5
);
select * from t_range partition(p4);
select * from t_range partition(p5);
select * from t_range partition(pmax);

// 9
create table t_asia(
    id number,
    region char(5),
    data varchar2(100)
);

alter table t_list exchange partition p_asia with table t_asia;

select * from t_asia;
select * from t_range partition(p_asia);

--------------------------------------------------------------------------------
DROP TABLE t_range PURGE;
DROP TABLE t_interval PURGE;
DROP TABLE t_hash PURGE;
DROP TABLE t_list PURGE;
DROP TABLE t_asia PURGE;