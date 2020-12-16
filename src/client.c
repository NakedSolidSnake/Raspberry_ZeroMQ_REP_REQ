#include <czmq.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  char buffer[120] = {0};
  if(argc != 3)
  {
    fprintf(stderr, "Usage: %s <ip> <command>.\ncommand - LED ON\n         - LED OFF\n", argv[0]);
    return EXIT_FAILURE;
  }
  snprintf(buffer, 120, "tcp://%s:5000", argv[1]);
  zsock_t *requester = zsock_new_req(buffer);

  zstr_send(requester, argv[2]);
  char *string = zstr_recv(requester);
  puts(string);
  zstr_free(&string);
  zsock_destroy(&requester);
  return 0;
}