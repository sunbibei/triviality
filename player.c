/*
 * player.c
 *
 *  Created on: Feb 18, 2018
 *      Author: bibei
 */

#define PLAYER
#ifdef  PLAYER

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
enum {
  LFT_NGB = 0,
  RGT_NGB,
  N_NGB,
};

int main(int argc, char* argv[]) {
//  for (int i = 0; i < argc; ++i)
//    printf("[%d/%d]: %s ", i, argc-1, argv[i]);
//  printf("\n");
//  return 0;

  if (3 != argc) {
    printf("Usage: player <machine_name> <port_num>\n");
    return -1;
  }
  char* MACHINE_NAME = argv[1];
  int   PORT_NUM     = atoi(argv[1]);

  struct hostent* hostinfo = gethostbyname(MACHINE_NAME);
  if (NULL == hostinfo) {
    if (__DEBUG__) printf("ERROR gethostbyname\n");
    return -1;
  }
  char *ip = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list);
  if (__DEBUG__) printf("ip: %s\n", ip);

  char* buf = (char*)malloc(MAX_BUF_SIZE);
  memset(buf, 0x00, MAX_BUF_SIZE);

  int player_fd      = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in serv_addr;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT_NUM);
  serv_addr.sin_addr.s_addr = inet_addr(ip);
  int serv_addr_len = sizeof(serv_addr);
  if (connect(player_fd, (struct sockaddr *)&serv_addr, serv_addr_len) < 0) {
    if (__DEBUG__) perror("Fail to connect\n");
    return -1;
  }

  //////////////////////////////////////////////////////
  ///! waiting the LINK INIT message
  //////////////////////////////////////////////////////
  int total_size = 0;
  while (total_size < INIT_LINK_MSG_SIZE) {
    int num = recv(player_fd, buf, INIT_LINK_MSG_SIZE - total_size, 0);
    if (num > 0) total_size += num;

    num = 0;
    while ( (HEAD[0] != buf[num + 0]) || (HEAD[1] != buf[num + 1])
         || (INDEX[INIT_LINK_IDX][0] != buf[num + 2]) || (INDEX[INIT_LINK_IDX][1] != buf[num + 3]) ) {
      ++num;
      continue;
    }
    if (0 != num) {
      memmove(buf, buf + num, total_size - num);
      num = 0;
    }
  }
  ///! got the LINK INIT message.
  int PLAYER_ID   = 0;
  int NEIGHBORS[] = {-1, -1};
  int NUM_PLAYERS = 0;
  int off = 0;
  ///! eat the header and index
  off += (HEAD_SIZE + INDEX_SIZE);
  ///! parse the player id
  memcpy(&PLAYER_ID,   buf + off, INTERGE_SIZE);
  off += INTERGE_SIZE;
  ///! parse the total player number
  memcpy(&NUM_PLAYERS, buf + off, INTERGE_SIZE);
  off += INTERGE_SIZE;
  ///! calculate the left and right neighbor id
  NEIGHBORS[LFT_NGB] = PLAYER_ID - 1;
  if (NEIGHBORS[LFT_NGB] < 0) NEIGHBORS[LFT_NGB] = NUM_PLAYERS - 1;
  NEIGHBORS[RGT_NGB] = PLAYER_ID + 1;
  if (NEIGHBORS[RGT_NGB] >= NUM_PLAYERS) NEIGHBORS[RGT_NGB] = 0;
  ///! print the information
  printf("Connected as player %d out of %d total players", PLAYER_ID, NUM_PLAYERS);

  //////////////////////////////////////////////////////
  ///! waiting the POTATO message
  //////////////////////////////////////////////////////
  srand( (unsigned int) time(NULL) + PLAYER_ID );
  Potato* _pp         = NULL;
  int is_recv_end_msg = 0;
  while (!is_recv_end_msg) {
    ///! read the message.
    int next_player = -1;
    int total_size  = 0;
    memset(buf, 0, MAX_BUF_SIZE);
    while (1) { // read the message and parse a potato object.
      int num = recv(player_fd, buf + total_size, MAX_BUF_SIZE - total_size, 0);
      if (num <= 0) continue;

      total_size += num;
      if (total_size < POTATO_MSG_SIZE(1)) continue;
      ///! find the header from the buffer and move to the head of buffer.
      num = 0;
      while (num <= total_size) {
        if ( (HEAD[0] != buf[num + 0]) || (HEAD[1] != buf[num + 1]) ) {
          ++num;
          continue;
        }
        if ((INDEX[END_MSG_IDX][0] == buf[num + 2]) && (INDEX[END_MSG_IDX][1] == buf[num + 3])) {
          is_recv_end_msg = 1;
          break;
        }
        if ((INDEX[POTATO_IDX][0] != buf[num + 2]) || (INDEX[POTATO_IDX][1] != buf[num + 3])) {
          ++num;
          continue;
        }

        if (0 != num) {
          memmove(buf, buf + num, total_size - num);
          num = 0;
        }
        break;
      } // end while (num <= total_size)
      if (is_recv_end_msg) break;
      ///! the potato message, and the num is must be zero.
      unsigned int id_size = 0;
      memcpy(&id_size, buf + (HEAD_SIZE + INDEX_SIZE + INTERGE_SIZE), INTERGE_SIZE);
      if ((total_size - num) < POTATO_MSG_SIZE(id_size)) continue;
      ///! eat the head, index and the next player id.
      off = HEAD_SIZE + INDEX_SIZE + INTERGE_SIZE;
      ///! get the whole potato message.
      _pp = potato_unserialize(buf + off);
      break;
    } // end while(1) -- read message

    if (_pp->hops > 0) {
      Potato potato;
      potato.hops    = _pp->hops - 1;
      potato.id_size = _pp->id_size + 1;
      potato.id_list = (unsigned int*)malloc(potato.id_size*sizeof(unsigned int));

      off = 0;
      ///! header
      memcpy(buf + off, HEAD, HEAD_SIZE);
      off += HEAD_SIZE;
      ///! index
      memcpy(buf + off, INDEX[POTATO_IDX], INDEX_SIZE);
      off += INDEX_SIZE;
      ///! next id
      int next_player = rand() % N_NGB;
      memcpy(buf + off, NEIGHBORS + next_player, INTERGE_SIZE);
      off += INTERGE_SIZE;
      ///! potato serialize
      potato_serialize(&potato, buf + off);
      off += POTATO_SERIALIZE_SIZE(potato.id_size);
      ///! tail
      memset(buf + off, 0x00, TAIL_SIZE);
      off += TAIL_SIZE;

      send(player_fd, buf, off, 0);
      ///! free the potato
      free(potato.id_list);
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
  ///! free the memory and exit the main process
  //////////////////////////////////////////////////////
  potato_free(_pp);
  close(player_fd);
  free(buf);

  return 0;
}

#endif
