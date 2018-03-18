#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <pqxx/pqxx>

#include "exerciser.h"

using namespace std;
using namespace pqxx;

void __create_table(connection*);
bool __load_player(connection*, const std::string&);
bool __load_team(connection*, const std::string&);
bool __load_state(connection*, const std::string&);
bool __load_color(connection*, const std::string&);

int main (int argc, char *argv[]) 
{

  //Allocate & initialize a Postgres connection object
  connection *C;

  try{
    //Establish a connection to the database
    //Parameters: database name, user name, user password
    C = new connection("dbname=ACC_BBALL user=postgres password=passw0rd");
    if (C->is_open()) {
      cout << "Opened database successfully: " << C->dbname() << endl;
    } else {
      cout << "Can't open database" << endl;
      return 1;
    }
  } catch (const std::exception &e){
    cerr << e.what() << std::endl;
    return 1;
  }


  //TODO: create PLAYER, TEAM, STATE, and COLOR tables in the ACC_BBALL database
  //      load each table with rows from the provided source txt files
  __create_table(C);
  __load_player( C, "player.txt");
  __load_team(   C, "team.txt");
  __load_state(  C, "state.txt");
  __load_color(  C, "color.txt");

  exercise(C);


  //Close database connection
  C->disconnect();

  return 0;
}


void __create_table(connection* C) {
  /* Create a transactional object. */
  work _dw(*C);
  ///! delete the all of table.
  std::string command = "DROP TABLE PLAYER;";
  _dw.exec(command);

  command = "DROP TABLE TEAM;";
  _dw.exec(command);

  command = "DROP TABLE STATE;";
  _dw.exec(command);

  command = "DROP TABLE COLOR;";
  _dw.exec(command);

  ///! commit
  _dw.commit();

  ///! create the new table.
  work _cw(*C);
  command = std::string("CREATE TABLE PLAYER(") +
      "PLAYER_ID INT PRIMARY KEY  NOT NULL," +
      "TEAM_ID               INT  NOT NULL," +
      "UNIFORM_NUM           INT  NOT NULL," +
      "FIRST_NAME            TEXT," +
      "LAST_NAME             TEXT," +
      "MPG                   INT," +
      "PPG                   INT," +
      "RPG                   INT," +
      "APG                   INT," +
      "SPG                   DOUBLE PRECISION," +
      "BPG                   DOUBLE PRECISION );";
  _cw.exec(command);
   /* Create a transactional object. */
  // work W(*C);
  /* Execute SQL query */
//  try{
//    W1.exec( command );
//    // W1.commit();
//  } catch (const std::exception &e){
//    cerr << e.what() << std::endl;
//  }


  command = std::string("CREATE TABLE TEAM(")  +
      "TEAM_ID INT PRIMARY KEY   NOT NULL," +
      "NAME                TEXT  NOT NULL," +
      "STATE_ID            INT   NOT NULL," +
      "COLOR_ID            INT," +
      "WINS                INT," +
      "LOSSES              INT);";
  _cw.exec(command);
   /* Create a transactional object. */
  // work W2(*C);
  /* Execute SQL query */
  // W.exec( command );
  // W2.commit();

  command = std::string("CREATE TABLE STATE(")  +
      "STATE_ID INT PRIMARY KEY  NOT NULL," +
      "NAME                 TEXT NOT NULL );";
   /* Create a transactional object. */
  // work W3(*C);
  /* Execute SQL query */
  _cw.exec( command );
  // W3.commit();

  command = std::string("CREATE TABLE COLOR(")  +
      "COLOR_ID INT PRIMARY KEY  NOT NULL," +
      "NAME                 TEXT NOT NULL );";
   /* Create a transactional object. */
  // work W4(*C);
  /* Execute SQL query */
  _cw.exec( command );
  _cw.commit();
}

