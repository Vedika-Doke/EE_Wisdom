# Electronic System Integration

### 1.1 What is meant by galvanic isolation? Why do you think isolation is crucial for any EV car?
Galvanic Isolation means that two electrical circuits have no direct conductive path between them — no shared ground or wire that allows current to flow freely.<br/>
Instead, data or power is transferred via indirect methods (e.g., transformers, opto-isolators, or capacitive coupling) to ensure safety and signal integrity.<br/>
Isolation is crucial for an EV:
1. Human Safety: Isolation ensures the high-voltage system stays quarantined, even when everything else is connected, to avoid electrocution.
2. Protecting Low-Voltage Electronics
3. Signal Integrity and Noise Rejection:
   - High-power systems (like inverters and motor drives) generate massive switching noise.
   - Isolation breaks ground loops, keeping noise localized and the signals clean.
4. Fault Containment: Avoid a catastrophic system-wide shutdown if a certain component is damaged or fails.

#### Rulebook EV 1.2.1
Galvanic Isolation – two electric circuits are defined as galvanically isolated if all of the following conditions are true:
- The resistance between both circuits is ≥500 Ω/ V, related to the maximum TS voltage of the vehicle, at a test voltage of maximum TS voltage or 250 V, whichever is higher.
- The isolation test voltage RMS, AC for 1 min, between both circuits is higher than three times the maximum TS voltage or 750 V, whichever is higher.
  - This is a stress test to ensure the isolation barrier doesn’t break down.
  - It’s the maximum AC voltage that can be applied between two isolated parts of a system for a short time without causing breakdown, arcing, or damage to the isolation barrier.
- The working voltage of the isolation barrier, if specified in the datasheet, is higher than the maximum TS voltage.
  - Working voltage is the maximum continuous voltage that the isolation barrier can safely withstand during normal operation, over the entire lifespan of the component — without degrading insulation or risking failure.
  - Because even if the isolation can survive a 2 kV spike for 1 minute (withstand voltage), that doesn’t mean it can tolerate 600 V across it 24/7.
- Capacitors that bridge galvanic isolation must be class-Y capacitors.
  - Class-Y capacitors are specifically rated for this job because of their fail-safe design: If they fail, they open the path, not short—so they don’t kill our isolation.

