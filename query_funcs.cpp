#include "query_funcs.h"

#include <vector>
#include <stringstream>

int __get_ava_player_id();
int __get_ava_team_id();
int __get_ava_state_id();
int __get_ava_color_id();
bool __fall_between(int, int, int);

void add_player(connection *C, int team_id, int jersey_num, string first_name, string last_name,
                int mpg, int ppg, int rpg, int apg, double spg, double bpg) {
	std::stringstream ss;

	ss << "INSERT INTO PLAYER(PLAYER_ID, TEAM_ID, UNIFORM_NUM, FIRST_NAME, LAST_NAME, MPG, PPG,
RPG, APG, SPG, BPG) ";
    ss << "VALUES (" << __get_ava_player_id() << ", " << team_id << ", "
       << jersey_num << ", '" << first_name << "', '" << last_name << "', "
       << mpg << ", "<< ppg << ", "<< rpg << ", "<< apg << ", "<< spg << ", "<< bpg << ");";

	/* Create a transactional object. */
	work W(C);

	/* Execute SQL query */
	W.exec( ss.str() );
	W.commit();
}


void add_team(connection *C, string name, int state_id, int color_id, int wins, int losses) {
	std::stringstream ss;

	ss << "INSERT INTO TEAM((TEAM_ID, NAME, STATE_ID, COLOR_ID, WINS, LOSSES) ";
    ss << "VALUES (" << __get_ava_team_id() << ", '" << name << "', '" << state_id << ", "
       << color_id << ", " << wins << ", " << losses << ");";

	/* Create a transactional object. */
	work W(C);

	/* Execute SQL query */
	W.exec( ss.str() );
	W.commit();
}


void add_state(connection *C, string name) {
	std::stringstream ss;

	ss << "INSERT INTO PLAYER(STATE_ID, NAME) ";
    ss << "VALUES (" << __get_ava_state_id() << ", '" << name << "');";

	/* Create a transactional object. */
	work W(C);

	/* Execute SQL query */
	W.exec( ss.str() );
	W.commit();
}


void add_color(connection *C, string name) {
	std::stringstream ss;

	ss << "INSERT INTO PLAYER(COLOR_ID, NAME) ";
    ss << "VALUES (" << __get_ava_color_id() << ", '" << name << "');";

	/* Create a transactional object. */
	work W(C);

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
	char* sql = "SELECT * from PLAYER";

	/* Create a non-transactional object. */
	nontransaction N(C);

	/* Execute SQL query */
	result R( N.exec( sql ));
	/* List down all the records */
	printf("PID TID UNUM  FIRST_NAME    LAST_NAME   MPG   PPG   RPG   APG   SPG   BPG \n");
//  printf("000 000  00  abcdfghiihhi abcdfghiihhi 00.00 00.00 00.00 00.00 00.00 00.00\n",
	int mpg, ppg, rpg, apg, spg, bpg;
	for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
		mpg = c[5].as<float>(); ppg = c[6].as<float>(); rpg = c[7].as<float>();
		apg = c[8].as<float>(); spg = c[9].as<float>(); bpg = c[10].as<float>();
		if (use_mpg && !__fall_between(mpg, min_mpg, max_mpg)) continue;
		if (use_ppg && !__fall_between(ppg, min_ppg, max_ppg)) continue;
		if (use_rpg && !__fall_between(rpg, min_rpg, max_rpg)) continue;
		if (use_apg && !__fall_between(apg, min_apg, max_apg)) continue;
		if (use_spg && !__fall_between(spg, min_spg, max_spg)) continue;
		if (use_bpg && !__fall_between(bpg, min_bpg, max_bpg)) continue;

		printf("%3d %3d  %2d  %12s %12s %5.02f %5.02f %5.02f %5.02f %5.02f %5.02f\n",
			c[0].as<int>(), c[1].as<int>(), c[2].as<int>(), c[3].as<std::string>(), c[4].as<std::string>(),
			mpg, ppg, rpg, apg, spg, bpg);
	}
}


void query2(connection *C, string team_color) {
	/* Create SQL statement */
	char* sql = "SELECT * from COLOR";
	/* Create a non-transactional object. */
	nontransaction N(C);
	/* Execute SQL query */
	result R( N.exec( sql ));

	std::vector<int> color_id;
	std::string color_name;
	for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
		color_name = c[1].as<string>();
		if (0 == color_name.compare(team_color))
			color_id.push_back(c[0].as<int>());
	}

	sql = "SELECT * from TEAM";
	/* Create a non-transactional object. */
	nontransaction N1(C);
	/* Execute SQL query */
	result R1( N1.exec( sql ));

	int t_color_id;
	std::string color_name;
	printf("TID     NAME     STATE_ID COLOR_ID WINS WINS\n");
//  printf("000 abcdfghiihhi   000      000     00   00 \n",
//  printf("%3d %12s   %3d      %3d     %2d   %2d\n",
	for (result::const_iterator c = R1.begin(); c != R1.end(); ++c) {
		t_color_id = c[3].as<int>();
		for (const auto& id : color_id) {
			if (id == t_color_id) {
				printf("%3d %12s   %3d      %3d     %2d   %2d\n",
					c[0].as<int>(), c[1].as<std::string>(), c[2].as<int>(),
					c[3].as<int>(), c[4].as<int>(), c[5].as<int>());
			}
		}
	}
}


void query3(connection *C, string team_name) {
}


void query4(connection *C, string team_state, string team_color) {
}


void query5(connection *C, int num_wins) {
}
