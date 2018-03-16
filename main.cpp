#include <string>
#include <fstream>
#include <iostream>
#include <stringstream>
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
  char* player = "CREATE TABLE PLAYER("  \
      "PLAYER_ID INT PRIMARY KEY  NOT NULL," \
      "TEAM_ID               INT  NOT NULL," \
      "UNIFORM_NUM           INT  NOT NULL," \
      "FIRST_NAME            CHAR(32)," \
      "LAST_NAME             CHAR(32)," \
      "MPG                   REAL," \
      "PPG                   REAL," \
      "RPG                   REAL," \
      "APG                   REAL," \
      "SPG                   REAL," \
      "BPG                   REAL );";
   /* Create a transactional object. */
  work W1(C);
  /* Execute SQL query */
  W1.exec( player );
  W1.commit();

  char* team = "CREATE TABLE TEAM("  \
      "TEAM_ID INT PRIMARY KEY   NOT NULL," \
      "NAME                TEXT  NOT NULL," \
      "STATE_ID            INT   NOT NULL," \
      "COLOR_ID            INT," \
      "WINS                INT," \
      "LOSSES              INT);";
   /* Create a transactional object. */
  work W2(C);
  /* Execute SQL query */
  W2.exec( team );
  W2.commit();

  char* state = "CREATE TABLE STATE("  \
      "STATE_ID INT PRIMARY KEY  NOT NULL," \
      "NAME                 TEXT NOT NULL );";
   /* Create a transactional object. */
  work W3(C);
  /* Execute SQL query */
  W3.exec( state );
  W3.commit();

  char* color = "CREATE TABLE COLOR("  \
      "COLOR_ID INT PRIMARY KEY  NOT NULL," \
      "NAME                 TEXT NOT NULL );";
   /* Create a transactional object. */
  work W4(C);
  /* Execute SQL query */
  W4.exec( color );
  W4.commit();
}

bool __load_player(connection* C, const std::string& filename) {
  std::ofstream _of(filename);
  if (!_of.is_open()) {
    std::cout << "Can't read the file[" << filename << << "]!" std::endl;
    return false;
  }

  std::stringstream ss;
  int int_buf   = 0;
  double db_buf = 0;
  std::string str_buf;
  while (!_of.eof()) {
    ss << "INSERT INTO PLAYER(PLAYER_ID, TEAM_ID, UNIFORM_NUM, FIRST_NAME, LAST_NAME, MPG, PPG,
RPG, APG, SPG, BPG) ";
    ss << "VALUES (";
    ///! player id
    _of >> int_buf; ss << int_buf; ss << ", ";
    ///! team id
    _of >> int_buf; ss << int_buf; ss << ", ";
    ///! uniform num
    _of >> int_buf; ss << int_buf; ss << ", ";
    ///! first name
    _of >> str_buf; ss << "'"; ss << str_buf; ss << "'"; ss << ", ";
    ///! last name
    _of >> str_buf; ss << "'"; ss << str_buf; ss << "'"; ss << ", ";
    ///! MPG
    _of >> db_buf;  ss << db_buf; ss << ", ";
    ///! PPG
    _of >> db_buf;  ss << db_buf; ss << ", ";
    ///! RPG
    _of >> db_buf;  ss << db_buf; ss << ", ";
    ///! APG
    _of >> db_buf;  ss << db_buf; ss << ", ";
    ///! SPG
    _of >> db_buf;  ss << db_buf; ss << ", ";
    ///! BPG
    _of >> db_buf;  ss << db_buf;
    ss << "); ";
  }
  /* Create a transactional object. */
  work W(C);
  
  /* Execute SQL query */
  W.exec( ss.str() );
  W.commit();
}

bool __load_team(connection* C, const std::string& filename) {
  std::ofstream _of(filename);
  if (!_of.is_open()) {
    std::cout << "Can't read the file[" << filename << << "]!" std::endl;
    return false;
  }

  int int_buf   = 0;
  std::stringstream ss;
  std::string str_buf;
  while (!_of.eof()) {
    ss << "INSERT INTO TEAM(TEAM_ID, NAME, STATE_ID, COLOR_ID, WINS, LOSSES) ";
    ss << "VALUES (";
    ///! TEAM_ID
    _of >> int_buf; ss << int_buf; ss << ", ";
    ///! name
    _of >> str_buf; ss << "'"; ss << str_buf; ss << "'"; ss << ", ";
    ///! STATE_ID
    _of >> int_buf; ss << int_buf; ss << ", ";
    ///! COLOR_ID
    _of >> int_buf; ss << int_buf; ss << ", ";
    ///! WINS
    _of >> int_buf; ss << int_buf; ss << ", ";
    ///! LOSSES
    _of >> int_buf; ss << int_buf;
    ss << "); ";
  }

  /* Create a transactional object. */
  work W(C);
  
  /* Execute SQL query */
  W.exec( ss.str() );
  W.commit();
}

bool __load_state(connection* C, const std::string& filename) {
  std::ofstream _of(filename);
  if (!_of.is_open()) {
    std::cout << "Can't read the file[" << filename << << "]!" std::endl;
    return false;
  }

  int int_buf = 0;
  std::stringstream ss;
  std::string str_buf;
  while (!_of.eof()) {
    ss << "INSERT INTO STATE(STATE_ID, NAME) ";
    ss << "VALUES (";
    ///! STATE_ID
    _of >> int_buf; ss << int_buf; ss << ", ";
    ///! NAME
    _of >> str_buf; ss << "'"; ss << str_buf; ss << "'";
    ss << "); ";
  }

  /* Create a transactional object. */
  work W(C);
  
  /* Execute SQL query */
  W.exec( ss.str() );
  W.commit();
}

bool __load_color(connection* C, const std::string& filename) {
  std::ofstream _of(filename);
  if (!_of.is_open()) {
    std::cout << "Can't read the file[" << filename << << "]!" std::endl;
    return false;
  }

  int int_buf = 0;
  std::stringstream ss;
  std::string str_buf;
  while (!_of.eof()) {
    ss << "INSERT INTO COLOR(COLOR_ID, NAME) ";
    ss << "VALUES (";
    ///! COLOR_ID
    _of >> int_buf; ss << int_buf; ss << ", ";
    ///! NAME
    _of >> str_buf; ss << "'"; ss << str_buf; ss << "'";
    ss << "); ";
  }

  /* Create a transactional object. */
  work W(C);
  
  /* Execute SQL query */
  W.exec( ss.str() );
  W.commit();
}
