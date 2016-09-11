create table trader(
  id integer primary key autoincrement,
  version integer not null,
  username varchar(100) NOT NULL,
  password varchar(20) NOT NULL,
  balancecash numeric NOT NULL
);

create table stock(
  id integer primary key autoincrement,
  version integer not null,
  stockcode varchar(10),
  lastsaleprice numeric NOT NULL
);

create table portfolio(
  id integer primary key autoincrement,
  version integer not null,
  username varchar(100),
  stockcode varchar(10),
  quantity number NOT NULL,
  totalcost numeric NOT NULL
);

create table stock_transaction(
  id integer primary key autoincrement,
  version integer not null,
  username varchar(100),
  stockcode varchar(10),
  quantity number NOT NULL,
  datetime numeric NOT NULL,
  status varchar(10) NOT NULL
);

insert into trader values(1, 1, 'guest', 'guest', 100000);

insert into stock values (1, 1, 'HLL', 789);
insert into stock values (2, 1, 'NTPC', 135);
insert into stock values (3, 1, 'FEDLLO', 100);

-- No traders yet but each company listed on market is owner of a stock.
--insert into trader values('FEDLLO_USER', 'top_secret', 0);
--insert into trader values('NTPC_USER', 'top_secret', 0);
--insert into trader values('HLL_USER', 'top_secret', 0);

-- Each company stock with initial price
--insert into stock values('FEDLLO', 100);
--insert into stock values('NTPC', 135);
--insert into stock values('HLL', 789);

-- Initially, ALL stocks are owned by the company itself.
--insert into portfolio values('FEDLLO_USER', 'FEDLLO', 100, 100 * 100);
--insert into portfolio values('NTPC_USER', 'NTPC', 135, 135 * 135);
--insert into portfolio values('HLL_USER', 'HLL', 789, 789 * 789);

-- Initially all companies offer their stocks for sale.
--insert into stock_transaction values('FEDLLO_USER', 'FEDLLO', '100', '4', 'PENDING');
--insert into stock_transaction values('NTPC_USER', 'NTPC', '135', '4', 'PENDING');
--insert into stock_transaction values('HLL_USER', 'HLL', '789', '4', 'PENDING');
