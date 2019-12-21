#ifndef RINGBUF_H__
#define RINGBUF_H__

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct ringbuf {
  char* buffer;
  char* head;
  char* tail;
  size_t size;
};

size_t ringbuf_elements(struct ringbuf* rb);
int ringbuf_put(struct ringbuf* rb, uint8_t b);
int ringbuf_get(struct ringbuf* rb);

inline size_t ringbuf_size(struct ringbuf* rb) { return rb->size; }
//inline int ringbuf_peek(struct ringbuf* rb) { return (ringbuf_elements(rb) ? *rb->tail : -1); }
int ringbuf_peek(struct ringbuf* rb);
inline void ringbuf_init(struct ringbuf* rb, char* buf, size_t buflen)
{
  rb->buffer = buf;
  rb->head = buf;
  rb->tail = buf;
  rb->size = buflen;
}

#ifdef __cplusplus
}
#endif

#endif
