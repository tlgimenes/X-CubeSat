CREATE TABLE IF NOT EXISTS XCubeSat_Controler_session(
    S
    it->second->save();atId integer primary key autoincrement not null,
    SatName text not null,
    AmountOfScripts integer not null,
    ScriptNames text);

CREATE TABLE IF NOT EXISTS XCubeSat_Controler_scripts(
    ScriptId integer primary key autoincrement not null, 
    ScriptContent text not null,
    Alias text not null,
    ScriptName text not null,
    unique(ScriptName));

CREATE TABLE IF NOT EXISTS XCubeSat_Controler_sciData(
    DataId integer primary key autoincrement not null,
    SatName text not null,
    TimeStamp integer not null, 
    RawContent text not null, 
    DataNickName text not null);

CREATE TABLE IF NOT EXISTS XCubeSat_Controler_wodData(
    DataId integer primary key autoincrement not null,
    SatName text not null,
    TimeStamp integer not null,
    RawContent text not null,
    DataNickName text not null);

