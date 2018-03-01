/*
 * ringmaster.c
 *
 *  Created on: Feb 18, 2018
 *      Author: bibei
 */

// #define MASTER
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

  // char localhost[] = "localhost";
  struct hostent* hostinfo = gethostbyname(name);
  // struct hostent* hostinfo = gethostbyname(localhost);
  if (NULL == hostinfo) {
#ifdef __DEBUG__
    printf("ERROR gethostbyname\n");
#endif
    return -1;
  }
  char *ip = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list);
#ifdef __DEBUG__
  printf("ip: %s\n", ip);
#endif

  int* player_fds = (int*)malloc(NUM_PLAYERS*sizeof(int));
  for (int i = 0; i < NUM_PLAYERS; ++i)
    player_fds[i]   = -1;


  char* buf = (char*)malloc(MAX_BUF_SIZE);
  memset(buf, 0x00, MAX_BUF_SIZE);

  struct sockaddr_in servaddr;
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  // servaddr.sin_addr.s_addr = htons(INADDR_ANY);
  servaddr.sin_addr.s_addr = inet_addr(ip);
  servaddr.sin_port = htons(PORT_NUM);

//  fd_set read_set;
//  fd_set write_set;
//  fd_set select_read_set;
//  FD_ZERO(&read_set);
//  FD_ZERO(&write_set);
//  FD_ZERO(&select_read_set);

  int ringmaster_fd = socket(AF_INET, SOCK_STREAM, 0);
  // unsigned int value = -1;
  // setsockopt(ringmaster_fd, SOL_SOCKET, SO_REUSEADDR, (void *)&value, sizeof(value));
  bind(ringmaster_fd, (struct sockaddr *)&servaddr, sizeof(servaddr));
  listen(ringmaster_fd, NUM_PLAYERS);
  // FD_SET(ringmaster_fd, &read_set);

  printf("Potato Ringmaster\n");
  printf("Players = %d\n", NUM_PLAYERS);
  printf("Hops = %d\n",    NUM_HOPS);

  ///! waiting for the player connecting request.
  int num_player = 0;
  while (num_player != NUM_PLAYERS) {
    struct sockaddr_in player_addr;
    int addr_len = 0;
#ifdef __DEBUG__
    printf("Waiting the player come to connect\n");
#endif
    player_fds[num_player] = accept(ringmaster_fd, (struct sockaddr*)(&player_addr), (socklen_t*)&addr_len);
#ifdef __DEBUG__
    printf("Got a player<%d>\n", num_player);
#endif
    __packet_init_link_msg(buf, num_player, NUM_PLAYERS);
#ifdef __DEBUG__
    for (int i = 0; i < INIT_LINK_MSG_SIZE; ++i)
      printf("0x%02X ", buf[i]);
    printf("\n");
#endif
    send(player_fds[num_player], buf, INIT_LINK_MSG_SIZE, 0);

    printf("Player %d is ready to play\n", num_player);
    ++num_player;
  }

  ///! start to play the hot-potato
  printf("Ready to start the game, ");
  srand( (unsigned int) time(NULL) + NUM_PLAYERS );
  int player_id = rand() % NUM_PLAYERS;
  ///! create a potato object.
  Potato* potato  = potato_malloc(0);
  potato->hops    = NUM_HOPS;

  int off = 0;
  ///! header
  memcpy(buf + off, HEAD, HEAD_SIZE);
  off += HEAD_SIZE;
  ///! index
  memcpy(buf + off, INDEX[POTATO_IDX], INDEX_SIZE);
  off += INDEX_SIZE;
  ///! next id
  memset(buf + off, 0x00, INTERGE_SIZE);
  off += INTERGE_SIZE;
  ///! potato serialize
  potato_serialize(potato, buf + off);
  off += POTATO_SERIALIZE_SIZE(potato->id_size);
  ///! tail
  memset(buf + off, 0x00, TAIL_SIZE);
  off += TAIL_SIZE;

  send(player_fds[player_id], buf, off, 0);
#ifdef __DEBUG__
    printf("POTATO message<%d>:\n", off);
    for (int i = 0; i < off; ++i)
      printf("0x%02X ", buf[i]);
    printf("\n");
    printf("==================================================\n");
    printf("==================START GAME======================\n");
    printf("==================================================\n");
