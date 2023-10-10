---
layout: post
title: Week 1 Making Embedded Systems
---

<!-- excerpt start -->
Week 1 of 10 of the Making Embedded Systems course.
<!-- excerpt end -->

I've signed up for Elicia Whites on-line course Making Embedded Systems. It's a ten week course with weekly homework and a long-running project of your own choice. You're supposed to spend 6-10 hours a week on the course, but I expect to at the very least double that.

This week was the introduction and I didn't spend that much time on the course. It's been fun hanging on the discord chat and I hope that more people start discussions. I'm very impressed by the careers of the course participants. There's a lot of experience to tap into there.

## The Wokwi simulator

I started off by experimenting with programming a simulated rp2040 on the [Wokwi online simulator](https://wokwi.com/) using the Pico SDK. I was amazed at how usable the simulator was. You can add ~50 different peripherals; hook up an in-simulator logic analyzer that exportc VCD-files; and create custom chips!

I only dipped my toes. Here's blinky for two leds:

```
#include <stdio.h>
#include "pico/stdlib.h"

#define HIGH_PIN 2
#define LOW_PIN 3

int main() {
  stdio_init_all();
  
  // Configure a pin that sources current.
  gpio_init(HIGH_PIN);
  gpio_set_dir(HIGH_PIN, GPIO_OUT);

  // Configure a pin that sinks current.
  gpio_init(LOW_PIN);
  gpio_set_dir(LOW_PIN, GPIO_OUT);
  gpio_set_outover(LOW_PIN, GPIO_OVERRIDE_INVERT);

  while (true) {
    printf("Hello world!\n");

    gpio_put(HIGH_PIN, 1);
    gpio_put(LOW_PIN, 1);
    sleep_ms(250);

    gpio_put(HIGH_PIN, 0);
    gpio_put(LOW_PIN, 0);
    sleep_ms(250);
  }
}
```

And here's a program that controls two leds using push-buttons, one polling and one interrupt-driven. I've set bounce to 0 in the simulators config file so that I don't have to handle debouncing.

```
#include <stdio.h>
#include "pico/stdlib.h"

#define PURPLE_LED 2
#define RED_LED 14
#define GREEN_BUTTON 3
#define BLUE_BUTTON 9

#define EDGE_LOW (1UL << 2)
#define EDGE_HIGH (1UL << 3)

void button_pressed(uint gpio, uint32_t events) {
  if (events & EDGE_LOW) {
    gpio_put(RED_LED, 1);
  } else {
    gpio_put(RED_LED, 0);
  }
}

int main() {
  stdio_init_all();

  // Init outputs.
  gpio_init(PURPLE_LED);
  gpio_set_dir(PURPLE_LED, GPIO_OUT);
  gpio_init(RED_LED);
  gpio_set_dir(RED_LED, GPIO_OUT);

  // Init inputs
  gpio_init(GREEN_BUTTON);
  gpio_pull_up(GREEN_BUTTON);
  gpio_init(BLUE_BUTTON);
  gpio_pull_up(BLUE_BUTTON);

  gpio_set_irq_enabled_with_callback(BLUE_BUTTON, EDGE_LOW | EDGE_HIGH, 
      /*enabled=*/true, button_pressed);

  while (true) {
    if (!gpio_get(GREEN_BUTTON)) {
      gpio_put(PURPLE_LED, 1);
    } else {
      gpio_put(PURPLE_LED, 0);
    }
    sleep_ms(1);
  }
}
```

And finally a program that lights a led using PWM:

```
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define LED 2

int main() {
    stdio_init_all();

    gpio_set_function(LED, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(LED);
    pwm_set_wrap(slice_num, 256);
    pwm_set_chan_level(slice_num, PWM_CHAN_A, 128);
    pwm_set_enabled(slice_num, true);
    
    while (true) {
      sleep_ms(1);
    }
}
```

I had some problems with the PWM library and the [pico-examples repo](https://github.com/raspberrypi/pico-examples) was very useful as a reference. The documentation for the SDK is a little hard to navigate. I found a dash docset which was quite an improvement.

## The STMCubeMX software stack

ST Microelectronics provides their own IDE, STM32CubeIDE based on Eclipse. I followed course mate Dananjaya Ramanayakes instructions for installing the VSCode extension that allows me to import CubeMX projects. This blog post was also helpful : [Importing ST projects into Visual Studio Code](https://devblogs.microsoft.com/cppblog/importing-st-projects-into-visual-studio-code/).

I verified that I could install a CubeMX generated project on a STM32F4xx nucleo board. So far, so good! It's gonna take quite some time for me to learn the ins and outs of CubeMX and I'm a bit skeptical of all this automatic code generation. But it will be fun to experiment with it!

I asked some questions about how to wrap the generated HAL behind your own interface. You can split the generated code into separate .c/.h files which probably will be useful. And you can also choose whether to generate driver code using HAL or the more low-level LL APIs. I hope I'll figure it out once I start coding.

## Architecture diagrams

This weeks assignment was to create software block diagrams, hierarchical control diagrams and layered software diagrams for a product of your own choosing. I started out by trying to draw the diagrams for the ipod mini and a point-and-shoot camera. I realized that both of them probably relies on Cortex-A5s or similar and not cortex-M microcontrollers. When I started thinking about different products I noticed that it was easier for me to find Cortex-A powered units than Cortex-M ones. I asked around a bit about the performance/cost tradeoffs. Here's some things I learned:

* Cortex-M uses bigger process nodes. Means less perf per amp but cheaper production
* ST has been using 90nm, some low-power devices are built using 40nm.
* Cortex-M can draw 1uA in sleep mode. A Cortex-A draws astleast 300uA!
* Cortex-A 32 bit processors are divided into in-order power efficient cores (Cortex-A5, Cortex-A7) and out-of-order speedrunners (Cortex-A15, Cortex-A17).
* The processors closest in performance and price to Cortex-M parts are Cortex-A5 and Cortex-A7.
* Cortex-A parts like some Allwinner chips will be in the same price range as the 5-10 USD for a STM32F1xx MCU
* For SOM chips like an RPI compute module you pay more - about 30 USD.
* There exists a power efficiency benchmark for Cortex-M parts: https://www.eembc.org/ulpmark/ulp-cp/scores.php

Back to the architecture diagrams: I found them useful but designing things are always hard. I wish there was a large samples of good reference materials for designs and that you could compare your solutions to something that's considered good by many people. On the other hand, design is a sloppy business, I probably should just get used to "flying blind".

