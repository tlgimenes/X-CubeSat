CREATE TABLE IF NOT EXISTS XCubeSat_Controller_session(
    SatId integer primary key autoincrement not null,
    SatName text not null,
    AmountOfScripts integer not null,
    ScriptNames text);

CREATE TABLE IF NOT EXISTS XCubeSat_Controller_scripts(
    ScriptId integer primary key autoincrement not null, 
    ScriptContent text not null,
    Alias text not null,
    ScriptName text not null,
    unique(ScriptName));

CREATE TABLE IF NOT EXISTS XCubeSat_Controller_sciData(
    DataId integer primary key autoincrement not null,
    SatName text not null,
    TimeStamp integer not null, 
    RawContent text not null, 
    DataNickName text not null);

CREATE TABLE IF NOT EXISTS XCubeSat_Controller_wodData(
    DataId integer primary key autoincrement not null,
    SatName text not null,
    TimeStamp integer not null,
    RawContent text not null,
    DataNickName text not null);