#endif
  printf("sending potato to player %d\n", player_id);

  //////////////////////////////////////////////////////
  ///! waiting to the game end or pass to the next player.
  //////////////////////////////////////////////////////
  // struct timeval timeout;
  Potato* _pp = NULL;
  int next_player = player_id;
  while (1) {
//    timeout.tv_sec = 5;
//    timeout.tv_usec = 0;

//    int max_fd = ringmaster_fd;
//    ///! find the maximum file descriptor.
//    for (int i = 0; i < NUM_PLAYERS; ++i)
//      if (max_fd < player_fds[i])
//        max_fd = player_fds[i];
//
//    select_read_set = read_set;
//    int ret = select(max_fd + 1, &select_read_set, NULL, NULL, &timeout);
//    if (ret <= 0) continue;
//#ifdef __DEBUG__
//    printf("something is coming...\n");
//#endif
//    int id = 0;
//    for (; id < NUM_PLAYERS; ++id) {
//      if (-1 == player_fds[id]) continue;
//      if (FD_ISSET(player_fds[id], &select_read_set)) break;
//    }
#ifdef __DEBUG__
    printf("thing comes from palyer id<%d>\n", next_player);
#endif

    ///! read the message.
    int total_size  = 0;
    memset(buf, 0, MAX_BUF_SIZE);
    while (1) { // read the message and parse a potato object.
      int num = recv(player_fds[next_player], buf + total_size, MAX_BUF_SIZE - total_size, 0);
      if (num <= 0) continue;
#ifdef __DEBUG__
      printf("recv message<%d>:\n", num);
      for (int i = total_size; i < total_size + num; ++i)
        printf("0x%02X ", buf[i]);
      printf("\n");
#endif
      total_size += num;
      if (total_size < POTATO_MSG_SIZE(0)) continue;
      ///! find the header from the buffer and move to the head of buffer.
      num = 0;
      while (num <= total_size) {
        if ( (HEAD[0] != buf[num + 0]) || (HEAD[1] != buf[num + 1])
             || (INDEX[POTATO_IDX][0] != buf[num + 2]) || (INDEX[POTATO_IDX][1] != buf[num + 3]) ) {
#ifdef __DEBUG__
      printf("no found the header\n");
#endif
          ++num;
          continue;
        }
        if (0 != num) {
          memmove(buf, buf + num, total_size - num);
          num = 0;
        }
        break;
      } // end while (num <= total_size)

      ///! the num is must be zero.
      unsigned int id_size = 0;
      memcpy(&id_size, buf + (HEAD_SIZE + INDEX_SIZE + sizeof(unsigned int)), sizeof(unsigned int));
      if ((total_size - num) < POTATO_MSG_SIZE(id_size)) continue;
      ///! eat the head and index.
      off = HEAD_SIZE + INDEX_SIZE;
      ///! parse the next player id.
      next_player = -1;
      memcpy(&next_player, buf + off, INTERGE_SIZE);
      off += INTERGE_SIZE;
      ///! get the whole potato message.
      _pp = potato_unserialize(buf + off);
      break;
    } // end while(1) -- read message
    if (_pp->hops > 0) {
      off = 0;
      ///! header
      memcpy(buf + off, HEAD, HEAD_SIZE);
      off += HEAD_SIZE;
      ///! index
      memcpy(buf + off, INDEX[POTATO_IDX], INDEX_SIZE);
      off += INDEX_SIZE;
      ///! next id
      memset(buf + off, 0x00, INTERGE_SIZE);
      off += INTERGE_SIZE;
      ///! potato serialize
      potato_serialize(_pp, buf + off);
      off += POTATO_SERIALIZE_SIZE(_pp->id_size);
      ///! tail
      memset(buf + off, 0x00, TAIL_SIZE);
      off += TAIL_SIZE;
#ifdef __DEBUG__
    printf("\n");
    printf("------------------GOT POTATO----------------------\n");
    printf("sending potato to player %d\n", next_player);
    potato_print_trace(_pp);

//    printf("message<%d>:\n", off);
//    for (int i = 0; i < off; ++i)
//      printf("0x%02X ", buf[i]);
//    printf("\n");
    printf("--------------------------------------------------\n");
#endif
      send(player_fds[next_player], buf, off, 0);
      ///! free the potato
      potato_free(_pp);
      _pp = NULL;
    } else if (0 == _pp->hops) {
      break;
    } else {
      ; // Unreached code
    }
  } // end while(1)   -- the main process.

  //////////////////////////////////////////////////////
  ///! endding the hot-potato game
  //////////////////////////////////////////////////////
  potato_print_trace(_pp);
  if ((NULL == _pp) || (0 != _pp->hops))
    printf("ERROR GAME HOPS\n");

  __packet_end_msg(buf);
  for (int id = 0; id < NUM_PLAYERS; ++id)
    send(player_fds[id], buf, END_MSG_SIZE, 0);

  //////////////////////////////////////////////////////
  ///! free the memory and exit the main process
  //////////////////////////////////////////////////////
  for (int i = 0; i < NUM_PLAYERS; ++i) {
    // FD_CLR(player_fds[i], &read_set);
    close(player_fds[i]);
  }
  free(player_fds);
  free(buf);

	return 0;
}

#endif
