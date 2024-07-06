PROGRAM circuit_breakers
  VAR
    Open_CB_33KV AT %QX0.0 : BOOL;
    Open_CB_TRSF AT %QX0.1 : BOOL;
    Open_CB_11KV AT %QX0.2 : BOOL;
    Open_CB_Load AT %QX0.3 : BOOL;
    Reset_All_CB AT %IX0.0 : BOOL;
    Status_CB_33KV AT %IX0.1 : BOOL;
    Status_CB_TRSF AT %IX0.2 : BOOL;
    Status_CB_11KV AT %IX0.3 : BOOL;
    Status_CB_Load AT %IX0.4 : BOOL;
    Fault_on_33BUS_Current AT %IW0 : INT;
    Fault_on_TRSF_Current AT %IW1 : INT;
    Fault_on_11BUS_Current AT %IW2 : INT;
    Fault_on_Load_Current AT %IW3 : INT;
  END_VAR
  VAR
    Threshold500 : INT := 500;
    TON0 : TON;
    TON1 : TON;
    TON2 : TON;
    TON3 : TON;
    SR0 : SR;
    SR1 : SR;
    SR2 : SR;
    SR3 : SR;
    _TMP_ABS50_OUT : INT;
    _TMP_GT5_OUT : BOOL;
    _TMP_ABS29_OUT : INT;
    _TMP_GT13_OUT : BOOL;
    _TMP_ABS45_OUT : INT;
    _TMP_GT39_OUT : BOOL;
    _TMP_ABS27_OUT : INT;
    _TMP_GT10_OUT : BOOL;
  END_VAR

  _TMP_ABS50_OUT := ABS(Fault_on_TRSF_Current);
  _TMP_GT5_OUT := GT(_TMP_ABS50_OUT, Threshold500);
  TON1(IN := _TMP_GT5_OUT, PT := T#0.1s);
  SR1(S1 := TON1.Q, R := Reset_All_CB);
  Open_CB_TRSF := SR1.Q1;
  _TMP_ABS29_OUT := ABS(Fault_on_11BUS_Current);
  _TMP_GT13_OUT := GT(_TMP_ABS29_OUT, Threshold500);
  TON2(IN := _TMP_GT13_OUT, PT := T#0.1s);
  SR2(S1 := TON2.Q, R := Reset_All_CB);
  Open_CB_11KV := SR2.Q1;
  _TMP_ABS45_OUT := ABS(Fault_on_Load_Current);
  _TMP_GT39_OUT := GT(_TMP_ABS45_OUT, Threshold500);
  TON3(IN := _TMP_GT39_OUT, PT := T#0.1s);
  SR3(S1 := TON3.Q, R := Reset_All_CB);
  Open_CB_Load := SR3.Q1;
  _TMP_ABS27_OUT := ABS(Fault_on_33BUS_Current);
  _TMP_GT10_OUT := GT(_TMP_ABS27_OUT, Threshold500);
  TON0(IN := _TMP_GT10_OUT, PT := T#0.1s);
  SR0(S1 := TON0.Q, R := Reset_All_CB);
  Open_CB_33KV := SR0.Q1;
END_PROGRAM


CONFIGURATION Config0

  RESOURCE Res0 ON PLC
    TASK TaskMain(INTERVAL := T#10ms,PRIORITY := 0);
    PROGRAM Inst0 WITH TaskMain : circuit_breakers;
  END_RESOURCE
END_CONFIGURATION
