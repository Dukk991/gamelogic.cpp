# gamelogic.cpp
a simple gamelogic for a button press - led game

I have provided the cpp and h files that together form a very simple gamelogic for a button - led game intented to be played on an arduino uno. (arduino uno has atmega328p microcontroller):

You start the game and the game starts generating numbers between 0 and 3. A corresponding led is set and the player must press the right button. If the correct button is pressed the game continues and increments the display by one. If the button press is wrong, the game ends.
After 10 cycles or 10 ledsets the number generating speed is incremented by 10%. This makes the game harder. The same number is not generated back to back. This allows the buttons to work correctly, because there is a 300 millisecond delay when you can press the same button again.
The player doesn't have to press the button when the led is set. The player can come in a delay, provided he still presses the buttons in the right sequence.

This was a schoolproject and my part was coding the gamelogic. There are additionals files (display.cpp/h, buttons.cpp/h and leds.cpp/h) that are requiered for the game to function. I don't feel that it would be right of me to add them here since I didn't code them personally. I can provide them on an invidual level if anyone wants. I can also provide the schematic for the arduino uno connections.
