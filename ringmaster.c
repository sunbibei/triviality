/*
 * ringmaster.c
 *
 *  Created on: Feb 18, 2018
 *      Author: bibei
 */

#define MASTER
#ifdef  MASTER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <netdb.h>

#include "potato.h"

const unsigned int MAX_BUF_SIZE = 1024*sizeof(char);

int main(int argc, char* argv[]) {
//  for (int i = 0; i < argc; ++i)
//    printf("[%d/%d]: %s ", i, argc-1, argv[i]);
//  printf("\n");
//  return 0;

  if (4 != argc) {
    printf("Usage: ringmaster <port_num> <num_players> <num_hops>\n");
    return -1;
  }
  int PORT_NUM    = atoi(argv[1]);
  int NUM_PLAYERS = atoi(argv[2]);
  int NUM_HOPS    = atoi(argv[3]);

//  const unsigned int MAX_NAME_SIZE = 128;
//  char name[MAX_NAME_SIZE]         = {0};
//  if (0 != gethostname ( name, MAX_NAME_SIZE)) {
//    printf("ERROR gethostname\n");
//    return -1;
//  }
//  printf("hostname: %s\n", name);

  char localhost[] = "localhost";
  // struct hostent* hostinfo = gethostbyname(name);
  struct hostent* hostinfo = gethostbyname(localhost);
  if (NULL == hostinfo) {
    printf("ERROR gethostbyname\n");
    return -1;
  }
  char *ip = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list);
  printf("ip: %s\n", ip);

  struct sockaddr_in** player_addrs = (struct sockaddr_in**)malloc(NUM_PLAYERS*sizeof(struct sockaddr_in));
  unsigned int* length_addrs = (unsigned int*)malloc(NUM_PLAYERS*sizeof(unsigned int));
  int* player_fds            = (int*)malloc(NUM_PLAYERS*sizeof(int));
  for (int i = 0; i < NUM_PLAYERS; ++i) {
    player_fds[i]   = -1;
    player_addrs[i] = NULL;
    length_addrs[i] = 0;
  }

  char* buf = (char*)malloc(MAX_BUF_SIZE);
  memset(buf, 0x00, MAX_BUF_SIZE);

  struct sockaddr_in servaddr;
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  // servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_addr.s_addr = inet_addr(ip);
  servaddr.sin_port = htons(PORT_NUM);

  fd_set read_set;
  fd_set write_set;
  fd_set select_read_set;
  FD_ZERO(&read_set);
  FD_ZERO(&write_set);
  FD_ZERO(&select_read_set);

  int ringmaster_fd = socket(AF_INET, SOCK_STREAM, 0);
  unsigned int value = -1;
  setsockopt(ringmaster_fd, SOL_SOCKET, SO_REUSEADDR, (void *)&value, sizeof(value));
  bind(ringmaster_fd, (struct sockaddr *)&servaddr, sizeof(servaddr));
  listen(ringmaster_fd, NUM_PLAYERS);
  FD_SET(ringmaster_fd, &read_set);

  printf("Potato Ringmaster\n");
  printf("Players = %d\n", NUM_PLAYERS);
  printf("Hops = %d\n",    NUM_HOPS);

  ///! waiting for the player connecting request.
  int num_player = 0;
  while (num_player != NUM_PLAYERS) {
    player_addrs[num_player] = (struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));
    length_addrs[num_player] = sizeof(player_addrs[num_player]);
    player_fds[num_player]   = accept(ringmaster_fd, (struct sockaddr*)(player_addrs[num_player]),
        length_addrs + num_player);

    __packet_init_link_msg(buf, num_player, NUM_PLAYERS);
    send(player_fds[num_player], buf, INIT0_MSG_SIZE, 0);

    ++num_player;
  }

  //////////////////////////////////////////////////////
  ///! send the RING INIT message
  //////////////////////////////////////////////////////
  for (num_player = 0; num_player < NUM_PLAYERS; ++num_player) {
    int off = 0;
    memset(buf, 0x00, MAX_BUF_SIZE);
    ///! header
    memcpy(buf + off, HEAD, HEAD_SIZE);
    off += HEAD_SIZE;
    ///! index
    memcpy(buf + off, INDEX[INIT_RING_IDX], INDEX_SIZE);
    off += INDEX_SIZE;
    ///! the length of left neighbor address
    int neighbor_id = num_player - 1;
    if (neighbor_id < 0) neighbor_id = NUM_PLAYERS - 1;
    memcpy(buf + off, length_addrs + neighbor_id, sizeof(int));
    off += sizeof(int);
    ///! the left neighbor address
    memcpy(buf + off, player_addrs + neighbor_id, length_addrs[neighbor_id]);
    off += length_addrs[num_player];

    ///! the length of right neighbor address
    neighbor_id = num_player + 1;
    if (neighbor_id >= NUM_PLAYERS) neighbor_id = 0;
    memcpy(buf + off, length_addrs + neighbor_id, sizeof(int));
    off += sizeof(int);
    ///! the right neighbor address
    memcpy(buf + off, player_addrs + neighbor_id, length_addrs[neighbor_id]);
    off += length_addrs[num_player];
    ///! the tail
    off += TAIL_SIZE;

    send(player_fds[num_player], buf, off, 0);
    printf("Player %d is ready to play\n", num_player);
  }

  ///! start to play the hot-potato
  printf("Ready to start the game, ");
  srand( (unsigned int) time(NULL) + NUM_PLAYERS );
  int player_id = rand() % NUM_PLAYERS;
  ///! create a potato object.
  Potato* potato  = potato_malloc(0);
  potato->hops    = NUM_HOPS;
  char* serialize = potato_serialize(potato);
  send(player_fds[player_id], serialize, POTATO_MSG_SIZE(potato->id_size), 0);
  printf("sending potato to player %d\n", player_id);
  free(serialize);
  serialize = NULL;

  //////////////////////////////////////////////////////
  ///! waiting to the game end.
  //////////////////////////////////////////////////////
  struct timeval timeout;
  Potato* _pp = NULL;
  while (1) {
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    int max_fd = ringmaster_fd;
    ///! find the maximum file descriptor.
    for (int i = 0; i < NUM_PLAYERS; ++i)
      if (max_fd < player_fds[i])
        max_fd = player_fds[i];

    select_read_set = read_set;
    int ret = select(max_fd + 1, &select_read_set, NULL, NULL, &timeout);
    if (ret <= 0) continue;

    int id = 0;
    for (; id < NUM_PLAYERS; ++id) {
      if (-1 == player_fds[id])
        continue;
      if (FD_ISSET(player_fds[id], &select_read_set))
        break;
    }

    ///! read the message.
    int total_size = 0;
    memset(buf, 0, MAX_BUF_SIZE);
    while (1) { // read the message and parse a potato object.
      int num = recv(player_fds[id], buf + total_size, MAX_BUF_SIZE - total_size, 0);
      if (num <= 0) continue;

      total_size += num;
      ///! find the header from the buffer and move to the head of buffer.
      num = 0;
      while (num <= total_size) {
        if ( (HEAD[0] != buf[num + 0]) || (HEAD[1] != buf[num + 1])
             || (INDEX[POTATO_IDX][0] != buf[num + 2]) || (INDEX[POTATO_IDX][1] != buf[num + 3]) ) {
          ++num;
          continue;
        }
        if (0 != num) {
          memmove(buf, buf + num, total_size - num);
          num = 0;
        }
        break;
      } // end while (num <= total_size)

      unsigned int id_size = 0;
      memcpy(&id_size, buf + (HEAD_SIZE + INDEX_SIZE), sizeof(unsigned int));
      if ((total_size - num) < 4*(id_size + 3)) continue;

      ///! get the whole potato message.
      _pp = potato_unserialize(buf + num);
      break;
//      if (_pp->hops > 0) {
//
//        --_pp->hops;
//        char* serialize = potato_serialize(_pp);
//        send(player_fds[rand() % NUM_PLAYERS], serialize, POTATO_MSG_SIZE(_pp->id_size), 0);
//
//        free(serialize);
//        serialize = NULL;
//      } else if (0 == _pp->hops) {
//        break; // end the game
//      } else {
//        ; // Unreached code
//      }
    } // end while(1) -- read message
    if (NULL != _pp) break;
  } // end while(1)   -- the main process.

  //////////////////////////////////////////////////////
  ///! endding the hot-potato game
  //////////////////////////////////////////////////////
  potato_print_trace(_pp);
  if ((NULL == _pp) || (0 != _pp->hops))
    printf("ERROR GAME HOPS\n");

  int off = 0;
  memset(buf, 0x00, END_MSG_SIZE);
  ///! header
  memcpy(buf + off, HEAD, HEAD_SIZE);
  off += HEAD_SIZE;
  ///! index
  memcpy(buf + off, INDEX[END_MSG_IDX], INDEX_SIZE);
  off += INDEX_SIZE;
  ///! the tail
  off += TAIL_SIZE;
  for (int id = 0; id < NUM_PLAYERS; ++id)
    send(player_fds[id], buf, END_MSG_SIZE, 0);

  //////////////////////////////////////////////////////
  ///! free the memory and exit the main process
  //////////////////////////////////////////////////////
  for (int i = 0; i < NUM_PLAYERS; ++i) {
    FD_CLR(player_fds[i], &read_set);
    close(player_fds[i]);

    free(player_addrs[i]);
  }
  free(player_addrs);
  free(length_addrs);
  free(player_fds);
  free(buf);

	return 0;
}

#endif
