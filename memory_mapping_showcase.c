#include <fcntl.h>
#include <stdint.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>

int main(void)
{
int fd = open("/dev/mem", O_RDWR | O_SYNC);
volatile uint32_t *b = mmap(NULL, 0x1000, PROT_READ | PROT_WRITE,
MAP_SHARED, fd, 0xFF200000);

const uint8_t d[10] = {0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78, 0x00, 0x10};

/* ---- HEX: show 123456 (HEX5 is the leftmost display) ---- */
b[84] = d[6]; /* HEX0 (0x0150) */
b[80] = d[5]; /* HEX1 (0x0140) */
b[76] = d[4]; /* HEX2 (0x0130) */
b[72] = d[3]; /* HEX3 (0x0120) */
b[68] = d[2]; /* HEX4 (0x0110) */
b[64] = d[1]; /* HEX5 (0x0100) */

/* ---- Button state ---- */
int btn_index = 96; // button PIO offset / 4 (0x180 / 4)
uint32_t prev = 0;
int first_flag = 0;

/* ---- Switch state ---- */
uint32_t sw_prev = 0xFFFFFFFF; // impossible value, so the first read always prints

/* ---- LED state ---- */
int led_n = 0;     // which LED is active
int led_ticks = 0; // 100 ms ticks since this LED started its cycle
b[88] = 0;         // ensure all LEDs are off

int n;
while (1)
{
/* ---- Buttons ---- */
uint32_t now = b[btn_index] & 0xF;
uint32_t pressed = prev & ~now;  // Define presses
uint32_t released = now & ~prev; // Define releases

for (n = 0; n < 4; n++)
{
if (((pressed | released) >> n) & first_flag) // Only the buttons that changed
{
printf("Button %d: %s\n", n,
((now >> n) & 1) ? "released" : "pressed");
}
}

prev = now;
first_flag = 1;

/* ---- Switches ---- */
uint32_t all = b[92] & 0x3FF; // all ten switches as a number

if (all != sw_prev)
{
for (n = 0; n < 10; n++)
{
if (((all ^ sw_prev) >> n) & 1) // only the switches that changed
printf("Switch %d changed from %s to %s\n",
n,
((all >> n) & 1) ? "OFF" : "ON",
((all >> n) & 1) ? "ON" : "OFF");
}
sw_prev = all;
}

/* ---- LEDs: on 1 s, off 1 s, then next LED ---- */
if (led_ticks == 0)
b[88] |= (1u << led_n);  // turn LEDR n on
else if (led_ticks == 10)
b[88] &= ~(1u << led_n); // turn LEDR n off

led_ticks++;
if (led_ticks == 20)
{
led_ticks = 0;
led_n = (led_n + 1) % 10;
}

usleep(100000); // 100 ms tick
}
}