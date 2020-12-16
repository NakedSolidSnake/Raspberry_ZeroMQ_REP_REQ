#include <czmq.h>
#include <led.h>
#include <string.h>

static LED_t led =
    {
        .gpio.pin = 0,
        .gpio.eMode = eModeOutput};

typedef struct
{
  const char *command;
  eState_t state;
} Map;

static void readButton(void);
static int state = 0;
static int commandExecute(const char *command);

Map map[] =
    {
        {.command = "LED ON", .state = eStateHigh},
        {.command = "LED OFF", .state = eStateLow},
};

const int map_amount = sizeof(map) / sizeof(map[0]);

int main(void)
{
  int ret;
  if (LED_init(&led))
    return EXIT_FAILURE;

  zsock_t *server = zsock_new_rep("tcp://*:5000");

  while (1)
  {
    char *string = zstr_recv(server);
    puts(string);
    ret = commandExecute(string);
    zstr_free(&string);

    if (!ret)
      zstr_send(server, "ack");
    else
      zstr_send(server, "nack");
  }

  zsock_destroy(&server);
  return 0;
}

static int commandExecute(const char *command)
{
  int ret = -1;
  for (int i = 0; i < map_amount; i++)
  {
    if (strncmp(command, map[i].command, strlen(map[i].command)) == 0)
    {
      LED_set(&led, map[i].state);
      ret = 0;
    }
  }

  return ret;
}