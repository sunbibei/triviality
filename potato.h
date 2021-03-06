/*
 * potato.h
 *
 *  Created on: Feb 18, 2018
 *      Author: bibei
 */

#ifndef POTATO_H_
#define POTATO_H_

#include <stdio.h>
#include <assert.h>

// #define __DEBUG__

/*!
 * The protocol for communication
 * |        HEADER       |  NEXT_ID | ID_SIZE |  ID_LIST  |  HOPS   |        TAIL         |
 * | 0x88 0x89 0x00 0x00 |  4bytes  |  4bytes |  N*4bytes |  4bytes | 0x00 0x00 0x00 0x00 |
 * The initialize link message format
 * |        HEADER       | PLAYER NO.| TOTAL NO. |        TAIL         |
 * | 0x88 0x89 0x01 0x01 |  4bytes   |  4bytes   | 0x00 0x00 0x00 0x00 |
 * The initialize ring message format (decreped)
 * |        HEADER       | LFT_ADDR_LEN | LFT_NEIGHOR_ADDR  | RGT_ADDR_LEN | RGT_NEIGHOR_ADDR |        TAIL         |
 * | 0x88 0x89 0x02 0x02 |    4bytes    |       N bytes     |    4bytes    |      N bytes     | 0x00 0x00 0x00 0x00 |
 * The end message format
 * |        HEADER       |        TAIL         |
 * | 0x88 0x89 0xFF 0xFF | 0x00 0x00 0x00 0x00 |
 */
///! MSG_SIZE: means HOPS + ID_SIZE + ID_LIST
#define POTATO_MSG_SIZE(id_size)           (((id_size) + 5)*4)
#define POTATO_SERIALIZE_SIZE(id_size)     (((id_size) + 2)*4)
#define INTERGE_SIZE                       (sizeof(unsigned int))
#define INIT_LINK_MSG_SIZE                 (4 * 4)
#define END_MSG_SIZE                       (2 * 4)
const char HEAD[] = {0x88, 0x89};
enum {
  POTATO_IDX = 0,
  INIT_LINK_IDX,
  INIT_RING_IDX,
  END_MSG_IDX,
  N_MSG_IDX,
};
const char INDEX[][2] = { {0x00, 0x00}, {0x01, 0x01}, {0x02, 0x02}, {0xFF, 0xFF} };
const unsigned int HEAD_SIZE  = 2;
const unsigned int INDEX_SIZE = 2;
const unsigned int TAIL_SIZE  = 4;

void __packet_init_link_msg(char* buf, int id, int nums);
void __packet_end_msg(char* buf);

/*!
 * The prototype of Potato, and the methods.
 */
typedef struct {
  unsigned int  hops;
  unsigned int  id_size;
  unsigned int* id_list;
} Potato;

///! The parameter @size means the id_size, and will malloc the memory of id_list.
Potato* potato_malloc(unsigned int id_size) {
  Potato* _pp  = (Potato*)malloc(sizeof(Potato));
  memset(_pp, 0x00, sizeof(Potato));
  if (id_size <= 0)
    _pp->id_list = NULL;
  else
    _pp->id_list = (unsigned int*)malloc(id_size * sizeof(unsigned int));

  return _pp;
}

void    potato_free(Potato* _pp) {
  if (NULL == _pp) return;

  free(_pp->id_list);
  _pp->id_list = NULL;
  _pp->id_size = 0;
  free(_pp);
}

void potato_print_trace(const Potato* _potato) {
#ifdef __DEBUG__
  printf("Trace of potato<%d>:\n", _potato->hops);
#else
  printf("Trace of potato:\n");
#endif
  int idx = 0;
  if (0 == _potato->id_size) {
    printf("\n");
    return;
  }

  printf("<%d>", _potato->id_list[idx++]);
  while (idx < _potato->id_size)
    printf(",<%d>", _potato->id_list[idx++]);
  printf("\n");
}

///! The caller need to free the memory by self.
void potato_serialize(const Potato* _potato, char* buf) {
  assert(NULL != buf);

  unsigned int size = POTATO_SERIALIZE_SIZE(_potato->id_size);
  memset(buf, 0x00, size);

  int off = 0;
//  ///! header
//  memcpy(buf + off, HEAD, HEAD_SIZE);
//  off += HEAD_SIZE;
//  ///! index
//  memcpy(buf + off, INDEX[POTATO_IDX], INDEX_SIZE);
//  off += INDEX_SIZE;
  ///! id size
  memcpy(buf + off, &(_potato->id_size), sizeof(unsigned int));
  off += sizeof(unsigned int);
  ///! id list
  for (int idx = 0; idx < _potato->id_size; ++idx) {
    memcpy(buf + off, _potato->id_list + idx, sizeof(unsigned int));
    off += sizeof(unsigned int);
  }
  ///! hops
  memcpy(buf + off, &(_potato->hops), sizeof(unsigned int));
  off += sizeof(unsigned int);
}

///! The caller need to free the memory by self.
Potato* potato_unserialize(const char* _buf) {
//  ///! header
//  _buf += HEAD_SIZE;
  ///! id size
  unsigned int id_size = 0;
  memcpy(&id_size, _buf, sizeof(unsigned int));
  _buf += sizeof(unsigned int);
  Potato* _pp  = potato_malloc(id_size);
  _pp->id_size = id_size;
  ///! id list
  for (int idx = 0; idx < _pp->id_size; ++idx) {
    memcpy(_pp->id_list + idx, _buf, sizeof(unsigned int));
    _buf += sizeof(unsigned int);
  }
  ///! hops
  memcpy(&(_pp->hops), _buf, sizeof(unsigned int));
  _buf += sizeof(unsigned int);

  return _pp;
}

void __packet_init_link_msg(char* buf, int id, int nums) {
  assert(NULL != buf);

  ///! the initialize message.
  int off = 0;
  memset(buf, 0x00, INIT_LINK_MSG_SIZE);
  ///! header
  memcpy(buf + off, HEAD, HEAD_SIZE);
  off += HEAD_SIZE;
  ///! index
  memcpy(buf + off, INDEX[INIT_LINK_IDX], INDEX_SIZE);
  off += INDEX_SIZE;
  ///! the current player id.
  memcpy(buf + off, &id, sizeof(int));
  off += sizeof(int);
  ///! the total player number
  memcpy(buf + off, &nums, sizeof(int));
  off += sizeof(int);
  ///! the tail
  off += TAIL_SIZE;
}

void __packet_end_msg(char* buf) {
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
}

#endif /* POTATO_H_ */
