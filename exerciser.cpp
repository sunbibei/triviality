#include "exerciser.h"

void exercise(connection *C) {
  int num_wins = 0;
  std::string team_color, team_name, team_state;
  std::cout << "================================" << std::endl;
  std::cout << "              QUERY1            " << std::endl;
  std::cout << "================================" << std::endl;
  int use_mpg = 1; int min_mpg = 35; int max_mpg = 40;
  int use_ppg = 0; int min_ppg = 0; int max_ppg = 0;
  int use_rpg = 0; int min_rpg = 0; int max_rpg = 0;
  int use_apg = 0; int min_apg = 0; int max_apg = 0;
  int use_spg = 0; double min_spg = 0; double max_spg = 0;
  int use_bpg = 0; double min_bpg = 0; double max_bpg = 0;
  query1(C,
        use_mpg, min_mpg, max_mpg,
        use_ppg, min_ppg, max_ppg,
        use_rpg, min_rpg, max_rpg,
        use_apg, min_apg, max_apg,
        use_spg, min_spg, max_spg,
        use_bpg, min_bpg, max_bpg );

  std::cout << "================================" << std::endl;
  std::cout << "              QUERY2            " << std::endl;
  std::cout << "================================" << std::endl;
  team_color = "Maroon";
  query2(C, team_color);

  std::cout << "================================" << std::endl;
  std::cout << "              QUERY3            " << std::endl;
  std::cout << "================================" << std::endl;
  team_name = "Duke";
  query3(C, team_name);

  std::cout << "================================" << std::endl;
  std::cout << "              QUERY4            " << std::endl;
  std::cout << "================================" << std::endl;
  team_state = "NC";
  team_color = "DarkBlue";
  query4(C, team_state, team_color);

  std::cout << "================================" << std::endl;
  std::cout << "              QUERY5            " << std::endl;
  std::cout << "================================" << std::endl;
  num_wins = 10;
  query5(C, num_wins);
}
