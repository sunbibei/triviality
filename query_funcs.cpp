#include "query_funcs.h"

#include <vector>
#include <sstream>

int __get_ava_player_id();
int __get_ava_team_id();
int __get_ava_state_id();
int __get_ava_color_id();

void add_player(connection *C, int team_id, int jersey_num, string first_name, string last_name,
                int mpg, int ppg, int rpg, int apg, double spg, double bpg) {
	std::stringstream ss;
	ss << "INSERT INTO PLAYER(TEAM_ID, UNIFORM_NUM, FIRST_NAME, LAST_NAME, MPG, PPG, RPG, APG, SPG, BPG) ";
  ss << "VALUES ("  << team_id << ", " << jersey_num << ", '" << first_name
     << "', '" << last_name << "', " << mpg << ", "<< ppg << ", "<< rpg << ", "
     << apg << ", "<< spg << ", "<< bpg << ");";

	/* Create a transactional object. */
	work W(*C);

	/* Execute SQL query */
	W.exec( ss.str() );
	W.commit();
}


void add_team(connection *C, string name, int state_id, int color_id, int wins, int losses) {
	std::stringstream ss;
	ss << "INSERT INTO TEAM(NAME, STATE_ID, COLOR_ID, WINS, LOSSES) ";
  ss << "VALUES ('" <<  name << "', '" << state_id << ", "
     << color_id << ", " << wins << ", " << losses << ");";

	/* Create a transactional object. */
	work W(*C);

	/* Execute SQL query */
	W.exec( ss.str() );
	W.commit();
}


void add_state(connection *C, string name) {
	std::stringstream ss;

	ss << "INSERT INTO PLAYER(NAME) ";
    ss << "VALUES ('" << name << "');";

	/* Create a transactional object. */
	work W(*C);

	/* Execute SQL query */
	W.exec( ss.str() );
	W.commit();
}


void add_color(connection *C, string name) {
	std::stringstream ss;

	ss << "INSERT INTO PLAYER(NAME) ";
    ss << "VALUES ('" << name << "');";

	/* Create a transactional object. */
	work W(*C);

	/* Execute SQL query */
	W.exec( ss.str() );
	W.commit();
}


void query1(connection *C,
	    int use_mpg, int min_mpg, int max_mpg,
            int use_ppg, int min_ppg, int max_ppg,
            int use_rpg, int min_rpg, int max_rpg,
            int use_apg, int min_apg, int max_apg,
            int use_spg, double min_spg, double max_spg,
            int use_bpg, double min_bpg, double max_bpg
            ) {
  /* Create SQL statement */
  std::stringstream ss;
  ss << "SELECT * from PLAYER ";
  bool has_where = false;
  if (use_mpg) {
    ss << "WHERE MPG BETWEEN " << min_mpg << " AND " << max_mpg;
    has_where = true;
  }
  if (use_ppg) {
    if (has_where) ss << ", ";
    ss << "WHERE PPG BETWEEN " << min_ppg << " AND " << max_ppg;
    has_where = true;
  }
  if (use_rpg) {
    if (has_where) ss << ", ";
    ss << "WHERE RPG BETWEEN " << min_rpg << " AND " << max_rpg;
    has_where = true;
  }
  if (use_apg) {
    if (has_where) ss << ", ";
    ss << "WHERE APG BETWEEN " << min_apg << " AND " << max_apg;
    has_where = true;
  }
  if (use_spg) {
    if (has_where) ss << ", ";
    ss << "WHERE SPG BETWEEN " << min_spg << " AND " << max_spg;
    has_where = true;
  }
  if (use_bpg) {
    if (has_where) ss << ", ";
    ss << "WHERE BPG BETWEEN " << min_bpg << " AND " << max_bpg;
    has_where = true;
  }
  ss << ";";
  /* Create a non-transactional object. */
  nontransaction N(*C);

  /* Execute SQL query */
  result R( N.exec( ss.str() ));
  /* List down all the records */
  printf("PID TID UNUM  FIRST_NAME    LAST_NAME   MPG PPG RPG APG  SPG   BPG \n");
//printf("000 000  00  abcdfghiihhi abcdfghiihhi  000 000 000 000 00.00 00.00\n");
  int mpg, ppg, rpg, apg;
  double spg, bpg;
  for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
    mpg = c[5].as<int>(); ppg = c[6].as<int>(); rpg = c[7].as<int>();
    apg = c[8].as<int>(); spg = c[9].as<double>(); bpg = c[10].as<double>();

    printf("%3d %3d  %2d  %-12s %-12s  %03d %03d %03d %03d %5.02f %5.02f\n",
      c[0].as<int>(), c[1].as<int>(), c[2].as<int>(), c[3].as<std::string>().c_str(),
      c[4].as<std::string>().c_str(), mpg, ppg, rpg, apg, spg, bpg);
  }
}


