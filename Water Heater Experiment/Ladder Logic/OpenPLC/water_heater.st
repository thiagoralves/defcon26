FUNCTION_BLOCK Temp_Conversion
  VAR_INPUT
    raw_temp : UINT;
  END_VAR
  VAR
    voltage : REAL;
    resistance : REAL;
    steinhart : REAL;
    THERMISTORNOMINAL : REAL := 10000.0;
    TEMPERATURENOMINAL : REAL := 25.0;
    BCOEFFICIENT : REAL := 3950.0;
    SERIESRESISTOR : REAL := 10000.0;
  END_VAR
  VAR_OUTPUT
    converted_temp : INT;
    voltage_out : INT;
    resistance_out : INT;
  END_VAR

  (* Calculate Voltage *)
  voltage := UINT_TO_REAL(raw_temp);
  voltage := voltage * 0.0001535107446;

  (* Calculate Resistance *)
  resistance := voltage / 10.0;
  resistance := 1.0 / resistance;
  resistance := resistance - 1.0;
  resistance := resistance * 10000.0;


  (* Calculate Steinhart *)
  steinhart := resistance / THERMISTORNOMINAL; (* (R/Ro) *)
  steinhart := LN(steinhart); (* ln(R/Ro) *)
  steinhart := steinhart / BCOEFFICIENT; (* 1/B * ln(R/Ro) *)
  steinhart := steinhart + 1.0 / (TEMPERATURENOMINAL + 273.15); (* (1/To) *)
  steinhart := 1.0 / steinhart; (* Invert *)
  steinhart := steinhart - 273.15; (* Convert to C *)

  (* Calculate the error correction *)
  steinhart := steinhart + 9.451149;
  steinhart := steinhart / 0.894263;
  steinhart := steinhart * 100.0; (* Multiply by 100 to get 2 decimals *)

  voltage_out := REAL_TO_INT(voltage * 100.0);
  resistance_out := REAL_TO_INT(resistance);
  converted_temp := REAL_TO_INT(steinhart);

  (*
  voltage := UINT_TO_REAL(raw_temp);
  voltage := 10000.0 / (65536.0 / voltage - 1.0);
  voltage := voltage / 10000.0;
  voltage := LN(voltage);
  voltage := 1.0 / ((1.0/298.15) + (voltage * (1.0/3950.0)));
  voltage := voltage - 273.15;
  voltage := voltage * 100.0;
  converted_temp := REAL_TO_INT(voltage);
  *)
END_FUNCTION_BLOCK

PROGRAM My_Program
  VAR
    raw_temp AT %IW0 : UINT;
    Ref_Voltage AT %QW0 : UINT := 65535;
    converted_temp AT %QW1 : INT;
    voltage AT %QW7 : INT;
    resistance AT %QW8 : INT;
    setpoint AT %QW2 : INT := 4000;
    mode_register AT %QW4 : INT;
    heater_control AT %QW5 : INT;
    heater_man AT %QX99.0 : BOOL;
    heater_auto AT %QX99.1 : BOOL;
    heater AT %QX0.0 : BOOL;
  END_VAR
  VAR
    Temp_Conversion0 : Temp_Conversion;
    SUB31_OUT : INT;
    LE9_OUT : BOOL;
    GE18_OUT : BOOL;
    EQ6_OUT : BOOL;
    EQ2_OUT : BOOL;
    EQ26_OUT : BOOL;
  END_VAR

  Temp_Conversion0(raw_temp := raw_temp);
  converted_temp := Temp_Conversion0.converted_temp;
  voltage := Temp_Conversion0.voltage_out;
  resistance := Temp_Conversion0.resistance_out;
  SUB31_OUT := SUB(setpoint, 150);
  LE9_OUT := LE(converted_temp, SUB31_OUT);
  IF LE9_OUT THEN
    heater_auto := TRUE; (*set*)
  END_IF;
  GE18_OUT := GE(converted_temp, setpoint);
  IF GE18_OUT THEN
    heater_auto := FALSE; (*reset*)
  END_IF;
  EQ6_OUT := EQ(heater_control, 1);
  heater_man := EQ6_OUT;
  EQ2_OUT := EQ(mode_register, 0);
  EQ26_OUT := EQ(mode_register, 1);
  heater := heater_auto AND EQ2_OUT OR heater_man AND EQ26_OUT;
END_PROGRAM


CONFIGURATION Config0

  RESOURCE Res0 ON PLC
    TASK TaskMain(INTERVAL := T#50ms,PRIORITY := 0);
    PROGRAM Inst0 WITH TaskMain : My_Program;
  END_RESOURCE
END_CONFIGURATION
