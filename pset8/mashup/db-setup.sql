CREATE TABLE places (
    country_code char(2),
    postal_code varchar(20),
    place_name varchar(180),
    admin_name1 varchar(100),
    admin_code1 varchar(20),
    admin_name2 varchar(100),
    admin_code2 varchar(20),
    admin_name3 varchar(100),
    admin_code3 varchar(20),
    latitude decimal(8,6),
    longitude decimal(9,6),
    accuracy TINYINT
);