void query2(connection *C, string team_color) {
	/* Create SQL statement */
	std::stringstream ss;
	ss << "SELECT * from COLOR WHERE NAME = '" << team_color << "';";
	/* Create a non-transactional object. */
	nontransaction N(*C);
	/* Execute SQL query */
	result R( N.exec( ss.str() ));

	std::vector<int> color_id;
	for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
	  color_id.push_back(c[0].as<int>());
	}

	for (const auto& cid : color_id) {
	  ss.str("");
	  ss << "SELECT * from TEAM WHERE COLOR_ID = " << cid << ";";
	  /* Create a non-transactional object. */
	  // nontransaction N1(*C);
	  /* Execute SQL query */
	  result R1( N.exec( ss.str() ));

	  printf("TID        NAME        STATE_ID COLOR_ID WINS WINS\n");
	//  printf("000 abcdfghiihhiaaaa   000      000     00   00 \n",
	//  printf("%3d %12s   %3d         %3d     %2d   %2d\n",
	  for (result::const_iterator c = R1.begin(); c != R1.end(); ++c) {
      printf("%3d %16s   %3d         %3d     %2d   %2d\n",
        c[0].as<int>(), c[1].as<std::string>().c_str(), c[2].as<int>(),
        c[3].as<int>(), c[4].as<int>(), c[5].as<int>());
	  }
	}
}

/*!
 * SELECT column-list
 * FROM table_name
 * [WHERE condition]
 * [ORDER BY column1, column2, .. columnN] [ASC | DESC];
 */
void query3(connection *C, string team_name) {
  std::stringstream ss;
  ss << "SELECT * from TEAM WHERE NAME = '" << team_name << "';";
  /* Create a non-transactional object. */
  nontransaction N(*C);
  /* Execute SQL query */
  result R( N.exec( ss.str() ));

  std::vector<int> tids;
  for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
    tids.push_back(c[0].as<int>());
  }

  for (const auto& tid : tids) {
    ss.str("");
    ss << "SELECT * FROM PLAYER WHERE TEAM_ID = " << tid
       << "ORDER BY PPG DESC;";
    result R1( N.exec( ss.str() ));

    printf("PID TID UNUM  FIRST_NAME    LAST_NAME   MPG PPG RPG APG  SPG   BPG \n");
  //printf("000 000  00  abcdfghiihhi abcdfghiihhi  000 000 000 000 00.00 00.00\n");
    int mpg, ppg, rpg, apg;
    double spg, bpg;
    for (result::const_iterator c = R1.begin(); c != R1.end(); ++c) {
      mpg = c[5].as<int>(); ppg = c[6].as<int>(); rpg = c[7].as<int>();
      apg = c[8].as<int>(); spg = c[9].as<double>(); bpg = c[10].as<double>();

      printf("%3d %3d  %2d  %-12s %-12s  %03d %03d %03d %03d %5.02f %5.02f\n",
        c[0].as<int>(), c[1].as<int>(), c[2].as<int>(), c[3].as<std::string>().c_str(),
        c[4].as<std::string>().c_str(), mpg, ppg, rpg, apg, spg, bpg);
    }
  }
}


void query4(connection *C, string team_state, string team_color) {
  std::stringstream ss;
  ss << "SELECT * from STATE WHERE NAME = '" << team_state << "';";
  /* Create a non-transactional object. */
  nontransaction N(*C);
  /* Execute SQL query */
  result R( N.exec( ss.str() ));
  int sid = R.begin()[0].as<int>();

  ss.str("");
  ss << "SELECT * from COLOR WHERE NAME = '" << team_color << "';";
  result R1( N.exec( ss.str() ));
  int cid = R1.begin()[0].as<int>();

  ss.str("");
  ss << "SELECT * from TEAM WHERE STATE_ID = " << sid << " AND "
     << "COLOR_ID = " << cid << ";";

  result R2( N.exec( ss.str() ));
  std::vector<int> tids;
  for (result::const_iterator c = R2.begin(); c != R2.end(); ++c) {
    tids.push_back(c[0].as<int>());
  }

  for (const auto& tid : tids) {
    ss.str("");
    ss << "SELECT * FROM PLAYER WHERE TEAM_ID = " << tid << ";";
    result R3( N.exec( ss.str() ));

    printf("UNUM  FIRST_NAME    LAST_NAME\n");
  //printf(" 00  abcdfghiihhi abcdfghiihhi\n");
    for (result::const_iterator c = R3.begin(); c != R3.end(); ++c) {
      printf(" %2d  %-12s %-12s\n",
        c[2].as<int>(), c[3].as<std::string>().c_str(), c[4].as<std::string>().c_str());
    }
  }
}


void query5(connection *C, int num_wins) {
  std::stringstream ss;
  ss << "SELECT * from TEAM WHERE WINS > " << num_wins << ";";
  /* Create a non-transactional object. */
  nontransaction N(*C);
  /* Execute SQL query */
  result R( N.exec( ss.str() ));

  std::vector<int> tids;
  std::vector<int> wins;
  std::vector<std::string> tnames;
  for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
    tids.push_back(c[0].as<int>());
    wins.push_back(c[4].as<int>());
    tnames.push_back(c[1].as<std::string>());
  }

  printf(" FIRST_NAME   LAST_NAME    TEAM_NAME   WINS\n");
//printf("abcdfghiihhi abcdfghiihhi abcdfghiihhi  00\n");
  for (size_t i = 0; i < tids.size(); ++i) {
    ss.str("");
    ss << "SELECT * FROM PLAYER WHERE TEAM_ID = " << tids[i] << ";";

    result R1( N.exec( ss.str() ));
    for (result::const_iterator c = R1.begin(); c != R1.end(); ++c) {
      printf("%-12s %-12s %-12s  %2d\n",
        c[3].as<std::string>().c_str(), c[4].as<std::string>().c_str(),
        tnames[i].c_str(), wins[i]);
    }
  }
}
