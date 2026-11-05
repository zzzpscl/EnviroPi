CREATE DATABASE IF NOT EXISTS enviroDb CHARACTER SET utf8mb4;
USE enviroDb;

CREATE TABLE IF NOT EXISTS station (
    id TINYINT PRIMARY KEY,
    raumname VARCHAR(50) NOT NULL
);

CREATE TABLE IF NOT EXISTS messung (
    id BIGINT AUTO_INCREMENT PRIMARY KEY,
    station_id TINYINT NOT NULL DEFAULT 1,
    temperatur DECIMAL(4,1) NULL,
    luftfeuchtigkeit DECIMAL(4,1) NULL,
    luftdruck DECIMAL(5,1) NULL,
    co2 SMALLINT UNSIGNED NULL,
    gas_raw SMALLINT UNSIGNED NULL,
    zeitstempel DATETIME NOT NULL,
    UNIQUE (station_id, zeitstempel),
    FOREIGN KEY (station_id) REFERENCES station(id)
);

INSERT IGNORE INTO station (id, raumname) VALUES (1, 'RAUM361');