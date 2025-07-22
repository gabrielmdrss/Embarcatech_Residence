# Foundational Stage

### The first stage of the Embarcatech program was marked by a leveling phase. Because the program accepts applications from people with different majors and different levels of knowledge, it was necessary to establish a period during which classes would be taught and activities conducted to ensure collective leveling among residents.

### During the leveling phase, activities were requested for both subjects.

<details> <summary><h2 style="display: inline;">Digital Systems</h2></summary>
<p>

### `01.` [tarefa2](tarefa2)

For Unit 2 of the Digital Systems course, the logic gates that form logic circuits as we know them were studied. To build on the knowledge acquired in class, a code was proposed using the Raspberry Pi Pico W. Using an SSD1306 display, an interactive menu would be created, with navigation using the BitDogLab board's joystick. 

The menu would display the seven fundamental logic gates. When accessing the options using the A and B buttons on the board, the logic gate output would be visible on the LED indicator, either green or red, indicating a 0 or a 1.

The assembly features the presence of:
- Raspberry Pi Pico W
- Joystick
- SSD1306 OLED Display
- 2 pushbuttons
- 1 LED RGB

</p>

<p>

### `02.` [tarefa4](tarefa4)

As part of the Unit 4 assignment of the leveling phase, a scenario was generated involving access permission to a university campus. Four conditions had to be met for a doorman to allow someone to enter. In this assignment, residents were asked to extract a truth table with the values of all possible variables for the four variables and then derive the Boolean expression to obtain the system's output S.

The BitDogLab board was used to create code that allowed the user to change the values of the four variables using a joystick and a pushbutton. The 5x5 LED matrix would indicate the values of the four variables and the output S in the last row of the matrix, where green would represent 1 and red would represent 0.
</p>

The assembly features the presence of:
- Raspberry Pi Pico W
- Joystick
- SSD1306 OLED Display
- 1 pushbuttons
- WS1828B LED matrix

</details>

<br>
<details> <summary><h2 style="display: inline;">Electronics</h2></summary>
<p>
<p>

### `03.` [tarefa8_eletro](tarefa8_eletro)
During the eighth and final unit, a fire sensor was requested. In addition to the BitDogLab development board, two separate circuits were to be built for flame detection. The students assembled and soldered the components using universal printed circuit boards.

One of the boards consisted of a photodiode, which, upon detecting flames, would conduct in reverse, causing a transistor to switch, thus activating an infrared LED. Separately, the other board would wait for this infrared signal. This board had another photodiode, which, upon conducting in reverse, would indicate a "1" for a GPIO input. Upon detecting the input, BitDogLab should trigger a visual and audible alarm.

The assembly features the presence of:
- Raspberry Pi Pico W
- 2 photodiode
- 1 transistor
- 1 infrared LED
- 1 buzzer
- WS1828B LED matrix

</details>