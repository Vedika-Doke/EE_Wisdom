# 555 Timer (Epic Device)

The basic 555 timer gets its name from the fact that there are three internally connected 5kΩ resistors which it uses to generate the two comparators reference voltages. <br/>

![Internal block diagram](https://drive.google.com/uc?export=view&id=1OJla2O5AveLq8uz_aVXyqjAeG9pbhUPF)

- Pin 1 is connected to the ground
- Pin 8 is connected to the source voltage (Vcc): This is the power supply pin and for general purpose TTL 555 timers is between 4.5V and 15V.
- Pin 3 is the output

## Key points
- Vcc voltage is divided using resistors and a voltage of (2/3)Vcc is applied at the inverting terminal of the upper comparator and a voltage of (1/3) Vcc is applied at the non-inverting terminal of the lower comparator.
  - A comparator gives HIGH output if the voltage at non-inverting terminal surpasses the voltage at the inverting terminal of the comparator.
- The output from the comparator goes as input to the RS flip-flop attached.
  - ![TTforRS](https://drive.google.com/uc?export=view&id=1wm2tee6MkKIdn_jQ-iHMQoeVoQ4rHN4M)
  - Since the threshold and trigger pins control the output of the comparators, which further control the output of the RS latch, the threshold and trigger pins control the output
- Pin number 5, the control signal/pin can also affect the output. The control pin can be used to apply a separate external voltage affecting the net output. When not used it is connected to ground via a 10nF capacitor to eliminate any noise.
- To change the threshold signal, external resistor and capacitors are connected from the threshold pin to the discharge pin at 7.
  - If the output is 0, Q' (Q bar) is 1 as the output block is inverting. Since Q' is 1, the gate for the transistor receives a HIGH signal, effectively closing the path and providing a low impeadance path for current, The capacitor therefore shall discharge
  - ![Discharge](https://drive.google.com/uc?export=view&id=1bSE3W5zgg3_AUXMPHIRKEF759zF89saO)
- Pin number 4, the reset pin (or the logic zero pin) is such that when logic zero is provided to the pin, the flip-flop gets reset. This is an active-low input and is generally connected to a logic “1” level when not used to prevent any unwanted resetting of the output.

## Uses

The most common use of the 555 timer oscillator is as a simple astable oscillator by connecting two resistors and a capacitor across its terminals to generate a fixed pulse train with a time period determined by the time constant of the RC network. <br/>
But the 555 timer oscillator chip can also be connected in a variety of different ways to produce Monostable or Bistable multivibrators as well as the more common Astable Multivibrator. <br/>

What are Multivibrators? 
https://drive.google.com/file/d/1n4iA6rgRZpMGuqQHu8mS7F65r6oXr7DS/view?usp=drive_link

### The Monostable 555 Timer:
#### Applications of a monostable multivibrator:
- Generating time delays
- Frequency division
- Pulse Width Modulation (PWM)
- Switching the relay <br/>

![Monostable](https://drive.google.com/uc?export=view&id=1nYg16oM8wLb6ZKKgr6CLSrgqAHuLRvTw)
When trigger signal is applied, the output signal changes, which further initiates the charging of a capacitor, when the voltage across the capacitor  reaches (2/3)Vcc, the output values changes, this provides a path for discharging of the capacitor. This elegant systems gives rise to the Monostable Multivibrator.
![MonoGraph](https://drive.google.com/uc?export=view&id=1CVJ84kkuEfTKlLk3yw-7k2RK3wU6Fna7)
- The time duration of this pulse (T) is given as T = 1.1 * R1 * C1
- A Monostable Multivibrator using 555 Timer can be used for frequency division.
  - It is important to note that the time period between two trigger signals must be less than the time period of the output pulse.
  - ![FrequencyDivision](https://drive.google.com/uc?export=view&id=1N-5myDXvdzHkf1HqA4G5-tybZJ1L_VkT)

### Bistable Multivibrator:
The switching of the output waveform is achieved by controlling the trigger and reset inputs of the 555 timer which are held “HIGH” by the two pull-up resistors, R1 and R2. By taking the trigger input (pin 2) “LOW”, switch in set position, changes the output state into the “HIGH” state and by taking the reset input (pin 4) “LOW”, switch in reset position, changes the output into the “LOW” state.
- Since threshold is connected to the ground, the ouput of that comparator will always give a LOW value.
- When the set pin is closed, the trigger pin is pulled to LOW momentarily, so the output of the second comparator will give logic HIGH. The RS latch will have S=1 and R=0, therefore the output will give a logic HIGH.
- Once SET switch is released/opened, the voltage at pin2 will become HIGH, so the SR latch will have S=0 and R=0, which retains the previous output

- When the RESET pin is closed and pulled LOW, flip-flop will reset to logic LOW or 0 value. Hence, the ouput will be pulled to LOW
- In this condition, as we are not really changing the values of thershold and reset pin, even after opening/releasing the RESET pin switch, the output of the timer will stay LOW as R=0, S=0.
- Unlike the normal flip-flop circuits, this Bistable Multivibrator will work at higher voltages, (4.15V-15V).
![BistableGraph](https://drive.google.com/uc?export=view&id=101NAlITQ4gA9Cs1KC6R8KtHOppVYkuPs)
This 555 timer circuit will remain in either state indefinitely and is therefore bistable. Then the Bistable 555 timer is stable in both states, “HIGH” and “LOW”. The threshold input (pin 6) is connected to ground to ensure that it cannot reset the bistable circuit as it would in a normal timing application.
 
### Sinking/Sourcing the 555 timer output:
![Sink/Source](https://drive.google.com/uc?export=view&id=1jhiOZo8lDOa_uexuxtrvfUn1GPfda5c8)
1. When the load is connected between +Vcc and the output terminal(Figure in LHS), the current will “Sink” (absorb) or flow into the 555 timer output terminal and the LED will be “ON” when the output is “LOW”.
2. The second circuit above shows that the LED is connected between the output pin 3 and ground ( 0v ). This means that the current will “Source” (supply) or flow out of the 555 timers output terminal and the LED will be “ON” when the output is “HIGH”.
3. Owing to the limited range of 200mA sourcing, only devices which consume less current can be sourced. So if we wanted to switch or control higher power devices such as motors, electromagnets, relays or loudspeakers. Then we would need to use a Transistor to amplify the 555 timers output in order to provide a sufficiently high enough power to drive the load.
4. - ![TransistorDriver](https://drive.google.com/uc?export=view&id=1A28VJEqmN57boMQY-hDC5WhESN5xr69p)
   - The transistor in the two examples above, can be replaced with a Power MOSFET device or Darlington transistor if the load current is high.
   - When using an inductive load such as a motor, relay or electromagnet, it is advisable to connect a freewheeling (or flywheel) diode directly across the load terminals to absorb any back emf voltages generated by the inductive device when it changes state.

#### Practical Takeaway:
- If you're driving something like an LED, relay, or transistor base, it's almost always better to use the 555 to sink current (i.e., connect the load between Vcc and the output).
- Don't rely on the 555 to source significant current unless you're sure the load is tiny.

### 555 Oscillator (Astable Multivibrator):
The 555 Oscillator is another type of relaxation oscillator for generating stabilized square wave output waveforms of either a fixed frequency of up to 500kHz or of varying duty cycles from 50 to 100%. <br/>
In order to get the 555 Oscillator to operate as an astable multivibrator it is necessary to continuously re-trigger the 555 IC after each and every timing cycle.<br/>
This re-triggering is basically achieved by connecting the trigger input (pin 2) and the threshold input (pin 6) together, thereby allowing the device to act as an astable oscillator.<br/>
![Oscillator](https://drive.google.com/uc?export=view&id=1xpOhJUEtagk-KzpOqFtH3PSNpS_XLM4k)

#### Working:
Reset Pin (4) is connected to the supply voltage and the Control Pin (5) is connected to the ground via a capacitor.<br/>
Threshold (6) and Trigger(2) Pins are connected together.<br/>
- When the circuit is just turned on, 6 and 2 will have 0V. So, the ouput of the first comparator will be 0 and the output of the second comparator will be 1, resulting in HIGH output.
- In this condition, the transistor will turn OFF, resulting in charging of capacitor through the path along R1 and R2.
  - While charging, when the voltage at 6 and 2 becomes just greater than (1/3)Vcc, R=0 and S=0 condition occurs, retaining the previous HIGH output.
  - When the capacitor voltage jus crosses (2/3)Vcc, S=0 and R=0, the flip-flop iis reset to logic 0, transitioning to LOW output.
  - LOW output in turn will turn the transistor ON, providing a path for the discharging of the capacitor.
  - ![AstableCharging/Discharging](https://drive.google.com/uc?export=view&id=1FgryFRPPVUYi4HqBMxPuP1f8k2hh9Cn6)
  - t1>t2 (ALWAYS): **Duty cycle will always be >50%**. Duty cycle = (t1/T)

##### Improved 555 Oscillator Duty Cycle
Addition of a Diode with forward biase resistance = R(D), enables us to have a much better control over the value of the duty cycle.
![Astable_ImprovedDTC](https://drive.google.com/uc?export=view&id=1-TJ-kqn46dAGGqnkKqLGqpvbGzyQTOqm)


