#include "ringbuf.h"
int ringbuf_peek(struct ringbuf* rb) { return (ringbuf_elements(rb) ? *rb->tail : -1); }

size_t ringbuf_elements(struct ringbuf* rb) {
  if(rb->head >= rb->tail) {
    return (rb->head - rb->buffer) - (rb->tail - rb->buffer);
  }
  return rb->size - (rb->tail - rb->buffer) + (rb->head - rb->buffer);
}

int ringbuf_put(struct ringbuf* rb, uint8_t b) {
  if(ringbuf_elements(rb) == ringbuf_size(rb)) return 0;

  *rb->head = b;
  rb->head++;
  if((rb->head - rb->buffer) >= (int)rb->size) {
    rb->head = rb->buffer;
  }

  return 1;
}

int ringbuf_get(struct ringbuf* rb) {
  int peek = ringbuf_peek(rb);
  rb->tail++;
  if((rb->tail - rb->buffer) >= (int)rb->size) {
    rb->tail = rb->buffer;
  }
  return peek;
}