bool __load_player(connection* C, const std::string& filename) {
  std::ifstream _fi(filename);
  if (!_fi.is_open()) {
    std::cout << "Can't read the file[" << filename << "]!" << std::endl;
    return false;
  }

  std::stringstream ss;
  int int_buf   = 0;
  double db_buf = 0;
  std::string str_buf;
  work W(*C);
  while (true) {
    ss << "INSERT INTO PLAYER(PLAYER_ID, TEAM_ID, UNIFORM_NUM, FIRST_NAME, LAST_NAME, MPG, PPG, RPG, APG, SPG, BPG) ";
    ss << "VALUES (";
    ///! player id
    _fi >> int_buf; ss << int_buf; ss << ", ";
    ///! team id
    _fi >> int_buf; ss << int_buf; ss << ", ";
    ///! uniform num
    _fi >> int_buf; ss << int_buf; ss << ", ";
    ///! first name
    _fi >> str_buf;
    if (std::string::npos != str_buf.find('\'')) {
      auto p = str_buf.find('\'');
      str_buf.insert(p, "\'");
    }
    ss << "'"; ss << str_buf; ss << "'"; ss << ", ";
    ///! last name
    _fi >> str_buf;
    if (std::string::npos != str_buf.find('\'')) {
      auto p = str_buf.find('\'');
      str_buf.insert(p, "\'");
    }
    ss << "'"; ss << str_buf; ss << "'"; ss << ", ";
    ///! MPG
    _fi >> db_buf;  ss << db_buf; ss << ", ";
    ///! PPG
    _fi >> db_buf;  ss << db_buf; ss << ", ";
    ///! RPG
    _fi >> db_buf;  ss << db_buf; ss << ", ";
    ///! APG
    _fi >> db_buf;  ss << db_buf; ss << ", ";
    ///! SPG
    _fi >> db_buf;  ss << db_buf; ss << ", ";
    ///! BPG
    _fi >> db_buf;  ss << db_buf;
    ss << "); ";

    if (_fi.eof()) break;
    str_buf = ss.str();
    ss.str("");
    W.exec( str_buf );
  }
  W.commit();

  return true;
}

bool __load_team(connection* C, const std::string& filename) {
  std::ifstream _fd(filename);
  if (!_fd.is_open()) {
    std::cout << "Can't read the file[" << filename << "]!" << std::endl;
    return false;
  }

  work W(*C);
  int int_buf   = 0;
  std::stringstream ss;
  std::string str_buf;
  while (true) {
    ss << "INSERT INTO TEAM(TEAM_ID, NAME, STATE_ID, COLOR_ID, WINS, LOSSES) ";
    ss << "VALUES (";
    ///! TEAM_ID
    _fd >> int_buf; ss << int_buf; ss << ", ";
    ///! name
    _fd >> str_buf;
    if (std::string::npos != str_buf.find('\'')) {
      auto p = str_buf.find('\'');
      str_buf.insert(p, "\'");
    }
    ss << "'"; ss << str_buf; ss << "'"; ss << ", ";
    ///! STATE_ID
    _fd >> int_buf; ss << int_buf; ss << ", ";
    ///! COLOR_ID
    _fd >> int_buf; ss << int_buf; ss << ", ";
    ///! WINS
    _fd >> int_buf; ss << int_buf; ss << ", ";
    ///! LOSSES
    _fd >> int_buf; ss << int_buf;
    ss << "); ";

    if (_fd.eof()) break;
    str_buf = ss.str();
    ss.str("");
    W.exec( str_buf );
  }
  
  W.commit();
  return true;
}

bool __load_state(connection* C, const std::string& filename) {
  std::ifstream _fd(filename);
  if (!_fd.is_open()) {
    std::cout << "Can't read the file[" << filename << "]!" << std::endl;
    return false;
  }

  work W(*C);
  int int_buf = 0;
  std::stringstream ss;
  std::string str_buf;
  while (true) {
    ss << "INSERT INTO STATE(STATE_ID, NAME) ";
    ss << "VALUES (";
    ///! STATE_ID
    _fd >> int_buf; ss << int_buf; ss << ", ";
    ///! NAME
    _fd >> str_buf;
    if (std::string::npos != str_buf.find('\'')) {
      auto p = str_buf.find('\'');
      str_buf.insert(p, "\'");
    }
    ss << "'"; ss << str_buf; ss << "'";
    ss << "); ";

    if (_fd.eof()) break;
    str_buf = ss.str();
    ss.str("");
    W.exec( str_buf );
  }
  
  W.commit();
  return true;
}

bool __load_color(connection* C, const std::string& filename) {
  std::ifstream _fd(filename);
  if (!_fd.is_open()) {
    std::cout << "Can't read the file[" << filename << "]!" << std::endl;
    return false;
  }

  work W(*C);
  int int_buf = 0;
  std::stringstream ss;
  std::string str_buf;
  while (!_fd.eof()) {
    ss << "INSERT INTO COLOR(COLOR_ID, NAME) ";
    ss << "VALUES (";
    ///! COLOR_ID
    _fd >> int_buf; ss << int_buf; ss << ", ";
    ///! NAME
    _fd >> str_buf;
    if (std::string::npos != str_buf.find('\'')) {
      auto p = str_buf.find('\'');
      str_buf.insert(p, "\'");
    }
    ss << "'"; ss << str_buf; ss << "'";
    ss << "); ";

    if (_fd.eof()) break;
    str_buf = ss.str();
    ss.str("");
    W.exec( str_buf );
  }
  
  W.commit();
  return true;
}
