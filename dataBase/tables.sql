BEGIN TRANSACTION;

CREATE TABLE IF NOT EXISTS XCubeSat_Controller_scripts(
    ScriptId integer primary key autoincrement, 
    ScriptContent text not null,
    Alias text not null,
    ScriptName text not null);

CREATE TABLE IF NOT EXISTS XCubeSat_Controller_sciData(
    DataId integer primary key autoincrement,
    SatName text not null,
    TimeStamp integer not null, 
    RawContent text not null, 
    DataNickName text not null);

CREATE TABLE IF NOT EXISTS XCubeSat_Controller_wodData(
    DataId integer primary key autoincrement,
    SatName text not null,
    TimeStamp integer not null,
    RawContent text not null
    DataNickNAme text not null);