### 1.2 The following is a circuit proposed by a friend of yours, in an attempt to measure the TS voltage of the Motor Controller (i.e, MC+ and MC-) to detect whether it is charged to 95% of TS voltage and control the precharge circuitry. Comment whether the given circuit is rule compliant.
![Q2](https://drive.google.com/uc?export=view&id=1tbyT8TqNbwt7WbvzatPw0LfAIImWjP_D)
**The system is NOT rule compliant.**
The above circuit is basically TS → R1+R2 → LV circuitry & LV ground tied to MC− <br/>
#### EV4.3.1 The entire TS and LVS must be galvanically isolated, see EV1.2.1 and IN4.1.1.
- The rule explicitly states that TS and LV systems must be galvanically isolated
- Voltage divider directly connects TS rails to LV side through resistors — violating the very definition of isolation

### 1.3 Suppose there is a system implausibility that has made an EV car stop. Upon inspection from afar, you see that a Red and a Green LED present in the cockpit are glowing, but there are no labels on them. All other cockpit mounted LEDs are not glowing. Judging from the cockpit LEDs alone, it has been reported to you that there was an isolation failure.

#### (i) What is the prescribed device that detects any isolation failure in the TS circuit? What should be the label on the cockpit lights? Mention the rule as your justification for the label names.
An IMD: Insulation Monitoring Device is used to detect any isolation failure in the TS circuit.
![IMDred](https://drive.google.com/uc?export=view&id=1TiCyNDNrNuJJ0ExWy5cq5NX3y_37XeYa)
**TSAL: Tractive System Active Light**
![TSALgreen](https://drive.google.com/uc?export=view&id=1l6apB8ExsBWlcz56ay8E131lo-RJvKno)

#### (ii)From the AMS module, calculate the max. response value (in ohms) that such a device should have for our car.
##### EV6.3.3 The response value of the IMD must be set to ≥500 Ω/ V, related to the maximum TS voltage.
![AMS](https://drive.google.com/uc?export=view&id=1xYJLUmEOVETZsDNN9cCxUu1h5BI4MHky)
The maximum TS voltage in our car is 403.2V, following rule EV 6.3.3, the IMD must trigger a fault if resistance falls below (500 * 403.2)<br/>
Therefore the maximum response values of an IMD is 500*403.2 ohm, i.e **201600 ohm**

#### (iii)What should be the necessary course of action of any safety circuit in such a case?
In the case where an isolation fault is detected by the IMD:
- Immediate opening the AIRs (Accumulator Isolation Relays) — i.e., disconnecting the TS.
- Illumination a red cockpit LED labeled "IMD"
- Prevention of the car from moving or reactivating the TS until the fault is cleared.

## OptoCoupler
It optically couples the signal of one circuit to the other circuit, providing electrical isolation.
![OptoC](https://drive.google.com/uc?export=view&id=1lXTb9oEt7xox3KSWZd6tysb0DJgdOz10)
- It cosists of an IR LED made up of GaAs and an light sensitive device that detects the light from the LED.
- When light falls on the photo detector, it acts like a closed switch and allows the flow of current in the other circuit. <br/>
The photo-transistor and photo-darlington devices are mainly for use in DC circuits while the photo-SCR and photo-triac allow AC powered circuits to be controlled. There are many other kinds of source-sensor combinations, such as LED-photodiode, LED-LASER, lamp-photoresistor pairs, reflective and slotted optocouplers. <br/>
For analog input signal, the photo transistor is used in the linear or active region and for digital input signals, the photo transistor is used in the saturation region.<br/>

**The Current Tranfer Ratio (CTR):**
Ratio of collector current over input current (Ic/If)

### 2.1: Redesign the faulty circuit that was presented earlier, using this Optocoupler/OptoMOS in the circuit. Please justify your calculations of all the relevant components used
Doubt :((

### 3.1 Should both the contactors be closed at the same time? <Inrush current and chattering>If yes then answer the appropriate delay.
Inrush current: A huge surge of current flows immediately to charge the DC link capacitors of the inverter, which can damage the contactors or battery cells. <br/>
Chattering: If the inrush current is not controlled, it may result in unstable or repeated opening/closing of the contactors, called chattering, which harms their longevity and reliability.<br/>
- The correct sequence is:
  1. Close the precharge relay (connected in series with a precharge resistor).
  2. Close AIR− contactor.
  3. Wait for a short delay for the DC link capacitors to charge gradually through the resistor.
  4. Then close AIR+ contactor.
This delay is sufficient to allow the capacitor voltage to rise to near the accumulator voltage (typically >90%). <br/>

### 3.2 The button/signal responsible for additional activation logic is called HV_RESET in our taxonomy. Is HV_RESET part of our shutdown circuit? If yes, explain how it affects SDC and do mention the exact rule that it complies.

In our system’s taxonomy, HV_RESET is the button or signal used to re-enable the tractive system after a shutdown condition has occurred and been cleared (such as AMS fault, IMD fault, etc.)<br/>
Pressing HV_RESET restarts the system logic that energizes the precharge relay, then AIR−, then AIR+, etc.<br/>
In a nutshell, HVLV reset is the additional logic that for the re-activation of the TS after the errors and faults are rectified.

![Rule](https://drive.google.com/uc?export=view&id=1xNC15kxn5fFSSvHZ1wwb4XHE-jjOFmrf)

### 3.3 Rule EV 6.1.4 compels us to have TSMS as the final element of the shutdown circuit before AIRs and other relays. Explain any reasoning that you can think of for this compliance.
Placing it last in the shutdown circuit ensures that cutting off the TSMS guarantees a full shutdown of the tractive system—including the AIRs and precharge/discharge relays. <br/>
In emergencies, it provides a single, reliable point of intervention to disable HV power to the car instantly.
